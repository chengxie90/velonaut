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

	self._boostSpeed = 1100
	self._boostCapacity = 5
	self._boostFuel = 0

	self._activeProjectiles = {}
	self._projectileCounter = 0
	self._projectileRange = 10000
	self._projectileLife = 50
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
		self:useItem(self._inventory[1][1])
		self._inventory[1][2] = self._inventory[1][2] - 1
		if (self._inventory[1][2] < 1) then self._inventory = {} end
	end

	self:updateItems()

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

function Player:updateItems()
	if self._boostFuel > 0 then
		local rigidbody = self:owner():getComponent("RigidBody")
		local transform = self:owner():getComponent("Transform")
		local look = transform:orientation():applyRotationTo(Vector(0,0,-1))
		rigidbody:setLinearVelocity(look:getNormalized() * self._boostSpeed)
		self._boostFuel = self._boostFuel - 1
	end

	local remotePlayers = App.scene():remotePlayers()
	for k, v in pairs(self._activeProjectiles) do
		local target = 0
		local targetDist = math.huge
		local pos = v[1]:transform():position()


        if (self:owner():transform():position() - pos):length() > self._projectileRange then
			self._activeProjectiles[k] = nil
			v[1]:destroy()
		end

		if v[2] > 0 then
			v[2] = v[2] - 1

			for i = 1, #remotePlayers do
				local relativePos = pos - remotePlayers[i]:transform():position()
				if relativePos:length() < targetDist then
					targetDist = relativePos:length()
					target = i
				end
			end

			if target ~= 0 then
				local force = remotePlayers[target]:transform():position() - pos
				v[1]:getComponent("RigidBody"):applyCentralForce(force:getNormalized() * 100)
			end
		end

		self:sendProjectile(v[1])
    end
end

function Player:sendProjectile(projectile)
	local rigidbody = projectile:getComponent("RigidBody") 
	local position 		= rigidbody:position()
	local orientation 	= rigidbody:orientation()
	local linearVelo	= rigidbody:linearVelocity()
	local angularVelo	= rigidbody:angularVelocity()
	local force			= rigidbody:force()
	local torque		= rigidbody:torque()

	Network.sendMessage( "{ eventType='projectileUpdate'" ..
						 ",projectileName='" .. projectile:name() .. "'" ..
						 ",playerId='" .. self:getId() .. "'" ..
						 ",position=" .. position:serialize3d() ..
						 ",orientation=" .. orientation:serialize4d() ..
   						 ",linearVelo=" .. linearVelo:serialize3d() ..
						 ",angularVelo=" .. angularVelo:serialize3d() ..
 						 ",force=" .. force:serialize3d() ..
						 ",torque=" .. torque:serialize3d() .. "}")

end

function Player:useItem(item)

	if item == "boost" then
		print("BOOOOST!")
		self._boostFuel = self._boostCapacity
	end

	if item == "projectile" then

		print("PEW!")
		local rigidbody = self:owner():getComponent("RigidBody")
		local transform = self:owner():getComponent("Transform")
		local look = transform:orientation():applyRotationTo(Vector(0,0,-1)):getNormalized()

		self._projectileCounter = self._projectileCounter + 1
		local name = "projectile_"..self:owner():name().."_".. self._projectileCounter
		local prefab = "projectile"

		local obj = App:scene():createObject(name)
		local data = loadDataFile(prefab, "object")
		obj:load(data)
		obj:start()

		local startPos = rigidbody:position() + (look * 10)
		obj:transform():setPosition(startPos)
		obj:getComponent("RigidBody"):setPosition(startPos)
		obj:getComponent("RigidBody"):setLinearVelocity(look * 1600)

		self._activeProjectiles[name] = {obj, self._projectileLife}
	end

end