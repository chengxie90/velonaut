local gfxMeshBuilder = require "engine.graphics.meshbuilder.c"
require "engine.mesh"

MeshBuilder = class()

function MeshBuilder:_init(primitiveType)
    self._handle = gfxMeshBuilder.create(primitiveType)
end

function MeshBuilder:position(pos)
    gfxMeshBuilder.position(self._handle, pos)
end

function MeshBuilder:normal(n)
    gfxMeshBuilder.normal(self._handle, n)
end

function MeshBuilder:index(index)
    gfxMeshBuilder.index(self._handle, index)
end

function MeshBuilder:getMesh()
    local handle = gfxMeshBuilder.getMesh(self._handle)
    local mesh = Mesh(handle)
    return mesh
end