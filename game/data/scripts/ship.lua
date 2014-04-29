require "engine.scene"
require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

Ship = class(Behavior)

local counter = 0

function Ship:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.RigidBody:setDamping(0.8, 0.9)
end

function Ship:update(dt)
	
end

function Ship:onCollision(collision)
    if collision.rigidbody:owner():getComponent("Projectile") then
        counter = counter + 1
        local name = "exp".."_".. counter
	print(name)
        local obj = App.scene():createObject(name);
        local data = loadDataFile("explode", "object")
        obj:load(data)
        obj:start()
        obj:transform():setParent(self:transform())
    end
end
