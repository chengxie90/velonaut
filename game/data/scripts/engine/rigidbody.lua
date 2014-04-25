require "engine.component"
local phyrigidbody = require "engine.physics.rigidbody.c"

RigidBody = class(Component)

local handlemap = {}

function RigidBody._onGlobalCollision(handle1, handle2)
    local obj1 = handlemap[handle1]
    local obj2 = handlemap[handle2]
    obj1:onCollision({rigidbody = obj2})
    obj2:onCollision({rigidbody = obj1})
end

function RigidBody:_init()

end

function RigidBody:load(data)
    if data.shape == "box" then
        self._handle = phyrigidbody.create(data.mass, data.shape, data.boxHalfExtents)
        handlemap[self._handle] = self	
    end
end

function RigidBody:start()
    self:setPosition(self:transform():position())
end

function RigidBody:update()
    local trans = self:transform()
    assert(trans)
    trans:setPosition(self:position())
    trans:setOrientation(self:orientation())
end

function RigidBody:onCollision(collision)
    local behaviors = self:behaviors()
    for _, c in ipairs(behaviors) do
        c:onCollision(collision)
    end
end

function RigidBody:position()
    return phyrigidbody.position(self._handle)
end

function RigidBody:orientation()
	return phyrigidbody.orientation(self._handle)
end

function RigidBody:linearVelocity()
	return phyrigidbody.linearVelocity(self._handle)
end

function RigidBody:angularVelocity()
	return phyrigidbody.angularVelocity(self._handle)
end

function RigidBody:force()
	return phyrigidbody.force(self._handle)
end

function RigidBody:torque()
	return phyrigidbody.torque(self._handle)
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

function RigidBody:clearForces()
	phyrigidbody.clearForces(self._handle)
end

function RigidBody:applyTorque(torque)
	assert(torque)
	assert(type(torque) == "table")
	assert(torque._class == Vector)
	phyrigidbody.applyTorque(self._handle, torque)
end

function RigidBody:setPosition(pos)
	local trans = self:transform()
    assert(trans)
    trans:setPosition(self:position())
    phyrigidbody.setPosition(self._handle, pos)	
end

function RigidBody:setOrientation(ori)
	local trans = self:transform()
    assert(trans)
    trans:setOrientation(self:orientation())
    phyrigidbody.setOrientation(self._handle, ori)	
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


