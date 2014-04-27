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
    self._stopDist = 40
end

function CameraMan:update()

    local linRangeX = 12
    local linRangeY = 12
    local rollRange = 10
    local rollScale = 1

    local pos = self.RigidBody:position()
    local ori = self.RigidBody:orientation()
    local look = (Vector(0,0,0) - pos):getNormalized()
    local linVel = self.RigidBody:linearVelocity()
    local angVel = self.RigidBody:angularVelocity()

    local newX = linVel.x
    local newY = linVel.y
    local newR = angVel.z

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

    if Input.getKey("key_d") and not Input.getKey("key_a") then 
        newR = (rollRange - ori.z )/1
    elseif Input.getKey("key_a") and not Input.getKey("key_d") then
        newR = ((-1*rollRange) - ori.z)/1
    else
        newR = (-1*ori.z)/1
    end

    self.RigidBody:setLinearVelocity(Vector(newX, newY, 0))
    self.RigidBody:setOrientation(Vector(0,0,-1):rotationTo(look))
    self.RigidBody:setAngularVelocity(Vector(0, 0, newR))

end

function CameraMan:initializePosition()
    self.RigidBody:setPosition((Vector(0,0,1) * self._stopDist))
    self.Transform:setPosition((Vector(0,0,1) * self._stopDist))
end

function CameraMan:stopDistance()
    return self._stopDist
end