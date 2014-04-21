require "engine.object"
local gfx = require "engine.graphics.c"
local gfxscene = require "engine.graphics.scene.c"

Scene = class()

function Scene:_init(data)
    self._handle = gfx.createScene()
    gfx.setActiveScene(self._handle)
    self._objects = {}
end

function Scene:load(data)
    for _, objectData in ipairs(data.objects) do
        local name = objectData.name
        local prefab = objectData.prefab
        assert(name and name ~= "")
        self:createObject(name, prefab, false)
    end

    local obj = self:findObject(data.mainCamera)
    if obj then 
        local cam = obj:getComponent("Camera")
        assert(cam)
        if cam then self:setMainCamera(cam) end
    end

    local bgColor = data.backgroundColor
    if bgColor then gfx.setBackgroundColor(bgColor) end

    local ambient = data.ambient
    if ambient then gfxscene.setAmbientLight(ambient) end
end

function Scene:start()
    for k, v in pairs(self._objects) do
        v:start()
    end
end

function Scene:update(dt)
    for k, v in pairs(self._objects) do
        v:update(dt)
    end
end

function Scene:setMainCamera(cam)
    gfxscene.setMainCamera(cam._handle)
end

function Scene:createObject(name, prefab, start)
    assert(self._objects[name] == nil)
    local obj = nil
    if prefab then
        local data = loadDataFile(prefab, "object")
        obj = Object(name)
        obj:load(data)
    else
        obj = Object(name)
    end
    assert(obj)
    self._objects[name] = obj
    if start == nil or start == true then obj:start() end
    return obj
end

function Scene:findObject(name)
    return self._objects[name]
end

function Scene:removeObject(name)
    self._objects[name] = nil
end
