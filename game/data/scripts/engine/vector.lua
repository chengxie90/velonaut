local m = require "engine.lib.matrix"

Vector = class()

function Vector:_init(...)
    local args = {...}
    for i = 1, #args do
        self[i] = {args[i]}
    end
end

function Vector:__index(key)
	if key == "x" then 
		return self[1][1]
	elseif key == "y" then 
		return self[2][1]
	elseif key == "z" then 
		return self[3][1]
	elseif key == "w" then 
			return self[4][1]
	end
end

function Vector:__mul(var)
	if type(var) == "number" then
		self:mulnum(var)
	elseif type(var) == "table" then
		self:scalar(var)
	end
end

Vector.__tostring = m.tostring

Vector.__add = m.add

Vector.__sub = m.sub
