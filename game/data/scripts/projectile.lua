require "engine.bezier"
require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

Projectile = class(Behavior)

function Projectile:start()
	
end

function Projectile:setSender(name)
	self._sender = name
end

function Projectile:update()
	
end

function Projectile:onCollision(collision)
	if collision.rigidbody:owner():getComponent("RemotePlayer") ~= nil and self._sender == App.scene():player() then
		App.scene():player():destroyProjectile(self:owner():name())
		self:destroy()
	end
end

