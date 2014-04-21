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