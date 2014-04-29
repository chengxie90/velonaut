require "engine.utility"
require "engine.component"

local gfxlight = require "engine.graphics.light.c"
local gfxnode = require "engine.graphics.node.c"

Light = class(Component)

function Light:_init()
    self._handle = gfxlight.create()
end

function Light:load(data)
    assert(data.lightType)
    self:setType(data.lightType)
    if data.diffuse then self:setDiffuse(data.diffuse) end
    if data.range then self:setRange(data.range) end
    if data.specular then self:setSpecular(data.specular) end
    if data.position then self:setPosition(data.position) end
    if data.direction then self:setDirection(data.direction) end
    if data.spotlightInnerAngle then self:setSpotlightInnerAngle(data.spotlightInnerAngle) end
    if data.spotlightOuterAngle then self:setSpotlightOuterAngle(data.spotlightOuterAngle) end
    if data.setSpotlightFalloff then self:setSpotlightFalloff(data.setSpotlightFalloff) end
end

function Light:start()
    local trans = self:transform()
    assert(trans)
    gfxnode.attachObject(trans._handle, self._handle)
end

function Light:setType(type)
    gfxlight.setType(self._handle, type)
end

function Light:setDiffuse(diffuse)
    gfxlight.setDiffuse(self._handle, diffuse)
end

function Light:setSpecular(specular)
    gfxlight.setSpecular(self._handle, specular)
end

function Light:setRange(range)
    gfxlight.setRange(self._handle, range)
end

function Light:setPosition(pos)
    gfxlight.setPosition(self._handle, pos)
end

function Light:setDirection(direction)
    gfxlight.setDirection(self._handle, direction)
end

function Light:setSpotlightInnerAngle(inner)
    gfxlight.setSpotlightInnerAngle(self._handle, inner)
end

function Light:setSpotlightOuterAngle(outer)
    gfxlight.setSpotlightOuterAngle(self._handle, outer)
end

function Light:setSpotlightFalloff(falloff)
    gfxlight.setSpotlightFalloff(self._handle, falloff)
end

