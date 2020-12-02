#include "headers/PE/EatHook.hpp"

PLH::EatHook::EatHook(const std::string& apiName, const std::wstring& moduleName, const char* fnCallback, uint64_t* userOrigVar)
	: EatHook(apiName, moduleName, (uint64_t)fnCallback, userOrigVar)
{}

PLH::EatHook::EatHook(const std::string& apiName, const std::wstring& moduleName, const uint64_t fnCallback, uint64_t* userOrigVar)
	: m_apiName(apiName)
	, m_moduleName(moduleName)
    , m_userOrigVar(userOrigVar)
    , m_fnCallback(fnCallback)
	, m_trampoline(0)
	, m_allocator(0)
{}

PLH::EatHook::~EatHook() {
	if (m_trampoline != 0) {
		VirtualFree((char*)m_trampoline, m_trampolineSize, MEM_RELEASE) ;
		m_trampoline = 0;
	}

	if (m_allocator != 0) {
		delete m_allocator;
		m_allocator = 0;
	}
}

bool PLH::EatHook::hook() {
	assert(m_userOrigVar != nullptr);
	uint32_t* pExport = FindEatFunction(m_apiName, m_moduleName);
	if (pExport == nullptr)
		return false;

	uint64_t offset = m_fnCallback - m_moduleBase;

	/* account for when offset to our function is beyond EAT slots size. We
	instead allocate a small trampoline within +- 2GB which will do the full
	width jump to the final destination, and point the EAT to the stub.*/
	if (offset > std::numeric_limits<uint32_t>::max()) {
		m_allocator = new PageAllocator(m_moduleBase, 0x80000000);
		m_trampoline = m_allocator->getBlock(m_trampolineSize);
		if (m_trampoline == 0) {
			ErrorLog::singleton().push("EAT hook offset is > 32bit's. Allocation of trampoline necessary and failed to find free page within range", ErrorLevel::INFO);
			return false;
		}

		PLH::ADisassembler::writeEncoding(makeAgnosticJmp(m_trampoline, m_fnCallback));
		offset = m_trampoline - m_moduleBase;

		ErrorLog::singleton().push("EAT hook offset is > 32bit's. Allocation of trampoline necessary", ErrorLevel::INFO);
	}

	// Just like IAT, EAT is by default a writeable section
	// any EAT entry must be an offset
	MemoryProtector prot((uint64_t)pExport, sizeof(uintptr_t), ProtFlag::R | ProtFlag::W);
	m_origFunc = *pExport; // original offset
	*pExport = (uint32_t)offset;
	m_hooked = true;
	*m_userOrigVar = m_moduleBase + m_origFunc; // original pointer (base + off)
	return true;
}

bool PLH::EatHook::unHook() {
	assert(m_userOrigVar != nullptr);
	assert(m_hooked);
	if (!m_hooked)
		return false;

	uint32_t* pExport = FindEatFunction(m_apiName, m_moduleName);
	if (pExport == nullptr)
		return false;

	MemoryProtector prot((uint64_t)pExport, sizeof(uintptr_t), ProtFlag::R | ProtFlag::W);
	*pExport = (uint32_t)m_origFunc;
	m_hooked = false;
	*m_userOrigVar = NULL;

	if (m_trampoline != 0) {
		VirtualFree((char*)m_trampoline, m_trampolineSize, MEM_RELEASE);
		m_trampoline = 0;
	}

	return true;
}

uint32_t* PLH::EatHook::FindEatFunction(const std::string& apiName, const std::wstring& moduleName) {
#if defined(_WIN64)
	PEB* peb = (PPEB)__readgsqword(0x60);
#else
	PEB* peb = (PPEB)__readfsdword(0x30);
#endif

	uint32_t* pExportAddress = nullptr;
	PEB_LDR_DATA* ldr = (PPEB_LDR_DATA)peb->Ldr;

	// find loaded module from peb
	for (LDR_DATA_TABLE_ENTRY* dte = (LDR_DATA_TABLE_ENTRY*)ldr->InLoadOrderModuleList.Flink;
        dte->DllBase != NULL;
        dte = (LDR_DATA_TABLE_ENTRY*)dte->InLoadOrderLinks.Flink) {

        // try all modules if none given, otherwise only try specified
        const ci_wstring_view baseModuleName{dte->BaseDllName.Buffer, dte->BaseDllName.Length / sizeof(wchar_t)};
        if (!moduleName.empty() && baseModuleName.compare(moduleName.data()) != 0)
            continue;

		//std::wcout << moduleName << L" Found module" << std::endl;

		m_moduleBase = (uint64_t)dte->DllBase;

		pExportAddress = FindEatFunctionInModule(apiName);
		if (pExportAddress != nullptr)
			return pExportAddress;
	}

	if (pExportAddress == nullptr) {
		ErrorLog::singleton().push("Failed to find export address from requested dll", ErrorLevel::SEV);
	}
	return pExportAddress;
}

uint32_t* PLH::EatHook::FindEatFunctionInModule(const std::string& apiName) {
	assert(m_moduleBase != NULL);
	if (m_moduleBase == NULL)
		return NULL;

	IMAGE_DOS_HEADER* pDos = (IMAGE_DOS_HEADER*)m_moduleBase;
	IMAGE_NT_HEADERS* pNT = RVA2VA(IMAGE_NT_HEADERS*, m_moduleBase, pDos->e_lfanew);
	IMAGE_DATA_DIRECTORY* pDataDir = (IMAGE_DATA_DIRECTORY*)pNT->OptionalHeader.DataDirectory;

	if (pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == NULL) {
		ErrorLog::singleton().push("PEs without export tables are unsupported", ErrorLevel::SEV);
		return NULL;
	}

	IMAGE_EXPORT_DIRECTORY* pExports = RVA2VA(IMAGE_EXPORT_DIRECTORY*, m_moduleBase, pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	uint32_t* pAddressOfFunctions = RVA2VA(uint32_t*, m_moduleBase, pExports->AddressOfFunctions);
	uint32_t* pAddressOfNames = RVA2VA(uint32_t*, m_moduleBase, pExports->AddressOfNames);
	uint16_t* pAddressOfNameOrdinals = RVA2VA(uint16_t*, m_moduleBase, pExports->AddressOfNameOrdinals);

	for (uint32_t i = 0; i < pExports->NumberOfNames; i++)
	{
        if(my_narrow_stricmp(RVA2VA(char*, m_moduleBase, pAddressOfNames[i]),
                             apiName.c_str()) != 0)
			continue;

		// std::cout << RVA2VA(char*, m_moduleBase, pAddressOfNames[i]) << std::endl;
		uint16_t iExportOrdinal = pAddressOfNameOrdinals[i];
		uint32_t* pExportAddress = &pAddressOfFunctions[iExportOrdinal];

		return pExportAddress;
	}

	ErrorLog::singleton().push("API not found before end of EAT", ErrorLevel::SEV);
	return nullptr;
}
