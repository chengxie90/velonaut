package.path = "./data/scripts/?.lua;" .. package.path

-- We just load these utility functions once here and avoid requiring them again
require "engine.lib.strict"
require "engine.class"
require "engine.vector"
require "engine.color"
require "engine.matrix"
--

require "engine.utility"
require "engine.scene"

local Gui = require "engine.gui.c"
local Network = require "engine.network.c"


App = class() -- this is a singleton

function App.start()
    local config = dofile("./data/game.config")
    assert(config.scene)
    App.loadScene(config.scene)

	Gui.loadFont("./data/ui/Delicious-Bold.otf")
	Gui.loadFont("./data/ui/Delicious-BoldItalic.otf")
	Gui.loadFont("./data/ui/Delicious-Italic.otf")
	Gui.loadFont("./data/ui/Delicious-Roman.otf")

	Gui.loadDocument("./data/ui/demo.rml")

	local function onGameMessageReceived(msg)
		print("Game message received yo!" .. msg.eventType)
	end

	local function onConnectedToServer()
		print("Connected to server yo!")
		Network.sendMessage( "{ eventType = 'blu' }" )
	end

	local function onConnectionFailed()
		print("on connection failed yo!")
	end

	local function onDisconnect()
		print("client disconnected yo!")
	end

	local function onServerResponse(resp)
		print("Received player list: ")
		for c = 1, #resp do
		  print (resp[c])
		end
	end

	Network.addEventListener("game_message", onGameMessageReceived)
	Network.addEventListener("server_response", onServerResponse)
	Network.addEventListener("connection_failed", onConnectionFailed)
	Network.addEventListener("connected_to_server", onConnectedToServer)
	Network.addEventListener("disconnect", onDisconnect)


	local function onStartServer()
		Network.startServer(60001)
		Network.setMaxIncomingConnections(12)
		print("starting server")
	end

	local function onStartClient()
		Network.connectToServer("127.0.0.1", 60001)	
		print("starting client")
	end

	local function onBtnDisconnect()
--		Network.sendMessage("{ eventType='BlaBLiBlaBla'} ")
		Network.sendRequest("getPlayerList")
		
--		Network.shutdownClient()
	end


	Gui.addEventListener("btn_start_disconnect", "click", onBtnDisconnect)
	Gui.addEventListener("btn_start_server", "click", onStartServer)
	Gui.addEventListener("btn_start_client", "click", onStartClient)


end

function App.update(dt)
    App._scene:update(dt)
end

function App.terminate()
    
end

function App:activeScene()
    return App._scene
end

function App.loadScene(name)
    local data = loadDataFile(name, "scene")
    App._scene = Scene()
    App._scene:load(data)
    App._scene:start()
end

App.start()
