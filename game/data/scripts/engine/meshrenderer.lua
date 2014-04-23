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

function MeshRenderer:start()
    local trans = self:transform()
    assert(trans)
    gfxnode.attachObject(trans._handle, self._handle)
end

function MeshRenderer:setMesh(mesh)
    self._handle = gfxentity.createMesh(mesh._handle)
    self._mesh = mesh
    -- TODO: destroy the former one
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