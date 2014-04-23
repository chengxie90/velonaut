require "engine.class"

Input = class()

-- Input State: 0 = NotPressed, 1 = Down, 2 = Pressed, 3 = Up
local inputState = {}
local mousePosition = {0,0}

function Input.createDictionary(...)
	local args = {...}
    for i = 1, #args do
        inputState[args[i]] = 0
    end
end

function Input.update()
	for k, v in pairs(inputState) do
		if v == 1 then
			inputState[k] = 2
		elseif v == 3 then
			inputState[k] = 0
		end
	end

end

function Input.onMouseDown(key, modifierState)
	if inputState[key] == 0 then
		inputState[key] = 1
	end
end

function Input.onMouseUp(key, modifierState)
	if inputState[key] == 1 or inputState[key] == 2 then
		inputState[key] = 3
	end
end

function Input.onMouseMove(x, y, modifierState)
	mousePosition[1] = x
	mousePosition[2] = y
end

function Input.onKeyDown(key)
	if inputState[key] == 0 then
		inputState[key] = 1
	end
end

function Input.onKeyUp(key)
	if inputState[key] == 1 or inputState[key] == 2 then
		inputState[key] = 3
	end
end

function Input.getKeyDown(key)
	return inputState[key] == 1
end

function Input.getKey(key)
	return inputState[k] == 1 or inputState[k] == 2
end

function Input.getKeyUp(key)
	return inputState[k] == 3
end