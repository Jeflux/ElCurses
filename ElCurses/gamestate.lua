GameState = {}

function GameState:new(o)
	local o = o or {}
	setmetatable(o, GameState)
	self.__index = GameState
	return o
end

function GameState:init(gamestateManager)
	self.gm = gamestateManager
end
function GameState:update(dt) end
function GameState:draw() end
function GameState:keyDown(key, name) end
function GameState:gamepadDown(playerNumber, key) end
function GameState:exit(key, name) end

return GameState
