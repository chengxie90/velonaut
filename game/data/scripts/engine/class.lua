function class(parent)
    if parent then assert(type(parent) == "table") end
    local c = {}
    c._base = parent
    c.__index = c

    function c:is_a(cls)
        assert(self._class) -- not called on a class
        assert(cls._class == nil)
        local t = getmetatable(self)
        while t do
            if t == cls then
                return true
            end
            t = t._base
        end
        return false
    end

    local mt = {}
    mt.__index = parent
    function mt:__call(...)
        local obj = {}
        obj._class = c
        setmetatable(obj, c)
        if obj._init then obj:_init(...) end
        return obj
    end
    setmetatable(c, mt)

    return c
end

