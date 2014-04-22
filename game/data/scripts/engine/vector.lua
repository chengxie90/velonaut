Vector = class()

function Vector:_init(...)
    local args = {...}
    for i = 1, #args do
        self[i] = args[i]
    end
end

function Vector.__add(v1, v2)
    assert(#v1 == #v2)
    local ret = Vector()
    for i = 1, #v1 do
        ret[i] = v1[i] + v2[i]
    end
    return ret
end

function Vector.__sub(v1, v2)
    return v1 + -v2
end

function Vector.__tostring(v)
    local ret = ""
    for i = 1, #v do
        ret = ret .. tostring(v[i]) .. " "
    end
    return ret
end

function Vector.__unm(v)
    local ret = Vector()
    for i = 1, #v do
        ret[i] = -v[i]
    end
    return ret
end

function Vector.__mul(v1, v2)
    local ret = Vector()
    if type(v1) == "number" then
        assert(type(v2) == "table")
        for i = 1, #v2 do
            ret[i] = v1 * v2[i]
        end
    elseif type(v2) == "number" then
        assert(type(v1) == "table")
        for i = 1, #v1 do
            ret[i] = v1[i] * v2
        end
    else
        assert(#v1 == #v2)
        for i = 1, #v1 do
            ret[i] = v1[i] * v2[i]
        end
    end
    return ret
end

function Vector.__div(v1, v2)
    local ret = Vector()
    if type(v1) == "number" then
        assert(type(v2) == "table")
        for i = 1, #v2 do
            ret[i] = v1 / v2[i]
        end
    elseif type(v2) == "number" then
        assert(type(v1) == "table")
        for i = 1, #v1 do
            ret[i] = v1[i] / v2
        end
    else
        assert(#v1 == #v2)
        for i = 1, #v1 do
            ret[i] = v1[i] / v2[i]
        end
    end
    return ret
end

local function test()
    v1 = Vector(1, 2, 3)
    v2 = Vector(2, 3, 4)

    print(v1 + v2)
    print(v1 - v2)
    print(v1 * v2)
    print(2 * v2)
    print(v1 * 3)
    print(-v1)
    print(v1 / v2)
    print(v1 / 2)
    print(2 / v1)
end



