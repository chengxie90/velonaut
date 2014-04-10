local Scene = createClass()

Scene._gameObjects = {}

function Scene:find (name)
	print("Scene find" .. name)
end

function Scene:a()
	print("a")
end

return Scene;



