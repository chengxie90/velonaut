require "engine.component"
require "engine.material"

local gfxparticle = require "engine.graphics.particle.c"
local gfxnode = require "engine.graphics.node.c"

ParticleRenderer = class(Component)

function ParticleRenderer:_init()
    
end

function ParticleRenderer:load(data)
    assert(data.particleSystem)
    self:setParticleSystem(data.particleSystem)
end

function ParticleRenderer:onDestroy()
    self:_detach()
end

function ParticleRenderer:_attach()
    if self._handle then
        local trans = self:transform()
        assert(trans)
        assert(trans._handle)
        gfxnode.attachObject(trans._handle, self._handle)
    end
end

function ParticleRenderer:_detach()
    if self._handle then
        local trans = self:transform()
        gfxnode.detachObject(trans._handle, self._handle)
        self._handle = nil
    end
end

function ParticleRenderer:setParticleSystem(name)
    self:_detach()
    self._handle = gfxparticle.create(name)
    self:_attach()
end

-- -- this must be called after setMesh, due to Ogre
-- function ParticleRenderer:setMaterial(material)
--     assert(self._handle) 
--     self._material = material
--     gfxparticle.setMaterial(self._handle, material._handle)
-- end

-- function ParticleRenderer:material()
--     return self._material
-- end