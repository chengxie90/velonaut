local gfxscene = require "engine.graphics.scene.c"
local gfxnode = require "engine.graphics.node.c"
require "engine.component"

Transform = class(Component)

function Transform:_init()
    self._handle = gfxscene.createNode()
end

function Transform:load(data)
    if data.position then self:setPosition(data.position) end
    if data.target then self:lookAt(data.target) end
end

function Transform:start()

end

function Transform:setPosition(pos)
    gfxnode.setPosition(self._handle, pos)
end

function Transform:position()
    return gfxnode.position(self._handle)
end

function Transform:lookAt(target)
    gfxnode.lookAt(self._handle, target)
end
