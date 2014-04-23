require "engine.component"
local gfxnode = require "engine.graphics.node.c"

Transform = class(Component)

function Transform:_init()
    self._position = Vector(0, 0, 0)
    self._orientation = Vector(0, 0, 0, 1)
    self._scale = Vector(1, 1, 1)
    self._delegates = {}
    self._handle = gfxnode.create()
end

function Transform:load(data)
    if data.position then self:setPosition(data.position) end
    if data.orientation then self:setOrientation(data.orientation) end
    if data.scale then self:setScale(data.scale) end
    if data.target then self:lookAt(data.target) end
end

function Transform:start()

end

function Transform:update()

end

function Transform:position()
    return self._position
end

function Transform:orientation()
    return self._orientation
end

function Transform:scale()
    return self._scale
end

function Transform:setPosition(pos)
    self._position = pos
    gfxnode.setPosition(self._handle, pos)
end

function Transform:setOrientation(ori)
    self._orientation = ori
	gfxnode.setOrientation(self._handle, ori)
end

function Transform:setScale(sca)
    self._scale = sca
    gfxnode.setScale(self._handle, sca)
end

function Transform:localX()
	return gfxnode.localX(self._handle)
end

function Transform:localY()
	return gfxnode.localY(self._handle)
end

function Transform:localZ()
	return gfxnode.localZ(self._handle)
end