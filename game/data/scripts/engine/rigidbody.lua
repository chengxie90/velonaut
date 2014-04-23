require "engine.component"
local phyrigidbody = require "engine.physics.rigidbody.c"

RigidBody = class(Component)

function RigidBody:_init()

end

function RigidBody:load(data)
    if data.shape == "box" then
        print(data.shape)
        self._handle = phyrigidbody.create(data.mass, data.shape, data.boxHalfExtents)
    end
end

function RigidBody:start()
    self:setPosition(self:transform():position())
end

function RigidBody:update()
    local trans = self:transform()
    assert(trans)
    trans:setPosition(self:position())
end

function RigidBody:position()
    return phyrigidbody.position(self._handle)
end

function RigidBody:setPosition(pos)
    phyrigidbody.setPosition(self._handle, pos)
end

function RigidBody:orientation()

end