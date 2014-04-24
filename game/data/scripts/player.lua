require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

Player = class(Behavior)

function Player:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.RigidBody:setDamping(0.8, 0.9)

	self.Transform = self:getComponent("Transform")
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
end