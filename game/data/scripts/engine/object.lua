require "engine.utility"
require "engine.component"
require "engine.transform"
require "engine.camera"
require "engine.light"
require "engine.renderer"
require "engine.meshrenderer"
require "engine.behavior"
require "engine.rigidbody"

Object = class()

function Object:_init(name)
    self._components = {}
    self._started = false
    self.name = name
end

function Object:load(data)
    for _, componentData in ipairs(data.components) do
        local typename = componentData.type
        local comp = self:addComponent(typename)
        comp:load(componentData)
    end
end

function Object:start()
    assert(self._started == false)
    for k, v in pairs(self._components) do
        v:start()
    end
    self._started = true
end

function Object:update(dt)
    for k, v in pairs(self._components) do
        v:update(dt)
    end
end

function Object:addComponent(typename)
    assert(self._components[typename] == nil, "Duplicated components")
    local classobject = rawget(_G, typename)
    if classobject == nil then
        require(typename:lower())
        classobject = _G[typename]
        assert(classobject)
    end
    local comp = classobject(self)
    comp:setOwner(self)
    self._components[typename] = comp
    if self._started then comp:start() end
    return comp
end

function Object:getComponent(typename)
    return self._components[typename];
end

function Object:removeComponent(typename)
    self._components[typename]._object = nil
    self._components[typename] = nil
end

function Object:transform()
    return self:getComponent("Transform")
end

