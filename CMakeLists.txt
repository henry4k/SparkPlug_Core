AddModule(${ModulePrefix}common)

IF(UNIX AND NOT APPLE)
	INCLUDE(CheckFunctionExists)

	CHECK_FUNCTION_EXISTS("clock_gettime" HAVE_CLOCK_GETTIME)
	IF(NOT HAVE_CLOCK_GETTIME)
		SET(CMAKE_REQUIRED_LIBRARIES "rt")
		CHECK_FUNCTION_EXISTS("clock_gettime" RT_HAS_CLOCK_GETTIME)
		UNSET(CMAKE_REQUIRED_LIBRARIES)
		IF(RT_HAS_CLOCK_GETTIME)
			TARGET_LINK_LIBRARIES(${ModulePrefix}common "rt")
			SET(HAVE_CLOCK_GETTIME 1)
		ELSE()
			MESSAGE(WARNING "Can't find clock_gettime! Please report this case the developers!")
		ENDIF()
	ENDIF()
ENDIF()

IF(WIN32)
	TARGET_LINK_LIBRARIES(${ModulePrefix}common "winmm") # "comctl32" "user32" "ole32")
	# ^- ??!
# ELSEIF(APPLE)
# 	SET(ClientFrameworks "Cocoa")
ENDIF()

InstallModule(${ModulePrefix}common)

ADD_SUBDIRECTORY("gl")
