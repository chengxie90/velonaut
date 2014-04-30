require "engine.bezier"
require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

Pickup = class(Behavior)

function Pickup:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.Transform = self:getComponent("Transform")
	self.RigidBody:setAngularVelocity(Vector(0,1,0))
	self._items = {{[1]="projectile", [2]=3}, {[1]="boost", [2]=3}}
end

function Pickup:update()

end

function Pickup:item()
	math.randomseed(os.time())
	local index = math.ceil(math.random(#self._items))
	local item = self._items[index]
	return self._items[math.ceil(math.random(#self._items))]
end
