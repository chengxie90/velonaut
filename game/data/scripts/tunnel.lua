require "engine.bezier"
require "engine.behavior"
require "engine.mesh"
require "engine.meshbuilder"
require "engine.meshrenderer"

local function getRandomPoint(min, max)
	local x = math.random()-0.5
	local y = math.random()-0.5
	local z = math.random()-0.5
	local dir = Vector(x, y, z)	
	dir = dir:getNormalized()
	local radius = min + (math.random() * (max- min))
	return dir * radius	
end

local function getRandomRadius(min, max)
	return min + (math.random() * (max - min))
end

Tunnel = class(Behavior)

function Tunnel:_init(object)

	-- Members
	self._tunnel = {} -- Each row of table is a table with 3 vectors: position, tangent, normal
	self._checkpoints = {} -- For convenience. Each row of table is a table with 3 vectors: position, tangent, normal
	self._pickups = {}
	self._numCurves = 5
	self._tunnelRadius = 100
	self._ringsPerCurve = 110
	self._samplesPerRing = 30
	self._samplesPerCheckpoint = 14
	self._samplesPerPickup = 60
	self._tunnelSeed = 7243897024 --TODO: set seed globally somewhere else

end

function Tunnel:start()

end

function Tunnel:update(dt)

end

function Tunnel:getClosestSamplePosition(position)
	local minDist = math.huge
	local minInd = 0	
	for i = 1, #self._tunnel do
		local newDist = (self._tunnel[i][1] - position):length()
		if (newDist < minDist) then 
			minDist = newDist
			minInd = i
		end
	end
	return {["position"] = self._tunnel[minInd][1], ["distance"] = minDist}
end

function Tunnel:tunnelRadius()
	return self._tunnelRadius
end

function Tunnel:getSamplePosition(sampleIndex)
	return self._tunnel[sampleIndex][1]
end

function Tunnel:getSampleTangent(sampleIndex)
	return self._tunnel[sampleIndex][2]
end

function Tunnel:getSampleNormal(sampleIndex)
	local rad = math.random(0,0.8*self._tunnelRadius)
	return self._tunnel[sampleIndex][3]
end

function Tunnel:getNumSamples()
	return #self._tunnel
end

function Tunnel:getCheckpointPosition(checkpointIndex)
	return self._checkpoints[checkpointIndex][1]
end

function Tunnel:getCheckpointTangent(checkpointIndex)
	return self._checkpoints[checkpointIndex][2]
end

function Tunnel:getCheckpointNormal(checkpointIndex)
	return self._checkpoints[checkpointIndex][3]
end

function Tunnel:getNumCheckpoints()
	return #self._checkpoints
end

function Tunnel:getPickupPosition(pickupIndex)
	local center = self._pickups[pickupIndex][1]
	local tangent = self._pickups[pickupIndex][2]
	local normal = self._pickups[pickupIndex][3]
	local rad = math.random(0,0.8*self._tunnelRadius)
	local ang = math.random(0,2*math.pi)

	return center + (normal * (math.cos(ang) * rad)) + 
						((tangent:cross(normal):getNormalized()) * (math.sin(ang) * rad))

end

function Tunnel:getPickupTangent(pickupIndex)
	return self._pickups[pickupIndex][2]
end

function Tunnel:getPickupNormal(pickupIndex)
	return self._pickups[pickupIndex][3]
end

function Tunnel:getNumPickups()
	return #self._pickups
end

