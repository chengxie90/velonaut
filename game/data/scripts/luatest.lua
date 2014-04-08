dofile("data/scripts/utility.lua")
dofile("data/scripts/scene.lua")

normalscene = Scene:new()
ascene = AScene:new()

normalscene:find("bla")
ascene:find("blu")

normalscene:a()
ascene:a()
ascene:b()


App = {}

App.init = function()	
	print("App init")
end

brownie = function( i )
	return i * 2;
end

