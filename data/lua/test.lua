
testTable =
{
	"640x480",
	"1280x1280",
	"800x600",
	"540*12090",
	"573x9494",
	"errorxerrorxerror",
	"blahx456"
};

function printDebug(str)
	print(str);
end

--[[
	Takes a string in the format "IntAxIntB" and splits it into two ints.

	@param resStr The resolution string.

	@return A LUA table (= {IntA, IntB}) or an empty table on error.
]]
function splitResolutionString(resStr)
	local FUNCTION_NAME = "splitResolutionString(): ";	-- Name of function/Debug prints.
	local resolutionTable = {};							-- Output table.

	-- Validate input.
	if (type(resStr) ~= "string") then
		printDebug(FUNCTION_NAME .. "Invalid type for resStr supplied: " .. type(resStr));
		return (resolutionTable);
	end

	-- Get the location of the first "x" character in the string.
	local SPLIT_CHAR = "x";								-- Character to split the resolution string around.
	local sepIndex = string.find(resStr, SPLIT_CHAR);
	if (sepIndex == nil) then
		printDebug(FUNCTION_NAME .. "Failed to find the " .. SPLIT_CHAR .. " character in the string: " .. resStr);
		return (resolutionTable);
	end

	-- Split the string about the character and convert each substring to ints.
	local resWidthStr = string.sub(resStr, 0, sepIndex-1);
	local resHeightStr = string.sub(resStr, sepIndex+1, -1);
	resolutionTable[1] = tonumber(resWidthStr);
	resolutionTable[2] = tonumber(resHeightStr);
	if(#resolutionTable ~= 2 or resolutionTable[1] == nil or resolutionTable[2] == nil) then
		printDebug(FUNCTION_NAME .. "The string " .. resStr .. " contains invalid numbers");
		resolutionTable = {};
		return (resolutionTable);
	end

	-- Success
	return (resolutionTable);
end

for i=1, #testTable, 1 do
	local resTable = splitResolutionString(testTable[i]);

	if(#resTable == 2) then
		print(i .. ": WxH " .. resTable[1] .. "x" .. resTable[2]);
	end
end
