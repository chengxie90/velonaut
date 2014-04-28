local Gui = require "engine.gui.c"
local Network = require "engine.network.c"

Hud = class(Behavior)

Hud.id = nil
function Hud:start()
	print("HUD start")
	Gui.loadFont("./data/ui/font/DINPro-Black.ttf")
	Gui.loadFont("./data/ui/font/DINPro-Bold.ttf")
	Gui.loadFont("./data/ui/font/DINMedium.ttf")
	Hud.id = Gui.loadDocument("./data/ui/hud.rml")

	self._numCheckpoints = 0;
	
end


function Hud:show()	

	local function onGameMessageReceived(event)

		if event.eventType == "countdown" then
			Gui.setText("txt_status", event.count);
			return		
		end

		if event.eventType == "gamestart" then
			Gui.setText("txt_status", "");
			return
		end	
	end


	Gui.showDocument(self.id)
	local numCP = App.scene():findObject("tunnel"):getComponent("Tunnel"):getNumCheckpoints()+1
	Gui.setText("lbl_total_checkpoints", tostring(numCP))
	Network.addEventListener("game_message", onGameMessageReceived)
	self:setNoPickup()
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

