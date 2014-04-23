require "engine.component"
local phyrigidbody = require "engine.physics.rigidbody.c"

RigidBody = class(Component)

function RigidBody:_init()
    self._handle = phyrigidbody.create()
end

function RigidBody:load(data)

end

function RigidBody:start()
	local force = Vector(100,0,100)
	self:applyCentralForce(force)
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

function RigidBody:applyCentralForce(force)
	assert(force)
	assert(force._class == Vector)
	phyrigidbody.applyCentralForce(self._handle, force)
end

function RigidBody:applyForce(force, relativePosition)
	assert(force)
	assert(relativePosition)
	assert(force._class == Vector)
	assert(relativePosition._class == Vector)
	phyrigidbody.applyCentralForce(self._handle, force, relativePosition)
end

function RigidBody:applyTorque(torque)
	assert(torque)
	assert(force._class == Vector)
	phyrigidbody.applyTorque(self._handle, torque)
end

function RigidBody:setLinearVelocity(velocity)
	assert(velocity)
	assert(force._class == Vector)
	phyrigidbody.setLinearVelocity(self._handle, velocity)
end

function RigidBody:setAngularVelocity(velocity)
	assert(velocity)
	assert(force._class == Vector)
	phyrigidbody.setAngularVelocity(self._handle, veloctiy)
end

function RigidBody:setDamping(linearDamping, angularDamping)
	assert(linearDamping)
	assert(angularDamping)
	assert(type(linearDamping) == "number")
	assert(type(angularDamping) == "number")
	phyrigidbody.setDamping(self._handle, linearDamping, angularDamping)
end