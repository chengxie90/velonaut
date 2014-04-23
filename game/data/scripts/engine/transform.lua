require "engine.component"
local gfxnode = require "engine.graphics.node.c"

Transform = class(Component)

function Transform:_init()
    self._position = Vector(0, 0, 0)
    self._delegates = {}
    self._handle = gfxnode.create()
end

function Transform:load(data)
    if data.position then self:setPosition(data.position) end
    if data.target then self:lookAt(data.target) end
end

function Transform:start()

end

function Transform:update()

end

function Transform:position()
    return self._position
end

function Transform:setPosition(pos)
    self._position = pos
    gfxnode.setPosition(self._handle, pos)
end

function Transform:setOrientation(ori)
	gfxnode.setOrientation(self._handle, ori)
end