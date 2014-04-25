require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

local Network = require "engine.network.c"

Player = class(Behavior)

function Player:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.Transform = self:getComponent("Transform")
end

function Player:setId( id )
	self._id = id
end

function Player:getId()
	return self._id
end

function Player:update(dt)

	local look = self.Transform:localZ() * -1
	local up = self.Transform:localY()
	local right = -self.Transform:localX() * -1


	local rotScale = 300
	local linScale = 800

	if Input.getKey("key_up") then self.RigidBody:applyTorque(right * rotScale) end
	if Input.getKey("key_down") then self.RigidBody:applyTorque(right * -rotScale) end
	if Input.getKey("key_left") then self.RigidBody:applyTorque(up * rotScale) end
	if Input.getKey("key_right") then self.RigidBody:applyTorque(up * -rotScale) end
	if Input.getKey("key_a") then self.RigidBody:applyTorque(look * -rotScale) end
	if Input.getKey("key_d") then self.RigidBody:applyTorque(look * rotScale) end
	if Input.getKey("key_space") then self.RigidBody:applyCentralForce(look * linScale) end

	self:sendPhysics()

end

function Player:sendPhysics()
	local position 		= self.RigidBody:position()
	local orientation 	= self.RigidBody:orientation()
	local linearVelo	= self.RigidBody:linearVelocity()
	local angularVelo	= self.RigidBody:angularVelocity()
	local force			= self.RigidBody:force()
	local torque		= self.RigidBody:torque()

	Network.sendMessage( "{ eventType='playerUpdate'" ..
						 ",playerId='" .. self:getId() .. "'" ..
						 ",position=" .. position:serialize3d() ..
						 ",orientation=" .. orientation:serialize4d() ..
   						 ",linearVelo=" .. linearVelo:serialize3d() ..
						 ",angularVelo=" .. angularVelo:serialize3d() ..
 						 ",force=" .. force:serialize3d() ..
						 ",torque=" .. torque:serialize3d() .. "}")

end
