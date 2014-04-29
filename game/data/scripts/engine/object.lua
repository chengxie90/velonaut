require "engine.utility"
require "engine.component"
require "engine.transform"
require "engine.camera"
require "engine.light"
require "engine.meshrenderer"
require "engine.behavior"
require "engine.rigidbody"
require "engine.particlerenderer"

Object = class()

function Object:_init(name)
    self._components = {}
    self._started = false
    self._name = name
end

function Object:onDestroy()
    for k, v in pairs(self._components) do
        v:onDestroy()
    end
end

function Object:destroy()
    App.scene():destroyObject(self:name())
end

function Object:name()
    return self._name
end

function Object:load(data)
    for _, componentData in ipairs(data.components) do
        local typename = componentData.type
        local comp = self:addComponent(typename)
        comp:load(componentData)
    end

    if data.children then
        for _, objData in ipairs(data.children) do
            local child = self:_loadObjectData(objData)
            child:transform():setParent(obj:transform())
        end
    end
end

function Object:start()
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
    return comp
end

function Object:behaviors()
    local ret = {}
    for _, comp in pairs(self._components) do
        if comp:is_a(Behavior) then
            table.insert(ret, comp)
        end
    end
    return ret
end

function Object:getComponent(typename)
    return self._components[typename];
end

function Object:removeComponent()
    local comp = self:getComponent(typename)
    assert(comp)
    comp:onDestroy()
    comp._object = nil
    self._components[typename] = nil
end

function Object:transform()
    return self:getComponent("Transform")
end

