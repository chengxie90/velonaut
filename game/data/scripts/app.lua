package.path = "./data/scripts/?.lua;" .. package.path

require "mathematics"
require "utility"
require "scene"

App = createClass() -- this is a singleton

function test( d0, d1 )
	print(d0)
	print(d1)
	return d0, d1
end

function App.init()
    local config = loadData("config")
    assert(config.scene)
    App.loadScene(config.scene)
	gui.loadFont("data/ui/Delicious-Bold.otf")
	gui.loadFont("data/ui/Delicious-BoldItalic.otf")
	gui.loadFont("data/ui/Delicious-Italic.otf")
	gui.loadFont("data/ui/Delicious-Roman.otf")
	gui.loadDocument("data/ui/demo.rml")

	local wu = "WUUUUUUUUU"

	function onClick()
		print("onClick! " .. wu)
	end

	gui.addEventListener("btn", "click", onClick)



end

function App.update( u)
	print("update" .. u) 

end

function App.terminate()
    
end

function App.loadScene(name)
    App.scene = Scene.new()
    local data = loadData(name, "scene")
    assert(data)
    App.scene:init(data)
end

App.init()
