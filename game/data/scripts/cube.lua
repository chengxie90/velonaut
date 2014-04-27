require "engine.behavior"

Cube = class(Behavior)

function Cube:start()

end

function Cube:update(dt)

end

function Cube:onCollision(collision)
    local obj = collision.rigidbody:owner()
    obj:destroy()
end