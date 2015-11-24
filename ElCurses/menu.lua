local Menu = require("gamestate"):new()

Menu.mainWindow = el.window.create(0, 0, 0, 0)
Menu.gameSelectWindow = el.window.create(3, 3, 60, 10)
Menu.gameSelectWindow:setBorder(0, 0, 0x00005E, 0x000061, 0x00002B, 0x00002B, 0, 0)

local choice = 0
local MAXCHOICE = 1

function Menu:new(o)
	local o = o or {}
	setmetatable(o, Menu)
	self.__index = Menu
	return o
end

function Menu:update(dt)
end

local function printCenter(window, text, y, color)
	color = color or 0
	window:setColor(color)
	local w, h = window:getSize();
	local l = string.len(text)
	window:print(math.floor(w / 2 - l / 2), y, text)
end

function Menu:draw()
	-- Clear screen
	local w, h = self.mainWindow:getSize()
	self.mainWindow:setColor(3)
	self.mainWindow:clear()
	--for x = 0, w do
	--	for y = 0, h do
	--		self.mainWindow:print(x, y, ' ')
	--	end
	--end
	self.mainWindow:setColor(0)

	local w2, h2 = self.gameSelectWindow:getSize();
	self.gameSelectWindow:setPosition(w / 2 - w2 / 2 - 1, h / 2 - h2 / 2 - 1)

	printCenter(self.gameSelectWindow, "MAIN MENU", 1)

	local color = 0
	local x = 5
	if choice == 0 then color = 1 x = 6 end
	self.gameSelectWindow:setColor(color)
	self.gameSelectWindow:print(x, 4, "SANEK")

	color = 0
	x = 5
	if choice == 1 then color = 1 x = 6 end
	self.gameSelectWindow:setColor(color)
	self.gameSelectWindow:print(x, 5, "BLIP BOP")

	self.gameSelectWindow:setColor(0)
end

function Menu:keyDown(key, name)
	-- 259 -- up
	-- 258 -- down
	-- 260 -- left
	-- 261 -- right

	if key == 10 then
		if choice == 0 then
			self.gm:push(require("snake"):new())
		elseif choice == 1 then
			self.gm:push(require("pong"):new())
		end
	end

	if key == 258 then
		choice = choice + 1
		if choice > MAXCHOICE then
			choice = 0
		end
	end

	if key == 259 then
		choice = choice - 1
		if choice < 0 then
			choice = MAXCHOICE
		end
	end

	-- Clear to remove garbage when offsetting text in draw later on
	self.gameSelectWindow:clear()
end

function Menu:gamepadDown(playerNumber, key)
	if key == "A" then
		if choice == 0 then
			self.gm:push(require("snake"):new())
		elseif choice == 1 then
			self.gm:push(require("pong"):new())
		end
	end

	if key == "UP" then
		choice = choice - 1
		if choice < 0 then
			choice = MAXCHOICE
		end
	end

	if key == "DOWN" then
		choice = choice + 1
		if choice < 0 then
			choice = MAXCHOICE
		end
	end


	-- Clear to remove garbage when offsetting text in draw later on
	self.gameSelectWindow:clear()
end


return Menu
