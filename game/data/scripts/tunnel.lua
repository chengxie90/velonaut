require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

Tunnel = class(Behavior)

function Tunnel:_init(object)
    local mb = MeshBuilder("LineStrip") -- or "LineStrip"
    
    mb:position(Vector(0, 0, 0))
    mb:normal(Vector(0, 0, 1))
    mb:position(Vector(1, 0, 0))
    mb:normal(Vector(0, 0, 1))
    mb:position(Vector(0, 1, 0))
    mb:normal(Vector(0, 0, 1))
    mb:index(0)
    mb:index(1)
    mb:index(2)
    mb:index(0)
    local m = mb:getMesh()

    local mr = object:addComponent("MeshRenderer")
    mr:setMesh(m)
end

function Tunnel:start()

end

function Tunnel:update(dt)

end