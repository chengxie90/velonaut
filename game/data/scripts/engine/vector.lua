require "engine.class"

Vector = class()

function Vector:_init(...)
    local args = {...}
	if type(args[1]) == "table" then
		for i = 1, #args[1] do
		    self[i] = args[1][i]
		end
		return
	end

	
    for i = 1, #args do
        self[i] = args[i]
    end
end

function Vector.__index(table, key)
	if key == "x" then return table[1] end
	if key == "y" then return table[2] end
	if key == "z" then return table[3] end
	if key == "w" then return table[4] end
	return Vector[key]
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

function Vector.cross(v1, v2)
	assert(#v1 == #v2)
	assert(#v1 == 3)
	local ret = Vector()
	ret[1] = v1[2] * v2[3] - v1[3] * v2[2]
	ret[2] = v1[3] * v2[1] - v1[1] * v2[3]
	ret[3] = v1[1] * v2[2] - v1[2] * v2[1]
	return ret
end

function Vector:length()
	local lenSq = 0
	for i = 1, #self do
		lenSq = lenSq + (self[i] * self[i])
	end
	return math.sqrt(lenSq)
end

function Vector:normalize()
	local len = self:length()
	for i = 1, #self do
		self[i] = self[i] / len
	end
end

function Vector:getNormalized()
	local ret = Vector()
	for i = 1, #self do
		ret[i] = self[i]
	end
	ret:normalize()
	return ret
end

function Vector:serialize3d()
	return "{" .. self[1] .. "," .. self[2] .. "," .. self[3] .. "}";
end

function Vector:serialize4d()
	return "{" .. self[1] .. "," .. self[2] .. "," .. self[3] .. "," .. self[4] .."}";
end

local function test()
    local v1 = Vector(1, 2, 3)
    local v2 = Vector(2, 3, 4)

    print(v1 + v2)
    print(v1 - v2)
    print(v1 * v2)
    print(2 * v2)
    print(v1 * 3)
    print(-v1)
    print(v1 / v2)
    print(v1 / 2)
    print(2 / v1)
	print(v1:cross(v2))
	print(v1:length())
	print(v1:getNormalized():length())
	print(v2)
	print(v2:getNormalized())
	v2:normalize()
	print(v2)
	
end

--test()



