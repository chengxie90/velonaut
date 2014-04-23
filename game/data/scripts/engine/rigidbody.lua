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

function RigidBody:applyCentralForce(force)
	assert(force)
	assert(type(force) == "table")
	assert(force._class == Vector)
	phyrigidbody.applyCentralForce(self._handle, force)
end

function RigidBody:applyForce(force, relativePosition)
	assert(force)
	assert(type(force) == "table")
	assert(force._class == Vector)
	assert(relativePosition)
	assert(type(relativePosition) == "table")
	assert(relativePosition._class == Vector)
	phyrigidbody.applyCentralForce(self._handle, force, relativePosition)
end

function RigidBody:applyTorque(torque)
	assert(torque)
	assert(type(torque) == "table")
	assert(torque._class == Vector)
	phyrigidbody.applyTorque(self._handle, torque)
end

function RigidBody:setLinearVelocity(velocity)
	assert(velocity)
	assert(type(velocity) == "table")
	assert(velocity._class == Vector)
	phyrigidbody.setLinearVelocity(self._handle, velocity)
end

function RigidBody:setAngularVelocity(velocity)
	assert(velocity)
	assert(type(velocity) == "table")
	assert(velocity._class == Vector)
	phyrigidbody.setAngularVelocity(self._handle, velocity)
end

function RigidBody:setDamping(linearDamping, angularDamping)
	assert(linearDamping)
	assert(angularDamping)
	assert(type(linearDamping) == "number")
	assert(type(angularDamping) == "number")
	phyrigidbody.setDamping(self._handle, linearDamping, angularDamping)
end