dofile("data/scripts/utility.lua")

local Scene = require("data/scripts/scene")
local Matrix = require("data/scripts/matrix")

local App = {}

App._scenes = {}

App.init = function()
	print("App init")
	-- load scene file

	-- parse scence file

	-- add a test scene
	App._scenes = {Scene:new()}
end

App.init()
