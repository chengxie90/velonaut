dofile("data/scripts/utility.lua")

local Scene = require("data/scripts/scene")
local Matrix = require("data/scripts/matrix")

App = {}

App._scenes = {}

App.init = function()
	print("App init")

	-- load scene file
	dofile("data/config.lua")

end

App.onConfigLoaded = function(config)

	-- parse configs and build scenes
	for key,sceneConfig in pairs(config.scenes) do
		
		print(sceneConfig.name)
		local s = Scene:new{}
		--and so forth		
					
	end

end


App.init()
