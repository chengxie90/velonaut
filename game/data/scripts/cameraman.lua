require "engine.scene"
require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

local Network = require "engine.network.c"

CameraMan = class(Behavior)

function CameraMan:start()
    self.RigidBody = self:getComponent("RigidBody")
    self.Transform = self:getComponent("Transform")
    self._stopDist = 35
end

function CameraMan:update()

    local linRangeX = 12
    local linRangeY = 12

    local pos = self.RigidBody:position()
    local look = (Vector(0,0,0) - pos)
    local linVel = self.RigidBody:linearVelocity()

    local newX = linVel.x
    local newY = linVel.y

    if Input.getKey("key_up") and not Input.getKey("key_down") then 
        newY = linRangeY - pos.y
    elseif Input.getKey("key_down") and not Input.getKey("key_up") then
        newY = (-1*linRangeY) - pos.y
    else
        newY = -1*pos.y
    end

    if Input.getKey("key_right") and not Input.getKey("key_left") then 
       newX = linRangeX - pos.x
    elseif Input.getKey("key_left") and not Input.getKey("key_right") then
        newX = (-1*linRangeX) - pos.x
    else
        newX = -1*pos.x
    end

    self.RigidBody:setLinearVelocity(Vector(newX, newY, 0))
    self.RigidBody:setOrientation(Vector(0,0,-1):rotationTo(look))

end

function CameraMan:initializePosition()
    self.RigidBody:setPosition((Vector(0,0,1) * self._stopDist) + Vector(0, 10, 0))
    self.Transform:setPosition((Vector(0,0,1) * self._stopDist) + Vector(0, 10, 0))
end

function CameraMan:stopDistance()
    return self._stopDist
end