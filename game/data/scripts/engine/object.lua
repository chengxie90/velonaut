require "engine.utility"
require "engine.component"
require "engine.transform"
require "engine.camera"
require "engine.light"
require "engine.renderer"
require "engine.meshrenderer"
require "engine.behavior"

Object = class()

function Object:_init(name)
    self._components = {}
    self.name = name
end

function Object:load(data)
    for _, componentData in ipairs(data.components) do
        local typename = componentData.type
        self:addComponent(typename, componentData, false)
    end
end

function Object:start()
    for k, v in pairs(self._components) do
        v:start()
    end
end

function Object:update(dt)
    for k, v in pairs(self._components) do
        v:update(dt)
    end
end

function Object:addComponent(typename, data, start)
    assert(self._components[typename] == nil, "Duplicated components")
    local classobject = rawget(_G, typename)
    if classobject == nil then
        require(typename)
        classobject = _G[typename]
        assert(classobject)
    end
    local comp = classobject(self)
    comp:setOwner(self)
    if data then comp:load(data) else comp:load() end
    self._components[typename] = comp
    if start == nil or start == true then comp:start() end
    return comp
end

function Object:getComponent(cls)
    return self._components[cls];
end

function Object:removeComponent(cls)
    self._components[cls]._object = nil
    self._components[cls] = nil
end

function Object:transform()
    return self:getComponent("Transform")
end

