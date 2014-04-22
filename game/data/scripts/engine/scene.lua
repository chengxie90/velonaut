require "engine.object"
local gfx = require "engine.graphics.c"
local gfxscene = require "engine.graphics.scene.c"

Scene = class()

function Scene:_init(data)
    self._handle = gfxscene.create()
    gfx.setActiveScene(self._handle)
    self._objects = {}
    self._started = false
end

function Scene:load(data)
    for _, objectData in ipairs(data.objects) do
        local name = objectData.name
        local prefab = objectData.prefab
        assert(name and name ~= "")
        local obj = self:createObject(name)
        local data = loadDataFile(objectData.prefab, "object")
        obj:load(data)
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
    assert(self._started == false)
    for k, v in pairs(self._objects) do
        v:start()
    end
    self._started = true
end

function Scene:update(dt)
    for k, v in pairs(self._objects) do
        v:update(dt)
    end
end

function Scene:setMainCamera(cam)
    gfxscene.setMainCamera(cam._handle)
end

function Scene:createObject(name)
    assert(self._objects[name] == nil)
    local obj = Object(name)
    self._objects[name] = obj
    if self._started then obj:start() end
    return obj
end

function Scene:findObject(name)
    return self._objects[name]
end

function Scene:removeObject(name)
    self._objects[name] = nil
end
