local Gui = require "engine.gui.c"
local Network = require "engine.network.c"

Hud = class(Behavior)

function Hud:start()
	print("HUD start")
	Gui.loadFont("./data/ui/font/DINPro-Black.ttf")
	Gui.loadFont("./data/ui/font/DINPro-Bold.ttf")
	Gui.loadFont("./data/ui/font/DINMedium.ttf")
	self.id = Gui.loadDocument("./data/ui/hud.rml")

	self._numCheckpoints = 0;
	self._goCounter = 0;
	self._isShown = false;
	
end


function Hud:show()	

	self._isShown = true

	local function onGameMessageReceived(event)

		if event.eventType == "countdown" then
			Gui.setText("txt_status", event.count);
			return		
		end

		if event.eventType == "gamestart" then
			Gui.setText("txt_status", "Go!");
			return
		end

		if event.eventType == "gameover" then

			if App.scene():player():getComponent("Player"):getId() == event.winnerId then
				Gui.setText("game_over_caption", "You win!")
			else
				Gui.setText("game_over_caption", "You lose!")
			end
			Gui.removeClass("game_over", "isHidden");
			return
		end	
	end

	local function onBtnBack()
		Network.shutdownServer();
		Network.shutdownClient();
		os.exit(0)
	end


	Gui.showDocument(self.id)
	local numCP = App.scene():findObject("tunnel"):getComponent("Tunnel"):getNumCheckpoints()
	Gui.setText("lbl_total_checkpoints", tostring(numCP))
	Network.addEventListener("game_message", onGameMessageReceived)
	Gui.addEventListener("gameoverBtnBack", "click", onBtnBack)
	self:setNoPickup()

end

function Hud:update()

	if not self._isShown then
		return
	end

	if  self._goCounter > 280 then
		Gui.setText("txt_status", "");
		self._isShown = false
	else
		self._goCounter = self._goCounter + 1
	end
end


function Hud:hide()
	Gui.hideDocument( self.id )
end


function Hud:setPickupNumber(num)
	Gui.setText("lbl_num_pickups", tostring(num))
end

function Hud:setPickupName(name)
	Gui.setText("lbl_current_pickup", name)
end

function Hud:setNoPickup()
	Gui.setText("lbl_current_pickup", "No Item")
	Gui.setText("lbl_num_pickups", "0")
end

function Hud:incrementCheckpoints()
	self._numCheckpoints = self._numCheckpoints + 1
	Gui.setText("lbl_num_checkpoints", tostring(self._numCheckpoints))
end

