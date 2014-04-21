require "engine.bezier"
require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

Tunnel = class(Behavior)

function Tunnel:_init(object)

	-- Params
--[[
	local numCurves = 4
	local tunnelRadius = 6
	local ringsPerCurve = 100
	local samplesPerRing = 30
	local minControlRadius = 100
	local maxControlRadius = 150
	local minAnchorRadius = 200
	local maxAnchorRadius = 250

	--TODO: set seed globally somewhere else
	tunnelSeed = 123456789

	-- Set seed and create table of curves
	math.randomseed(tunnelSeed)
	local curves = {}
	

	-- First curve
	local p0 = Vector(0, 0, 0)
	local p1 = Vector(0, 0, -1)
	local radius = minAnchorRadius + (math.random() * (maxControlRadius - minControlRadius))
	p1 = p1*radius
--]]

    local mb = MeshBuilder("LineStrip") -- or "LineStrip"
    mb:position(Vector(0, 0, 0))
    mb:normal(Vector(0, 0, 1))
    mb:position(Vector(1, 0, 0))
    mb:normal(Vector(0, 0, 1))
    mb:position(Vector(0, 1, 0))
    mb:normal(Vector(0, 0, 1))
    mb:index(0)
    mb:index(1)
    mb:index(2)
    mb:index(0)
    local m = mb:getMesh()



    local mr = object:addComponent("MeshRenderer", nil, false)
    mr:setMesh(m)
end

function Tunnel:start()

end

function Tunnel:update(dt)

end

local function getRandomPoint(min, max)
	local x
end
