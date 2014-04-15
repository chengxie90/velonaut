require "utility"
require "object"

Scene = createClass()

function Scene:init(data)
    self.handle = Graphics.createScene()
    Graphics.setActiveScene(self)
    self.objects = {}

    for _, name in ipairs(data.objects) do
        local obj = Object.new()
        local data = loadData(name, "object")
        obj:init(data)
        table.insert(self.objects, obj)
    end
end

