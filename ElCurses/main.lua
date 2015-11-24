el.console.show();
el.console.setSize(40, 10)
el.console.setPosition(100 - 40, 30 - 10)

local g = require("gamestateManager")
g:push(require("menu"):new())

el.color.initPair(0, el.color.white, el.color.black)
el.color.initPair(1, el.color.red | el.color.green, el.color.black)
el.color.initPair(2, el.color.red, el.color.black)

el.color.initPair(3, el.color.blue, el.color.blue)

math.randomseed(os.time())

function el.update(dt)
	g:update(dt)
end

function el.draw()
	g:draw()

	if showConsole then
		el.console.show() -- Always draw console on top
	end
end

function el.key(key, name)
	if name == "d" then
		showConsole = not showConsole

		if showConsole then
			el.console.show()
		else
			el.console.hide()
		end
	end


	g:keyDown(key, name)
end

function el.gamepadDown(playerNumber, key)
	g:gamepadDown(playerNumber, key)
end
