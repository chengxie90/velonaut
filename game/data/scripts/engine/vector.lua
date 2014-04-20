local m = require "engine.lib.matrix"

Vector = class()

function Vector:_init(...)
    local args = {...}
    for i = 1, #args do
        self[i] = {args[i]}
    end
end

Vector.__tostring = m.tostring

Vector.__add = m.add

Vector.__sub = m.sub