local Pong = require("gamestate"):new()

local paused = false

function Pong:new(o)
	local o = o or {}
	setmetatable(o, Pong)
	self.__index = Pong


	o.window = el.window.create(0, 5, 100, 25)
	o.scoreWindow = el.window.create(0, 0, 100, 5)
	o.scoreWindow:setBorder(0, 0, 0, 0, 0, 0, 0, 0)

	o.leftScore = 0
	o.rightScore = 0

	o.ball = { x = 48, y = 10, vy = 10, vx = 20 }

	o.paddle1 = { x = 2, y = 12  }
	o.paddle2 = { x = 97, y = 12 }

	return o
end

function Pong:update(dt)
	if paused then return end

	self.ball.x = self.ball.x + self.ball.vx * dt
	self.ball.y = self.ball.y + self.ball.vy * dt

	local w, h = self.window:getSize()

	if self.ball.y <= 0 or
	   self.ball.y >= h then
		self.ball.vy = -self.ball.vy
	end

	if self.ball.x <= 0 then
		self.ball.x = 48
		self.rightScore = self.rightScore + 1
		self.ball.vx = 20
	end

	if self.ball.x >= w then
		self.ball.x = 48
		self.leftScore = self.leftScore + 1
		self.ball.vx = -20
	end

	local dx = self.ball.x - self.paddle1.x
	local dy = self.ball.y - self.paddle1.y
	if math.abs(dx) <= 1 and math.abs(dy) <= 3 then
		self.ball.vx = -self.ball.vx
		self.ball.vx = self.ball.vx * ((math.random() / 4) + 1)
	end

	dx = self.ball.x - self.paddle2.x
	dy = self.ball.y - self.paddle2.y
	if math.abs(dx) <= 1 and math.abs(dy) <= 3 then
		self.ball.vx = -self.ball.vx
		self.ball.vx = self.ball.vx * ((math.random() / 4) + 1)
	end

	if el.gamepad.isDown(0, "UP") and self.paddle1.y > 1 then
		self.paddle1.y = self.paddle1.y - 0.7
	end
	if el.gamepad.isDown(0, "DOWN") and self.paddle1.y < 23 then
		self.paddle1.y = self.paddle1.y + 0.7
	end


	if el.gamepad.isDown(1, "UP") and self.paddle2.y > 1 then
		self.paddle2.y = self.paddle2.y - 0.7
	end
	if el.gamepad.isDown(1, "DOWN") and self.paddle2.y < 23 then
		self.paddle2.y = self.paddle2.y + 0.7
	end
end

function Pong:draw()
	self.window:clear()

	local w, h = self.scoreWindow:getSize()
	local scoreString = self.leftScore .. " : " .. self.rightScore
	local scoreStringLen = string.len(scoreString)
	self.scoreWindow:print(math.floor(w / 2 - scoreStringLen / 2) - 1, 2, scoreString)

	self.window:setColor(1)
	local w2, h2 = self.window:getSize()
	for i = 0, h2 - 2 do
		self.window:print(math.floor(w / 2) - 2, i, "|");
	end
	self.window:setColor(0)

	self.window:print(math.floor(self.ball.x), math.floor(self.ball.y), "o")

	self.window:print(math.floor(self.paddle1.x), math.floor(self.paddle1.y), "|")
	self.window:print(math.floor(self.paddle1.x), math.floor(self.paddle1.y - 1), "|")
	self.window:print(math.floor(self.paddle1.x), math.floor(self.paddle1.y + 1), "|")

	self.window:print(math.floor(self.paddle2.x), math.floor(self.paddle2.y), "|")
	self.window:print(math.floor(self.paddle2.x), math.floor(self.paddle2.y - 1), "|")
	self.window:print(math.floor(self.paddle2.x), math.floor(self.paddle2.y + 1), "|")
end

function Pong:keyDown(key, name)
	-- 259 -- up
	-- 258 -- down
	-- 260 -- left
	-- 261 -- right

	if name == "w" and self.paddle1.y > 1 then
		self.paddle1.y = self.paddle1.y - 1
	end
	if name == "s" and self.paddle1.y < 23 then
		self.paddle1.y = self.paddle1.y + 1
	end


	if key == 259 and self.paddle2.y > 1 then
		self.paddle2.y = self.paddle2.y - 1
	end
	if key == 258 and self.paddle2.y < 23 then
		self.paddle2.y = self.paddle2.y + 1
	end

	if name == "p" then
		paused = not paused
	end
end

function Pong:exit(key, name)
	self.window:delete()
	self.scoreWindow:delete()
end


return Pong
