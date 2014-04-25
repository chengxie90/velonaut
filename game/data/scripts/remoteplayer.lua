require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

local Network = require "engine.network.c"

RemotePlayer = class(Behavior)

function RemotePlayer:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.Transform = self:getComponent("Transform")
	
	local function onGameMessageReceived(event)

		if event.eventType == "playerUpdate" and event.playerId == self:getId() then		
			self.RigidBody:clearForces()
			
			local curPosition = (Vector(event.position) - self.Transform:position()) / 0.03;

--			self.RigidBody:setPosition(Vector(event.position))
			self.RigidBody:setOrientation(Vector(event.orientation))
			self.RigidBody:setLinearVelocity(curPosition)
			self.RigidBody:setAngularVelocity(Vector(event.angularVelo))
			self.RigidBody:applyCentralForce(Vector(event.force))
			self.RigidBody:applyTorque(Vector(event.torque))

			return
		end
	end

	Network.addEventListener("game_message", onGameMessageReceived)
end

function RemotePlayer:setId( id )
	self._id = id
end

function RemotePlayer:getId()
	return self._id
end

function RemotePlayer:update(dt)
	
	

end

