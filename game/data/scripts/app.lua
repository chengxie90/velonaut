dofile("data/scripts/utility.lua")

local Scene = require("data/scripts/scene")
Matrix = require("data/scripts/matrix")

Matrix:new({1,2,3})

function passMatrix(m)
--[[
	print(m[1][1])
	print(m[1][2])
	print(m[1][3])

	print(m[2][1])
	print(m[2][2])
	print(m[2][3])

	print(m[3][1])
	print(m[3][2])
	print(m[3][3])
]]--

end

function passVector(v)
	print(v[1])
	print(v[2])
	print(v[3])
end

function hello(d1,m,d2,s,i)
	print(d1)

	print(m[1])
	print(m[2])
	print(m[3])

	print(d2)

	print(s)
	print(i)
	return d1,"shubidu",69
	-- return 51.0,{{1,2,3},{4,5,6},{7,8,9}},45.0
end



App = {}
App._scenes = {}

App.init = function()
	-- load file
	print("App init")

	-- load scene file
	dofile("data/config.lua")

	lscript_bla(123, "BlaBli", 29.6)

end


App.onConfigLoaded = function(config)

	-- parse configs and build scenes
	for key,sceneConfig in pairs(config.scenes) do
		
		print(sceneConfig.name)
		local s = Scene:new{}
		--and so forth		
					
	end

end
