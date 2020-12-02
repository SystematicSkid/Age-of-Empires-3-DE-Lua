#ifndef POLYHOOK_2_0_HWBPHOOK_HPP
#define POLYHOOK_2_0_HWBPHOOK_HPP

#include <cassert>

#include "headers/Exceptions/AVehHook.hpp"
#include "headers/Misc.hpp"

namespace PLH {

class HWBreakPointHook : public AVehHook {
public:
	HWBreakPointHook(const uint64_t fnAddress, const uint64_t fnCallback);
	HWBreakPointHook(const char* fnAddress, const char* fnCallback);
	~HWBreakPointHook();

	virtual bool hook() override;
	virtual bool unHook() override;
	auto getProtectionObject() {
		return finally([=] () {
			hook();
		});
	}
private:
	uint64_t m_fnCallback;
	uint64_t m_fnAddress;
	uint8_t m_regIdx;

	LONG OnException(EXCEPTION_POINTERS* ExceptionInfo) override;
};
}

#endif