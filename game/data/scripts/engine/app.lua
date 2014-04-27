package.path = "./data/scripts/?.lua;" .. package.path

-- We just load these utility functions once here and avoid requiring them again
require "engine.lib.strict"
require "engine.class"
require "engine.vector"
require "engine.color"
require "engine.matrix"
require "engine.input"
--

require "engine.utility"
require "engine.scene"

App = class() -- this is a singleton

function App.start()
	print("app start")
	math.epsilon = 0.00001

	local config = dofile("./data/game.config")
    App.loadScene(config.scene)
end

function App.update(dt)
    App._scene:update(dt)
    Input.update()

end

function App.terminate()
    
end

function App.scene()
    return App._scene
end

function App.loadScene(name)
    local data = loadDataFile(name, "scene")
    App._scene = Scene()
    App._scene:load(data)
    App._scene:start()
end

App.start()
