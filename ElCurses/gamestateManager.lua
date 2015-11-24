local GamestateManager = {}

GamestateManager.states = {}

function GamestateManager:update(dt)
	if (#self.states <= 0) then
		el.exit()
		return
	end

	self.states[#self.states]:update(dt)
end

function GamestateManager:draw()
	if (#self.states <= 0) then
		return
	end

	self.states[#self.states]:draw()
end

function GamestateManager:keyDown(key, name)
	if key == 27 then
		self:pop()
		return
	end

	if (#self.states <= 0) then
		return
	end

	self.states[#self.states]:keyDown(key, name)
end

function GamestateManager:gamepadDown(playerNumber, key)
	if key == "BACK" then
		self:pop()
		return
	end

	if (#self.states <= 0) then
		return
	end

	self.states[#self.states]:gamepadDown(playerNumber, key)
end

function GamestateManager:pop()
	if (#self.states <= 0) then
		return
	end

	self.states[#self.states]:exit()
	table.remove(self.states, #self.states)
end

function GamestateManager:push(gamestate)
	table.insert(self.states, gamestate)
	gamestate:init(self)
end

return GamestateManager
