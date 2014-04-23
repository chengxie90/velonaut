require "engine.component"
local phyrigidbody = require "engine.physics.rigidbody.c"

RigidBody = class(Component)

function RigidBody:_init()
    self._handle = phyrigidbody.create()
end

function RigidBody:load(data)

end

function RigidBody:start()

end

function RigidBody:update()
    local trans = self:transform()
    assert(trans)
    trans:setPosition(self:position())
end

function RigidBody:position()
    return phyrigidbody.position(self._handle)
end

function RigidBody:orientation()

end