local gfxmesh = require "engine.graphics.mesh.c"

Mesh = class()

function Mesh:_init(arg)
    if type(arg) == "string" then
        self._handle = gfxmesh.create(arg)
    elseif type(arg) == "userdata" then
        self._handle = arg
    else
        print(debug.traceback())
        error("bad argument", 2)
    end
end