hook.Add("PostExecute", "post_exec", function(game)
	print("Testing syscalls...\n")
	exit()
end)