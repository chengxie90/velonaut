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
    self._moveScale = 10
    self._stopDist = 40
    self._targetDist = 40
end

function CameraMan:update()
    local player = App.scene():player()
    local toPlayer = player:getComponent("RigidBody"):position() - self.Transform:position()
    local dist = toPlayer:length()
    local look = toPlayer:getNormalized()

    -- look at player 
    local angle = Vector(0,0,-1):angleBetween(look)
    local axis = Vector(0,0,-1):cross(look)
    local ori = Vector(0, 0, 0, 0) ori:makeQuaternionFromAngleAxis(angle, axis)
    self.RigidBody:setOrientation(ori)

    -- change velocity
    local interpDist = dist - self._stopDist
    if (interpDist > 0) then
        self.RigidBody:setLinearVelocity(look * interpDist * self._moveScale)
    end

end

function CameraMan:initializePosition()
    self.RigidBody:setPosition(App.scene():player():getComponent("Transform"):position() + (Vector(0,0,1) * self._stopDist))
    self.Transform:setPosition(App.scene():player():getComponent("Transform"):position() + (Vector(0,0,1) * self._stopDist))
end

function CameraMan:stopDistance()
    return self._stopDist
end