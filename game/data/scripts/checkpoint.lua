require "engine.bezier"
require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

Checkpoint = class(Behavior)

function Checkpoint:start()

end

function Checkpoint:update()

end

function Checkpoint:onCollision(collision)
    --print("cube : onCollision with" .. " " .. collision.rigidbody:owner():name() .. " at " .. tostring(collision.position))
end
