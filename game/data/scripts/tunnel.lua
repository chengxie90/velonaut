require "engine.bezier"
require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

local function getRandomPoint(min, max)
	local x = math.random()
	local y = math.random()
	local z = math.random()
	local dir = Vector(x, y, z)	
	dir = dir:getNormalized()
	local radius = min + (math.random() * (max- min))
	return dir * radius	
end

Tunnel = class(Behavior)

function Tunnel:_init(object)

	-- Params
	local numCurves = 4
	local tunnelRadius = 6
	local ringsPerCurve = 100
	local samplesPerRing = 30
	local minControlRadius = 100
	local maxControlRadius = 150
	local minAnchorRadius = 200
	local maxAnchorRadius = 250

	--TODO: set seed globally somewhere else
	local tunnelSeed = 123456789

	-- Set seed and create table of curves
	math.randomseed(tunnelSeed)
	local curves = {}
	
	-- CREATE THE SPLINE --------------------------------------------------------------------------

	-- First curve
	local p0 = Vector(0, 0, 0)
	local p1 = Vector(0, 0, -1)
	local radius = minAnchorRadius + (math.random() * (maxControlRadius - minControlRadius))
	p1 = p1 * radius
	local rando = getRandomPoint(minControlRadius, maxControlRadius)
print("RANDO")
--print(rando)
	local p2 = p1 + rando
print("P2") --print(p2)
	rando = getRandomPoint(minAnchorRadius, maxAnchorRadius)
print("RANDO")
--print(rando)
	local p3 = p2 + rando
print("P3") --print(p3)
	local first = Bezier(p0, p1, p2, p3)
	curves[1] = first

	-- Middle of spline
	for i = 2, numCurves do
		p0 = curves[i-1].p3
print("P0") --print(p0)
		p1 = (p0 - curves[i-1].p2)
		p1 = p1:getNormalized()
		radius = minAnchorRadius + (math.random() * (maxControlRadius - minControlRadius))
print("TEST")
local test = (p1 * radius)
--print(test)
		p1 = p0 + (p1 * radius)
		p2 = p1 + getRandomPoint(minControlRadius, maxControlRadius)
		p3 = p2 + getRandomPoint(minAnchorRadius, maxAnchorRadius)
		curves[i] = Bezier(p0, p1, p2, p3)
	end

	-- Last curve
	p0 = curves[numCurves].p3
	p1 = (p0 - curves[numCurves].p2):getNormalized()
	radius = minAnchorRadius + (math.random() * (maxControlRadius - minControlRadius))
	p1 = p0 + (p1 * radius)
	p3 = first.p0
	p2 = (p3 - first.p1):getNormalized()
	p2 = p2 * ((p3-p1):length() + (p1-p1):length())
	p2 = p3 + p2
	curves[numCurves] = Bezier(p0, p1, p2, p3)

	-- CREATE THE SAMPLES -------------------------------------------------------------------------

	local normalHelper = Vector(0,1,0)
	local lastNormal = Vector(0,1,0)
	local ringSamples = {}
	local curveSamples = {}

	for curveIndex = 1, numCurves do
		local bezier = curves[curveIndex]
		for t = 0, 1, 1/ringsPerCurve do
			local point = bezier:getPoint(t)
			local tangent = (bezier:getDerivativePoint(t)):getNormalized()
			local normal = (tangent:cross(normalHelper)):getNormalized()
			if normal.x < math.epsilon and normal.x > (-1*math.epsilon) and 
			   normal.y < math.epsilon and normal.y > (-1*math.epsilon) and 
			   normal.z < math.epsilon and normal.z > (-1*math.epsilon) then normal = lastNormal end
			normalHelper = normal:cross(tangent)
			lastNormal = normal
			curveSamples[#curveSamples+1] = point
			for ringSampleIndex = 1, samplesPerRing do
				local theta = ringSampleIndex * 2 * math.pi / samplesPerRing
				local ringSample = point + (normal * (math.cos(theta) * radius)) + 
						((tangent:cross(normal):getNormalized()) * (math.sin(theta) * radius))
				ringSamples[#ringSamples+1] = ringSample
			end
		end
	end

	print("made samples: " .. #ringSamples .. " ring samples, " .. #curveSamples .. " curve samples")

	-- CREATE THE MESH ----------------------------------------------------------------------------

	local numRings = numCurves * ringsPerCurve
	local mb = MeshBuilder("LineStrip")
	print("noRings: " ..numRings)
	for ringIndex = 0, numRings-1 do
		for ringSampleIndex = 0, samplesPerRing-1 do
			local ind = ((ringIndex * samplesPerRing) + ringSampleIndex) + 1 
			print("ring iterator: " .. ringIndex .. "sample iterator: " .. ringSampleIndex .. " index: " ..ind)
			local ringSample = ringSamples [ ((ringIndex * samplesPerRing) + ringSampleIndex) + 1 ]
			local sampleNormal = ringSample-curveSamples[ringIndex + 1]			
			mb:position(ringSample)
			mb:normal(sampleNormal)			
		end
	end
	for ringIndex =  0, numRings-1 do
		local currRingSampleBase = ringIndex * samplesPerRing
		local nextRingSampleBase = (currRingSampleBase + samplesPerRing) % #ringSamples
		for ringSampleIndex = 0, samplesPerRing-1 do
			mb:index(nextRingSampleBase+ringSampleIndex)		
			mb:index(currRingSampleBase+ringSampleIndex)
		end
	end

	local m = mb:getMesh()
	local mr = object:addComponent("MeshRenderer", nil, false)
	mr:setMesh(m)

--[[
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
--]]
end

function Tunnel:start()

end

function Tunnel:update(dt)

end


