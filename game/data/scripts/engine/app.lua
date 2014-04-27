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

local Gui = require "engine.gui.c"
local Network = require "engine.network.c"


App = class() -- this is a singleton

function App.start()
	math.epsilon = 0.00001

	App.gamestarted = false

    local config = dofile("./data/game.config")
    App.loadScene(config.scene)

	Gui.loadFont("./data/ui/font/Delicious-Bold.otf")
	Gui.loadFont("./data/ui/font/Delicious-BoldItalic.otf")
	Gui.loadFont("./data/ui/font/Delicious-Italic.otf")
	Gui.loadFont("./data/ui/font/Delicious-Roman.otf")

	Gui.loadDocument("./data/ui/mainmenu.rml")

	local function onGameMessageReceived(event)
		
		if event.eventType == "welcome" then
			print( "Welcome, you been assigned ID " .. event.id )
			App.playerId = event.id
			return
		end	

		if event.eventType == "playerlist" then
			print("received playerlist")
			App.players = event.players
			
			for i, player in ipairs(App.players) do
				print (player.name )
				print (player.id )
				Gui.setText("txt_status", "Connnect as " .. player.name);
			end

			return
		end

		if event.eventType == "gameinit" then
			print("received gameinit")	
			print("seed: " .. event.seed)			
			Gui.setText("txt_status", "Initializing...");

			App.scene():loadPlayers(App.players, App.playerId)		
			return
		end

		if event.eventType == "countdown" then
			print("received gameinit")	
			print("Countdown: " .. event.count)
			Gui.setText("txt_status", event.count);	
			return		
		end

		if event.eventType == "gamestart" then
			print("GOOOOOOOO!!!!!!!!!")
			Gui.setText("txt_status", "GOOOOOOOO!!!!!!!!!");
			App.gamestarted = true	
			return
		end	

	end

	local function onConnectedToServer()

		print("Connected to server yo! ")
		Network.RPC("setPlayerName", "Nate")

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

	local function onServerFound( serverip )
		print("Found server at " .. serverip)
		Network.connectToServer(serverip, 60001)	
	end
	
	Network.addEventListener("server_found", onServerFound)
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

		print("Finding server...") 
		Network.findServer( 60001 )
		--Network.connectToServer("10.116.74.71",60001)

	end

	local function onStartGame()
		Network.RPC("startGame", "")
	end

	local function onDisconnectClient()
		Network.shutdownClient()
	end

	local function onPlayerReady()
		Network.RPC("setPlayerReady", "")
	end

	Gui.addEventListener("btn_start_server", "click", onStartServer)
	Gui.addEventListener("btn_start_client", "click", onStartClient)
	Gui.addEventListener("btn_start_game", "click", onStartGame)
	Gui.addEventListener("btn_player_ready", "click", onPlayerReady)
	Gui.addEventListener("btn_disconnect", "click", onDisconnectClient)

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
