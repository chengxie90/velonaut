dofile("data/scripts/utility.lua")

local Matrix = require "data/scripts/matrix"
local Scene = require "data/scripts/scene"

local test_scene = Scene:new()
test_scene:find("bla")
test_scene:a()

local mtx = Matrix {{1,2},{3,4}}
mtx = mtx * 2
print(mtx)

App = {}

App.init = function()	
	print("App init")
end

brownie = function( i )
	return i * 2;
end

