local Gui = require "engine.gui.c"
local Network = require "engine.network.c"

MainMenu = class(Behavior)

function MainMenu:start()

	self.menus = {	"menu_main",
					"menu_join_race",
					"menu_settings",
					"menu_credits"
				 }

	self.isServer = false

	local function onGameMessageReceived(event)
		if event.eventType == "welcome" then
			print( "Welcome, you been assigned ID " .. event.id )
			App.playerId = event.id
			return
		end	

		if event.eventType == "playerlist" then
			App.players = event.players
			Gui.setText("lbl_host",  event.host);
			Gui.setText("lbl_host_ip",  event.hostip);
			
			local list = ""

			for i, player in ipairs(App.players) do
				list = list .. "<li class='listitem'>" .. player.name .. "</li>"
			end

			local minPlayers = tonumber(Gui.getAttribute("fld_min_num_players", "value"))
			local numPlayers = #App.players
			Gui.setText("lbl_current_players", "Current Players (" .. numPlayers .. "/" .. minPlayers .. ")");
			Gui.setText("lst_players", list);
			return
		end

		if event.eventType == "gameinit" then
			print("received gameinit")	
			if App.players then
				App._scene:loadPlayers(App.players, App.playerId)
				Network.RPC("setPlayerReady", "")
				Gui.addClass("main_menu_wrp", "isHidden")
			end
			return
		end

		if event.eventType == "countdown" then
			Gui.setText("txt_status", event.count);	
			return		
		end

		if event.eventType == "gamestart" then
			Gui.setText("txt_status", "");
			return
		end	

	end

	local function onConnectedToServer()
		Network.RPC("setPlayerName", Gui.getAttribute("fld_player_name", "value"))
		if self.isServer then Network.RPC("setNumPlayers", Gui.getAttribute("fld_min_num_players", "value")) end
	end

	local function onConnectionFailed()
		print("on connection failed yo!")
	end

	local function onDisconnect()
		print("client disconnected yo!")
	end

	local function onServerFound( serverip )
		print("Found server at " .. serverip)
		Network.connectToServer(serverip, 60001)	
	end

	local function onStartGame()
		Network.RPC("startGame", "")
	end

	local function onBtnJoinRace()
		print("onBtnJoinRace")
	end


	local function onBtnBack()
		Network.shutdownServer();
		Network.shutdownClient();
		self:showMenu("menu_main")
	end

	local function onSettingsBtnBack()
		self:showMenu("menu_main")
	end

	local function onBtnJoinRace()
		self.isServer = false
		local serverAdress = Gui.getAttribute("fld_server_address","value");
		local serverPort = tonumber(Gui.getAttribute("fld_server_port","value"))
		
		Network.startClient()

		if Gui.hasClass("chb_finder_server","checked") then
			Network.findServer( serverPort )
		else
			Network.connectToServer(serverAdress, serverPort)
		end

		self:showMenu("menu_join_race")		
	end

	local function onBtnHostRace()
		self.isServer = true
		self:showMenu("menu_join_race")

		local serverPort = tonumber(Gui.getAttribute("fld_server_port","value"))

		Network.startServer(serverPort)
		os.execute("sleep " .. 0.2)
		Network.startClient()
		Network.connectToServer("127.0.0.1", serverPort)
		Network.setMaxIncomingConnections(12)
		
	end

	local function onBtnSettings()
		self:showMenu("menu_settings")
	end

	local function onBtnCredits()
		self:showMenu("menu_credits")
	end

	local function onCheckboxInvertControls()
		if Gui.hasClass("chb_invert_controls", "checked") then
			print("hasClass")
			Gui.removeClass("chb_invert_controls", "checked")
		else
			print("hasNotClass")
			Gui.addClass("chb_invert_controls", "checked")
		end
	end

	local function onCheckboxFindServer()
		if Gui.hasClass("chb_finder_server", "checked") then
			print("hasClass")
			Gui.removeClass("chb_finder_server", "checked")
		else
			print("hasNotClass")
			Gui.addClass("chb_finder_server", "checked")
		end
	end

	Network.addEventListener("server_found", onServerFound)
	Network.addEventListener("game_message", onGameMessageReceived)
	Network.addEventListener("connection_failed", onConnectionFailed)
	Network.addEventListener("connected_to_server", onConnectedToServer)
	Network.addEventListener("disconnect", onDisconnect)

	Gui.loadFont("./data/ui/font/DINPro-Black.ttf")
	Gui.loadFont("./data/ui/font/DINPro-Bold.ttf")
	Gui.loadFont("./data/ui/font/DINMedium.ttf")
	Gui.loadDocument("./data/ui/mainmenu.rml")
	Gui.removeClass("menu_join_race", "isHidden")

	Gui.addEventListener("btn_join_race", "click", onBtnJoinRace)
	Gui.addEventListener("btn_host_race", "click", onBtnHostRace)
	Gui.addEventListener("btn_settings", "click", onBtnSettings)
	Gui.addEventListener("btn_credits", "click", onBtnCredits)
	Gui.addEventListener("btn_quit", "click", self.onBtnQuit)
	Gui.addEventListener("chb_invert_controls", "click", onCheckboxInvertControls)
	Gui.addEventListener("chb_finder_server", "click", onCheckboxFindServer)	
	Gui.addEventListener("hostraceBtnBack", "click", onBtnBack)
	Gui.addEventListener("settingsBtnBack", "click", onSettingsBtnBack)
	Gui.addEventListener("creditsBtnBack", "click", onBtnBack)

	Gui.setAttribute("fld_player_name", "value", "Guest");

	self:showMenu("menu_main")

end

function MainMenu:showMenu(newMenu)

	for i,menu in pairs( self.menus ) do
		print("menu: " .. menu)
		Gui.removeClass( menu, "isHidden" )
		Gui.addClass( menu, "isHidden" )
	end

	Gui.removeClass( newMenu, "isHidden" )
end