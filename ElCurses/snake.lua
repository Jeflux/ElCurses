local Game = require("gamestate"):new()

local UP = 1
local DOWN = 2
local LEFT = 3
local RIGHT = 4

local frame = 0

function Game:new(o)
	local o = o or {}
	setmetatable(o, Game)
	self.__index = Game

	o.window = el.window.create(0, 0, 80, 30)
	o.window:setBorder(0, 0, 0, 0, 0, 0, 0, 0)
	o.scoreWindow = el.window.create(80, 0, 20, 30)
	o.scoreWindow:setBorder(0, 0, 0, 0, 0, 0, 0, 0)
	o.scoreWindow:setTitle("SCORE")
	o.snake = { head = { x = 40, y = 15, direction = 0 }, body = {} }
	o.apple = { x = 10, y = 10 }

	for i = 0, 10 do
		table.insert(o.snake.body, { x = 0, y = 0})
	end

	o.gameover = false
	o.score = 0


	return o
end

function Game:update(dt)
	if self.gameover then
		print("You died")
		return
	end

	frame = frame + 1

	if frame % 2 == 0 then
		self.snake.body[1].x = self.snake.head.x
		self.snake.body[1].y = self.snake.head.y

		for i = #self.snake.body, 2, -1 do
			self.snake.body[i].x = self.snake.body[i - 1].x
			self.snake.body[i].y = self.snake.body[i - 1].y
		end


		local d = self.snake.head.direction
		if d == UP then
			self.snake.head.y = self.snake.head.y + 1
		elseif d == DOWN then
			self.snake.head.y = self.snake.head.y - 1
		elseif d == LEFT then
			self.snake.head.x = self.snake.head.x - 2
		elseif d == RIGHT then
			self.snake.head.x = self.snake.head.x + 2
		end

		self.snake.head.x = math.floor(self.snake.head.x)
		self.snake.head.y = math.floor(self.snake.head.y)

		if d ~= 0 then
			for i, v in ipairs(self.snake.body) do
				local dx = v.x - self.snake.head.x
				local dy = v.y - self.snake.head.y
				local dist = math.sqrt(dx * dx + dy * dy)

				if dist < 1 then
					self.gameover = true
				end
			end

			local dx = self.apple.x - self.snake.head.x
			local dy = self.apple.y - self.snake.head.y
			local dist = math.sqrt(dx * dx + dy * dy)

			if math.abs(dx) < 2 and math.abs(dy) < 1 then
				table.insert(self.snake.body, { x = 0, y = 0 })
				self.apple.x = math.floor((math.random() * 77) + 2)
				self.apple.y = math.floor((math.random() * 27) + 2)

				self.apple.x = self.apple.x - (self.apple.x % 2)

				self.score = self.score + 1
			end
		end

		if self.snake.head.x <= 1 or
		   self.snake.head.x >= 79 or
		   self.snake.head.y <= 0 or
		   self.snake.head.y >= 29 then

			self.gameover = true
		end

	end
end

function Game:draw()
	-- Clear screen
	local w, h = self.window:getSize()
	for x = 0, w do
		for y = 0, h do
			self.window:print(x, y, ' ')
		end
	end

	self.window:setColor(1)
	for i, v in ipairs(self.snake.body) do
		self.window:print(math.floor(v.x), math.floor(v.y), "*")
    end

	self.window:setColor(0)
	self.window:print(math.floor(self.snake.head.x), math.floor(self.snake.head.y), "#")

	self.window:setColor(2)
	self.window:print(self.apple.x, self.apple.y, "$")
	self.window:setColor(0)

	local x = 1
	local y = 1
	local w, h = self.scoreWindow:getSize()

	self.scoreWindow:setColor(2)
	for i = 1, self.score do
		self.scoreWindow:print(x + 1, h - y - 1, "$")

		x = x + 1
		if x > w - 4 then
			y = y + 1
			x = 1
		end
	end

	self.scoreWindow:print(2, 1, "$")
	self.scoreWindow:setColor(0)
	self.scoreWindow:print(3, 1, "x" .. self.score)
end

function Game:keyDown(key, name)
	-- 259 -- up
	-- 258 -- down
	-- 260 -- left
	-- 261 -- right

	if key == 258 and self.snake.head.direction ~= DOWN then
		self.snake.head.direction = UP
	end

	if key == 259 and self.snake.head.direction ~= UP then
		self.snake.head.direction = DOWN
	end

	if key == 260 and self.snake.head.direction ~= RIGHT then
		self.snake.head.direction = LEFT
	end

	if key == 261 and self.snake.head.direction ~= LEFT then
		self.snake.head.direction = RIGHT
	end
end

function Game:gamepadDown(playerNumber, key)
	if key == "DOWN" and self.snake.head.direction ~= DOWN then
		self.snake.head.direction = UP
	end

	if key == "UP" and self.snake.head.direction ~= UP then
		self.snake.head.direction = DOWN
	end

	if key == "LEFT" and self.snake.head.direction ~= RIGHT then
		self.snake.head.direction = LEFT
	end

	if key == "RIGHT" and self.snake.head.direction ~= LEFT then
		self.snake.head.direction = RIGHT
	end
end

function Game:exit(key, name)
	self.window:delete()
	self.scoreWindow:delete()
end


return Game
