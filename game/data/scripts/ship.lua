require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

Ship = class(Behavior)

function Ship:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.RigidBody:setDamping(0.8, 0.9)
end

function Ship:update(dt)

end

function Ship:useItem(item)
	--[[
	if (item == "boost") then
		local vel = self:owner():getComponent("RigidBody"):linearVelocity()
		--]]

end