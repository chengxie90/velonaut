local gfxmaterial = require "engine.graphics.material.c"

Material = class()

function Material:_init(name)
    self._handle = gfxmaterial.create(name)
end