require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

Ship = class(Behavior)

function Ship:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.RigidBody:setDamping(0.8, 0.9)

	self._boostSpeed = 1100
	self._boostCapacity = 5
	self._boostFuel = 0
end

function Ship:update(dt)
	if self._boostFuel > 0 then
		local rigidbody = self:owner():getComponent("RigidBody")
		local transform = self:owner():getComponent("Transform")
		local look = transform:orientation():applyRotationTo(Vector(0,0,-1))
		rigidbody:setLinearVelocity(look:getNormalized() * self._boostSpeed)
		self._boostFuel = self._boostFuel - 1
	end

end

function Ship:useItem(item)

	if (item == "boost") then
		print("BOOOOST!")
		self._boostFuel = self._boostCapacity
	end

end