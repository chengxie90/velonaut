require "engine.scene"
require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

local Network = require "engine.network.c"

Player = class(Behavior)

function Player:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.Transform = self:getComponent("Transform")

	-- Create first checkpoint 
	self._nextCheckpoint = 1
	self:createNextCheckpoint();self.RigidBody = self:getComponent("RigidBody")
	self.Transform = self:getComponent("Transform")
end

function Player:createNextCheckpoint()
	local tun = App.scene():findObject("tunnel"):getComponent("Tunnel")	
	if self._nextCheckpoint <= tun:getNumCheckpoints() then
		local name = "checkpoint" .. self._nextCheckpoint
		local prefab = "checkpoint"

		local obj = App.scene():createObject(name)
		local data = loadDataFile(prefab, "object")	
		obj:load(data)
		obj:start()

		local startPos = tun:getCheckpointPosition(self._nextCheckpoint)
		local startTan = tun:getCheckpointTangent(self._nextCheckpoint):getNormalized()

		local angle = Vector(0,0,-1):angleBetween(startTan)
		local axis = Vector(0,0,-1):cross(startTan)
		local startOri = Vector(0, 0, 0, 0)
		startOri:makeQuaternionFromAngleAxis(angle, axis)

		obj:transform():setPosition(startPos)
		obj:transform():setOrientation(startOri)
		obj:getComponent("RigidBody"):setPosition(startPos)		
		obj:getComponent("RigidBody"):setOrientation(startOri)

		self._nextCheckpoint = self._nextCheckpoint + 1
	elseif self._nextCheckpoint == tun:getNumCheckpoints()+1 then
		print("making finishline")
		local name = "finishline"
		local prefab = "finishline"

		local obj = App.scene():createObject(name)
		local data = loadDataFile(prefab, "object")	
		obj:load(data)
		obj:start()

		local startPos = tun:getCheckpointPosition(1)
		local startTan = tun:getCheckpointTangent(1):getNormalized()

		local angle = Vector(0,0,-1):angleBetween(startTan)
		local axis = Vector(0,0,-1):cross(startTan)
		local startOri = Vector(0, 0, 0, 0)
		startOri:makeQuaternionFromAngleAxis(angle, axis)

		obj:transform():setPosition(startPos)
		obj:transform():setOrientation(startOri)
		obj:getComponent("RigidBody"):setPosition(startPos)		
		obj:getComponent("RigidBody"):setOrientation(startOri)
	end
end

function Player:setId( id )
	self._id = id
end

function Player:getId()
	return self._id
end

function Player:update(dt)

	local cammantrans = App.scene():findObject("cameraman"):getComponent("Transform")
	local thrust = self:transform():localZ() * -1
	local look = cammantrans:localZ() * -1
	local up = cammantrans:localY()
	local right = cammantrans:localX()

	local rotScale = 300
	local linScale = 800
	
	local inTun = true
	--[[
	local tun = App.scene():findObject("tunnel"):getComponent("Tunnel")
	local tunnelDist = tun:getClosestSamplePosition(self.Transform:position())
	if tunnelDist.distance > (tun:tunnelRadius() * 1.1) then 
		inTun = false
	end
	--]]

	if Input.getKey("key_up") then self.RigidBody:applyTorque(right * rotScale) end
	if Input.getKey("key_down") then self.RigidBody:applyTorque(right * -rotScale) end
	if Input.getKey("key_left") then self.RigidBody:applyTorque(up * rotScale) end
	if Input.getKey("key_right") then self.RigidBody:applyTorque(up * -rotScale) end
	if Input.getKey("key_a") then self.RigidBody:applyTorque(look * -rotScale) end
	if Input.getKey("key_d") then self.RigidBody:applyTorque(look * rotScale) end
	if Input.getKey("key_space") and inTun then self.RigidBody:applyCentralForce(thrust * linScale) end

	self:sendPhysics()

		
	

end

function Player:onCollision(collision)
    if collision.rigidbody:owner():getComponent("Checkpoint") ~= nil then
		local checkpoint = collision.rigidbody:owner()
    	checkpoint:destroy()
		self:createNextCheckpoint();
	end
end

function Player:sendPhysics()
	local position 		= self.RigidBody:position()
	local orientation 	= self.RigidBody:orientation()
	local linearVelo	= self.RigidBody:linearVelocity()
	local angularVelo	= self.RigidBody:angularVelocity()
	local force			= self.RigidBody:force()
	local torque		= self.RigidBody:torque()

	Network.sendMessage( "{ eventType='playerUpdate'" ..
						 ",playerId='" .. self:getId() .. "'" ..
						 ",position=" .. position:serialize3d() ..
						 ",orientation=" .. orientation:serialize4d() ..
   						 ",linearVelo=" .. linearVelo:serialize3d() ..
						 ",angularVelo=" .. angularVelo:serialize3d() ..
 						 ",force=" .. force:serialize3d() ..
						 ",torque=" .. torque:serialize3d() .. "}")

end