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
	self._inventory = {}
	self:createNextCheckpoint()
	self:createPickups()
end

function Player:createPickups()
	local tun = App.scene():findObject("tunnel"):getComponent("Tunnel")	
	for index = 1, tun:getNumPickups() do
		local name = "pickup"..index
		local prefab = "pickup"

		local obj = App:scene():createObject(name)
		local data = loadDataFile(prefab, "object")
		obj:load(data)
		obj:start()

		local startPos = tun:getPickupPosition(index)
		obj:transform():setPosition(startPos)
		obj:getComponent("RigidBody"):setPosition(startPos)
	end

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

	--local cammantrans = App.scene():findObject("cameraman"):getComponent("Transform")
	--local thrust = self:transform():localZ() * -1
	local look = self:transform():localZ() * -1
	local up = self:transform():localY()
	local right = self:transform():localX() * -1


	local rotScale = 200
	local linScale = 800
	
	local inTun = true
	--[[
	local tun = App.scene():findObject("tunnel"):getComponent("Tunnel")
	local tunnelDist = tun:getClosestSamplePosition(self.Transform:position())
	if tunnelDist.distance > (tun:tunnelRadius() * 1.1) then 
		inTun = false
	end
	--]]

	if Input.getKey("key_up") then self.RigidBody:applyTorque(right * -rotScale) end
	if Input.getKey("key_down") then self.RigidBody:applyTorque(right * rotScale) end
	if Input.getKey("key_left") then self.RigidBody:applyTorque(up * rotScale) end
	if Input.getKey("key_right") then self.RigidBody:applyTorque(up * -rotScale) end
	if Input.getKey("key_a") then self.RigidBody:applyTorque(look * -rotScale) end
	if Input.getKey("key_d") then self.RigidBody:applyTorque(look * rotScale) end
	if Input.getKey("key_space") and inTun then self.RigidBody:applyCentralForce(look * linScale) end
	if Input.getKeyDown("key_lshift") and #self._inventory > 0 then
		self:owner():getComponent("Ship"):useItem(self._inventory[1][1])
		self._inventory[1][2] = self._inventory[1][2] - 1
		if (self._inventory[1][2] < 1) then self._inventory = {} end
	end

	self:sendPhysics()
end

function Player:onCollision(collision)
    if collision.rigidbody:owner():getComponent("Checkpoint") ~= nil then
		local checkpoint = collision.rigidbody:owner()
    	checkpoint:destroy()
		self:createNextCheckpoint()
	elseif collision.rigidbody:owner():getComponent("Pickup") ~= nil then
		local pickup = collision.rigidbody:owner():getComponent("Pickup"):item()
		if #self._inventory == 0 then
			self._inventory[1] = pickup
		elseif self._inventory[1][1] == pickup[1] then
			self._inventory[1][2] = self._inventory[1][2] + pickup[2]
		end
		collision.rigidbody:owner():destroy()
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