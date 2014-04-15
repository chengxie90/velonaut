dofile("data/scripts/utility.lua")

local Scene = require("data/scripts/scene")
Matrix = require("data/scripts/matrix")

App = {}
App._scenes = {}

App.init = function( p )

	print(p)

	-- load file
	print("App init")

	-- load scene file
	dofile("data/config.lua")

	lscript_test(123, "Test", 29.6, {{4,25,3},{4,5,6},{7,8,9}})
	
end


App.onConfigLoaded = function(config)

	-- parse configs and build scenes
	for key,sceneConfig in pairs(config.scenes) do
		
		print(sceneConfig.name)
		local s = Scene:new{}
		--and so forth		
					
	end

end
