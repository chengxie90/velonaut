require "engine.scene"
require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

Ship = class(Behavior)

function Ship:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.RigidBody:setDamping(0.8, 0.9)
    self._counter = 0
end

function Ship:update(dt)
	
end

function Ship:onCollision(collision)
    if collision.rigidbody:owner():getComponent("projectile") then
        self._counter = self._counter + 1
        local name = "exp".."_".. self._counter
        local obj = App.scene():createObject(name);
        local data = loadDataFile("explode", "object")
        obj:load(data)
        obj:start()
        obj:transform():setParent(self:transform())
    end
end