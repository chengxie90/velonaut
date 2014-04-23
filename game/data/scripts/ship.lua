require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

Ship = class(Behavior)

function Ship:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.RigidBody:setDamping(0.5, 0)
end

function Ship:update(dt)
	if Input.getKey("key_up") then self.RigidBody:applyCentralForce(Vector(0,100,0)) end
	if Input.getKey("key_down") then self.RigidBody:applyCentralForce(Vector(0,-100,0)) end
	if Input.getKey("key_left") then self.RigidBody:applyCentralForce(Vector(-100,0,0)) end
	if Input.getKey("key_right") then self.RigidBody:applyCentralForce(Vector(100, 0,0)) end
	if Input.getKey("key_a") then self.RigidBody:setAngularVelocity(Vector(0,0,10)) end
	if Input.getKey("key_d") then self.RigidBody:setAngularVelocity(Vector(0,0,-10)) end
end