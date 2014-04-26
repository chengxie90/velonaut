require "engine.object"
local gfx = require "engine.graphics.c"
local gfxscene = require "engine.graphics.scene.c"

Scene = class()

function Scene:_init(data)
    self._handle = gfxscene.create()
    gfx.setActiveScene(self._handle)
    self._objects = {}
    self._started = false
	
end

function Scene:loadPlayers(players, playerId)
	local ringSampleIndex = 0
	local initRadius = 65
	local tangent = Vector(0,0,-1)
	local startingPoint = Vector(0,0,0)
	local normal = Vector(0,1,0)

    for _, player in ipairs(players) do
        local name = "player" .. ringSampleIndex

		local prefab
		if player.id == playerId then
			prefab = "player"
			self.playerName = name
		else
			prefab = "remoteplayer"
		end

		local obj = self:createObject(name)
        local data = loadDataFile(prefab, "object")
		
		obj:load(data)
		obj:start()
		
		local theta = ringSampleIndex * 2 * math.pi / #players
		local ringSample = startingPoint + (normal * (math.cos(theta) * initRadius)) + 
				((tangent:cross(normal):getNormalized()) * (math.sin(theta) * initRadius))
		ringSampleIndex = ringSampleIndex+1	       

        obj:getComponent("RigidBody"):setPosition(ringSample)   
        obj:transform():setPosition(ringSample)   

		if player.id == playerId then
			obj:getComponent("Player"):setId(playerId)
            self._player = obj
		else
			obj:getComponent("RemotePlayer"):setId(player.id)   
		end
    end	

    local name = "cameraman"
    local prefab = "cameraman"
    local obj = self:createObject(name)
    local data = loadDataFile(prefab, "object")
    obj:load(data)
    obj:start()
    obj:getComponent("CameraMan"):initializePosition()

	self:setMainCamera(self:findObject("cameraman"):getComponent("Camera"))	
end

function Scene:_loadObjectData(data)
    local name = data.name
    local prefab = data.prefab
    assert(name and name ~= "")
    local obj = self:createObject(name)
    local prefabData = loadDataFile(data.prefab, "object")
    obj:load(prefabData)
    if data.children then
        for _, objData in ipairs(data.children) do
            local child = self:_loadObjectData(objData)
            child:transform():setParent(obj:transform())
        end
    end
    return obj
end

function Scene:load(data)
    assert(data.objects)
    for _, objectData in ipairs(data.objects) do
        self:_loadObjectData(objectData)
    end

    local obj = self:findObject(data.mainCamera)
    if obj then 
        local cam = obj:getComponent("Camera")
        assert(cam)
        if cam then self:setMainCamera(cam) end
    end

    local bgColor = data.backgroundColor
    if bgColor then gfx.setBackgroundColor(bgColor) end

    local ambient = data.ambient
    if ambient then gfxscene.setAmbientLight(ambient) end
end

function Scene:start()
    assert(self._started == false)
    for k, v in pairs(self._objects) do
        v:start()
    end
    self._started = true
end

function Scene:update(dt)
    for k, v in pairs(self._objects) do
        v:update(dt)
    end
end

function Scene:player()
    return self._player
end

function Scene:setMainCamera(cam)
    gfxscene.setMainCamera(cam._handle)
end

function Scene:createObject(name)
    assert(self._objects[name] == nil)
    local obj = Object(name)
    self._objects[name] = obj
    return obj
end

function Scene:findObject(name)
    return self._objects[name]
end

function Scene:destroyObject(name)
    local obj = self:findObject(name)
    assert(obj)
    obj:onDestroy()
    self._objects[name] = nil
end
