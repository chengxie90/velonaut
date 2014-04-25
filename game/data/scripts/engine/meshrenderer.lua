require "engine.component"
require "engine.mesh"
require "engine.material"

local gfxentity = require "engine.graphics.entity.c"
local gfxnode = require "engine.graphics.node.c"

MeshRenderer = class(Component)

function MeshRenderer:_init()
    
end

function MeshRenderer:load(data)
    assert(data.mesh)
    local mesh = Mesh(data.mesh)
    self:setMesh(mesh)
    
    if data.material then
        local mat = Material(data.material)
        self:setMaterial(mat)
    end
end

function MeshRenderer:onDestroy()
    self:_detach()
end

function MeshRenderer:_attach()
    if self._handle then
        local trans = self:transform()
        assert(trans)
        assert(trans._handle)
        gfxnode.attachObject(trans._handle, self._handle)
    end
end

function MeshRenderer:_detach()
    if self._handle then
        local trans = self:transform()
        gfxnode.detachObject(trans._handle, self._handle)
        self._handle = nil
    end
end

function MeshRenderer:start()

end

function MeshRenderer:setMesh(mesh)
    self:_detach()
    self._handle = gfxentity.createMesh(mesh._handle)
    self._mesh = mesh
    self:_attach()
end

function MeshRenderer:mesh()
    return self._mesh
end

-- this must be called after setMesh, due to Ogre
function MeshRenderer:setMaterial(material)
    assert(self._handle) 
    self._material = material
    gfxentity.setMaterial(self._handle, material._handle)
end

function MeshRenderer:material()
    return self._material
end