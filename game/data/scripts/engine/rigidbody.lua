
require "engine.component"
local phyrigidbody = require "engine.physics.rigidbody.c"

RigidBody = class(Component)

local handlemap = {}

function RigidBody._onGlobalCollision(handle1, handle2, pos)
    local obj1 = handlemap[handle1]
    local obj2 = handlemap[handle2]
    obj1:onCollision({rigidbody = obj2, position = pos})
    obj2:onCollision({rigidbody = obj1, position = pos})
end

function RigidBody:_init()

end

function RigidBody:load(data)
   -- print(table.unpack(data))
    self:rebuild(data.mass, data.shape, data)

    assert(self._handle)
    if data.trigger then self:setTrigger(data.trigger) end
end

function RigidBody:rebuild(mass, shape, params)
    self:_destroy()
    if shape == "box" then
        local boxHalfExtents = params.boxHalfExtents
        self._handle = phyrigidbody.create(mass, shape, boxHalfExtents)     
    elseif shape == "sphere" then
        local radius = params.radius
        self._handle = phyrigidbody.create(mass, shape, radius)
    end
    assert(self._handle)
    handlemap[self._handle] = self
end

function RigidBody:_destroy()
    if self._handle then
        handlemap[self._handle] = nil
        phyrigidbody.destroy(self._handle)
        self._handle = nil
    end
end

function RigidBody:onDestroy()
    self:_destroy()
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

function RigidBody:setTrigger(trigger)
    phyrigidbody.setTrigger(self._handle, trigger)
end

function RigidBody:position()
   -- print(debug.traceback())
    return phyrigidbody.position(self._handle)
end

function RigidBody:orientation()
   -- print(debug.traceback())
	return phyrigidbody.orientation(self._handle)
end

function RigidBody:linearVelocity()
  --  print(debug.traceback())
	return phyrigidbody.linearVelocity(self._handle)
end

function RigidBody:angularVelocity()
   -- print(debug.traceback())
	return phyrigidbody.angularVelocity(self._handle)
end

function RigidBody:force()
   -- print(debug.traceback())
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


