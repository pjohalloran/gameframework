OPENAL_INCLUDE_DIR=nil
OPENAL_LIB_DIR=nil

-- We only checkand allow configuration for preinstalled OpenAL dirs on Windows and Linux.
-- We know that for OSX, OpenAL is always installed in the Frameworks dir...
if (os.getversion().description == "windows") then
	-- directory assuming you installed OpenAL SDK to default location...
	OPENAL_INCLUDE_DIR="C:/Program Files (x86)/OpenAL 1.1 SDK/include/"
	OPENAL_LIB_DIR="C:/Program Files (x86)/OpenAL 1.1 SDK/libs/WIN32/"
elseif (os.getversion().description == "linux") then
	-- TODO...
	OPENAL_INCLUDE_DIR=""
	OPENAL_LIB_DIR=""
end

if not os.isdir(OPENAL_INCLUDE_DIR) or not os.isdir(OPENAL_LIB_DIR) then
	print("Could not find OPENAL_INCLUDE_DIR (" .. OPENAL_INCLUDE_DIR .. ") or OPENAL_LIB_DIR (" .. OPENAL_LIB_DIR .. ") on the path")
	return
end



print()