require "engine.vector"
require "engine.behavior"
require "engine.input"
require "engine.rigidbody"
require "engine.camera"

local Network = require "engine.network.c"

RemotePlayer = class(Behavior)

function RemotePlayer:start()
	self.RigidBody = self:getComponent("RigidBody")
	self.Transform = self:getComponent("Transform")
	self._activeProjectiles = {}
	self._projectileRange = 20000

	self._inTun = true
	
	local function onGameMessageReceived(event)

		if event.eventType == "playerUpdate" and event.playerId == self:getId() then		
			self.RigidBody:clearForces()
			
			local curPosition = (Vector(event.position) - self.Transform:position()) / 0.03;

--			self.RigidBody:setPosition(Vector(event.position))
			self.RigidBody:setOrientation(Vector(event.orientation))
			self.RigidBody:setLinearVelocity(curPosition)
			self.RigidBody:setAngularVelocity(Vector(event.angularVelo))
			self.RigidBody:applyCentralForce(Vector(event.force))
			self.RigidBody:applyTorque(Vector(event.torque))
			self._inTun = event.inTun

			return
		end

		if event.eventType == "projectileUpdate" and event.playerId == self:getId() then
			if self._activeProjectiles[event.projectileName] == nil and App.scene():findObject(event.projectileName) == nil then
				local prefab = "projectile"

				local obj = App:scene():createObject(event.projectileName)
				local data = loadDataFile(prefab, "object")
				obj:load(data)
				obj:start()

				local rigidbody = obj:getComponent("RigidBody")
				local transform = obj:transform()
				transform:setPosition(Vector(event.position))
				rigidbody:setPosition(Vector(event.position))
				rigidbody:setLinearVelocity(Vector(event.linearVelo))

				self._activeProjectiles[event.projectileName] = obj
			elseif self._activeProjectiles[event.projectileName] == nil and App.scene():findObject(event.projectileName) ~= nil then
				return
			else
				local obj = self._activeProjectiles[event.projectileName]
				local rigidbody = obj:getComponent("RigidBody")
				local transform = obj:transform()
				rigidbody:setLinearVelocity(Vector(event.linearVelo))
			end
		end
	end

	Network.addEventListener("game_message", onGameMessageReceived)
end

function RemotePlayer:setId( id )
	self._id = id
end

function RemotePlayer:getId()
	return self._id
end

function RemotePlayer:update(dt)

	local ff = App.scene():findObject(self._forcefieldName)
	local mr = ff:getComponent("MeshRenderer")
	if self._inTun then
		mr:setMaterial(Material("forcefieldblue"))
	else
		mr:setMaterial(Material("forcefieldred"))
	end


	for k, v in pairs(self._activeProjectiles) do
		if (v:transform():position() - self:owner():transform():position()):length() > self._projectileRange then
			self._activeProjectiles[k] = nil
			v:destroy()
		end
    end
end

function RemotePlayer:destroyProjectile(name)
	local obj = App.scene():findObject(name)
	if self._activeProjectiles[name] ~= nil then
		self._activeProjectiles[name] = nil
		obj:destroy()
	end
end

function RemotePlayer:setForcefieldName(name)
	self._forcefieldName = name
end
