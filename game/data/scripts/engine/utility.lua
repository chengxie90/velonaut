function loadDataFile(name, type)
	local base = "./data"
	local folder = "./"
	local ext = ""
	if type == "scene" then
		folder = "scenes"
		ext = ".scene"
	elseif type == "object" then
		folder = "objects"
		ext = ".object"
	end
	local filename = base .. "/" .. folder .. "/" .. name .. ext
	local ret = dofile(filename)
	assert(ret)
	return ret
end