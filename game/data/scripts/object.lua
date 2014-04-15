require "utility"
require "component"

Object = createClass()

function Object:init(data)
    self.components = {}
    for _, componentData in ipairs(data.components) do
        local type = componentData.type
        local comp = _G[type].new() -- dynamic creation
        comp:init(componentData)
        table.insert(self.components, comp)
    end
end