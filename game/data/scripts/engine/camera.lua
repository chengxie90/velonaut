require "engine.component"
require "engine.transform"
local gfxcam = require "engine.graphics.camera.c"
local gfxnode = require "engine.graphics.node.c"

Camera = class(Component)

function Camera:_init(object)
    self._handle = gfxcam.create(object:name())
end

function Camera:onDestroy()
    
end

function Camera:load(data)
    if data.near then self:setNear(data.near) end
    if data.far then self:setFar(data.far) end
    if data.fov then self:setFOV(data.fov) end
end

function Camera:start()
    local trans = self:transform()
    assert(trans)
    gfxnode.attachObject(trans._handle, self._handle)
end

function Camera:setNear(n)
    gfxcam.setNear(self._handle, n)
end

function Camera:setFar(f)
    gfxcam.setFar(self._handle, f)
end

function Camera:setFOV(fov)
    gfxcam.setFOV(self._handle, fov)
end
