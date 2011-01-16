-- A simple test script
print("Lua test program by Shine", "Press circle for testing and cross for exit")

lastPad = 0
while true do
	waitVblankStart();
	pad = ctrlRead()
	if lastPad ~= pad then
		lastPad = pad
		if isCtrlCircle(pad) then
			print("circle pressed")
		elseif isCtrlCross(pad) then
			break
		end
	end
end
