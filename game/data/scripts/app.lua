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

	d, s = LScript_Bla(13.0, "hello from lua")
	print(s)
	print("got from c " .. d );
	print("got from c " .. s );

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
