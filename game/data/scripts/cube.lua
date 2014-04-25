require "engine.behavior"

Cube = class(Behavior)

function Cube:start()

end

function Cube:update(dt)

end

function Cube:onCollision(collision)
    print("cube : onCollision with" .. " " .. collision.rigidbody:owner():name() .. " at " .. tostring(collision.position))
end