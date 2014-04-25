Component = class()

function Component:onDestroy()

end

function Component:start()

end

function Component:update(dt)

end

function Component:load()

end

function Component:setOwner(object)
    self._owner = object
end

function Component:owner()
    return self._owner
end

function Component:getComponent(cls)
    return self:owner():getComponent(cls)
end

function Component:transform()
    return self:owner():transform()
end

function Component:behaviors()
    return self:owner():behaviors()
end

