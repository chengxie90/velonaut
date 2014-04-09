Scene = createClass()

Scene._gameObjects = {}

function Scene:find (name)
	print("Scene find" .. name)
end

function Scene:a()
	print("a")
end


AScene = createClass(Scene)

function AScene:find(name)
	print("AScene find" .. name)
end

function AScene:b()
	print("b")
end

