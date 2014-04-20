require "engine.component"
require "engine.mesh"

local gfxscene = require "engine.graphics.scene.c"
local gfxnode = require "engine.graphics.node.c"

MeshRenderer = class(Component)

function MeshRenderer:_init()
    self._handle = gfxscene.createEntity("cube") -- TODO
end

function MeshRenderer:load(data)
    
end

function MeshRenderer:start()
    local trans = self:transform()
    assert(trans)
    gfxnode.attachObject(trans._handle, self._handle)
end

function MeshRenderer:setMesh()

end

function MeshRenderer:mesh()

end

function MeshRenderer:setMaterial()

end

function MeshRenderer:material()

end