function Tunnel:createTunnel()
	-- Params
	local minStartRadius = 500
	local maxStartRadius = 550
	local minControlRadius = 6000
	local maxControlRadius = 8000
	local minAnchorRadius = 500
	local maxAnchorRadius = 550

	-- Set seed and create table of curves
	math.randomseed(self._tunnelSeed)
	local curves = {}
	
	-- CREATE THE SPLINE --------------------------------------------------------------------------

	-- First curve
	local p0 = Vector(0, 0, 0)
	local p1 = Vector(0, 0, -1) * getRandomRadius(minStartRadius, maxStartRadius)
	local p2 = p1 + (Vector(0, 0, -1) * getRandomRadius(minStartRadius, maxStartRadius))
	local p3 = p2 + Vector(0, 0, -1) * getRandomRadius(minStartRadius, maxStartRadius)
	curves[1] = Bezier(p0, p1, p2, p3)

	-- Middle of spline
	for i = 2, self._numCurves-1 do
		p0 = curves[i-1].p3
		p1 = p0 + (((p0 - curves[i-1].p2):getNormalized()) * getRandomRadius(minControlRadius, maxControlRadius))
		p3 = p0 + getRandomPoint(minAnchorRadius, maxAnchorRadius)
		p2 = p3 + getRandomPoint(minControlRadius, maxControlRadius)
		curves[i] = Bezier(p0, p1, p2, p3)
	end

	-- Last curve
	p0 = curves[self._numCurves-1].p3
	p1 = p0 + (((p0 - curves[self._numCurves-1].p2):getNormalized()) * ((p3-p1):length()+minAnchorRadius))
	p3 = curves[1].p0
	p2 = p3 + (((p3 - curves[1].p1):getNormalized()) * ((p3-p1):length()+minAnchorRadius))
	curves[self._numCurves] = Bezier(p0, p1, p2, p3)

	-- CREATE THE SAMPLES -------------------------------------------------------------------------

	local normalHelper = Vector(0,1,0)
	local lastNormal = Vector(0,1,0)
	local ringSamples = {}
	local curveSamples = {}

	for curveIndex = 1, self._numCurves do
		local bezier = curves[curveIndex]
		for t = 0, 1-(1/self._ringsPerCurve), 1/self._ringsPerCurve do
			local point = bezier:getPoint(t)
			local tangent = (bezier:getDerivativePoint(t)):getNormalized()
			local normal = (tangent:cross(normalHelper)):getNormalized()
			if normal.x < math.epsilon and normal.x > (-math.epsilon) and 
			   normal.y < math.epsilon and normal.y > (-math.epsilon) and 
			   normal.z < math.epsilon and normal.z > (-math.epsilon) then normal = lastNormal end
			self._tunnel[#curveSamples+1] = { point, tangent, normal }

			normalHelper = normal:cross(tangent)
			lastNormal = normal
			curveSamples[#curveSamples+1] = point

			for ringSampleIndex = 0, self._samplesPerRing-1 do
				local theta = ringSampleIndex * 2 * math.pi / self._samplesPerRing
				local ringSample = point + (normal * (math.cos(theta) * self._tunnelRadius)) + 
						((tangent:cross(normal):getNormalized()) * (math.sin(theta) * self._tunnelRadius))
				ringSamples[#ringSamples+1] = ringSample
			end
		end
	end

	-- CREATE THE MESH ----------------------------------------------------------------------------

	local numRings = self._numCurves * self._ringsPerCurve
	local mb = MeshBuilder("LineStrip")

	for ringIndex = 0, numRings-1 do
		for ringSampleIndex = 0, self._samplesPerRing-1 do
			local ind = ((ringIndex * self._samplesPerRing) + ringSampleIndex) + 1 
			local ringSample = ringSamples [ ((ringIndex * self._samplesPerRing) + ringSampleIndex) + 1 ]
			local sampleNormal = ringSample-curveSamples[ringIndex + 1]			
			mb:position(ringSample)
			mb:normal(-sampleNormal)			
		end
	end
	for ringIndex =  0, numRings-1 do
		local currRingSampleBase = ringIndex * self._samplesPerRing
		local nextRingSampleBase = (currRingSampleBase + self._samplesPerRing) % #ringSamples
		for ringSampleIndex = 0, self._samplesPerRing-2 do
			mb:index(nextRingSampleBase+ringSampleIndex+1)		
			mb:index(currRingSampleBase+ringSampleIndex+1)
		end
	end

	local m = mb:getMesh()
	local mr = self:owner():addComponent("MeshRenderer")
	mr:start()
	mr:setMesh(m)
	local mat = Material("tunnel")
	mr:setMaterial(mat)

	-- Add specific samples to checkpoints member for convenience
	local checkpointIndex = 1
	local countIndex = 1
	while checkpointIndex*self._samplesPerCheckpoint < #self._tunnel do
		if checkpointIndex == 1 or checkpointIndex > 7 then
			self._checkpoints[countIndex] = self._tunnel[checkpointIndex*self._samplesPerCheckpoint]
			countIndex = countIndex + 1
		end
		checkpointIndex = checkpointIndex + 1
	end

		-- Add specific samples to pickups member for convenience
	local pickupIndex = 1
	while pickupIndex*self._samplesPerPickup < #self._tunnel do
		self._pickups[pickupIndex] = self._tunnel[pickupIndex*self._samplesPerPickup]
		pickupIndex = pickupIndex + 1
	end
end

function Tunnel:setSeed(seed)
	self._tunnelSeed = seed 
end