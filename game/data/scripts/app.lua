package.path = "./data/scripts/?.lua;" .. package.path

Matrix = require("data/scripts/matrix")
require "utility"
require "scene"

App = createClass() -- this is a singleton

function App.init()
    local config = loadData("config")
    assert(config.scene)
    App.loadScene(config.scene)
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
