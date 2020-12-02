hook.Add("OnUpdate", "update", function(game)
	-- Do nothing
end)

hook.Add("PostExecute", "post_exec", function(game)
	print("Dumping prototypes!\n")
	
	local resource_manager = game.ResourceManager
	for i=0, resource_manager.NumPrototypes
	do
		local test_prototype = resource_manager:GetPrototype(i)
		if test_prototype ~= nil then
			local output = string.format("[ Lua ] %s\n", test_prototype.Name)
			print(output)
		end
	end
	
end)