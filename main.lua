WHITE = {
	r = 255,
	g = 255,
	b = 255,
	a = 255
}

COLORS = {
	[0] = GREEN,
	default = BLUE
}

function tprint (tbl, indent)
  if not indent then indent = 0 end
  local toprint = string.rep(" ", indent) .. "{\r\n"
  indent = indent + 2 
  for k, v in pairs(tbl) do
    toprint = toprint .. string.rep(" ", indent)
    if (type(k) == "number") then
      toprint = toprint .. "[" .. k .. "] = "
    elseif (type(k) == "string") then
      toprint = toprint  .. k ..  "= "   
    end
    if (type(v) == "number") then
      toprint = toprint .. v .. ",\r\n"
    elseif (type(v) == "string") then
      toprint = toprint .. "\"" .. v .. "\",\r\n"
    elseif (type(v) == "table") then
      toprint = toprint .. tprint(v, indent + 2) .. ",\r\n"
    else
      toprint = toprint .. "\"" .. tostring(v) .. "\",\r\n"
    end
  end
  toprint = toprint .. string.rep(" ", indent-2) .. "}"
  return toprint
end

-- TODO: SCREEN_W, SCREEN_H should be global
function shouldRender(x, y)
	local SCREEN_W = 1280
	local SCREEN_H = 720

	return x >= 0 and x < SCREEN_W and y >= 0 and y < SCREEN_H
end

function render()
	if IsKeyDown(KEY_D) then
		data.player.x = data.player.x + 1
	elseif IsKeyDown(KEY_A) then
		data.player.x = data.player.x - 1
	end

	if IsKeyDown(KEY_W) then
		data.player.y = data.player.y - 1
	elseif IsKeyDown(KEY_S) then
		data.player.y = data.player.y + 1
	end

	local drawn = 0
	for x = 0, MAP_W do
		for y = 0, MAP_H do
			if shouldRender(x * 16, y * 16) then	
				if COLORS[GetTileAt(x, y).id] then
					DrawRect(x * 16, y * 16, 16, 16, COLORS[GetTileAt(x, y).id])
				else	
					DrawRect(x * 16, y * 16, 16, 16, COLORS["default"])
				end
				drawn = drawn + 1
			end
		end
	end

	DrawRect(data.player.x, data.player.y, 16, 16, WHITE);

	if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
		print("Pressed Mouse Left Button")
		local xRnd = math.random(0, MAP_W)
		local yRnd = math.random(0, MAP_H)
		print(xRnd)
		print(yRnd)
		SetTileAt(xRnd, yRnd, 1)
	end

	DrawText("FPS: ".. GetFPS(), 10, 10, 12, WHITE)
	DrawText(string.format("Drawn %d/%d", drawn, MAP_W * MAP_H), 10, 22, 12, WHITE)
end


