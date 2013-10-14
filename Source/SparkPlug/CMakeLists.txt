FILE(GLOB Sources "*.h" "*.cpp")
ADD_LIBRARY(sparkplug-core ${Sources})

IF(UNIX AND NOT APPLE)
	INCLUDE(CheckFunctionExists)

	CHECK_FUNCTION_EXISTS("clock_gettime" HAVE_CLOCK_GETTIME)
	IF(NOT HAVE_CLOCK_GETTIME)
		SET(CMAKE_REQUIRED_LIBRARIES "rt")
		CHECK_FUNCTION_EXISTS("clock_gettime" RT_HAS_CLOCK_GETTIME)
		UNSET(CMAKE_REQUIRED_LIBRARIES)
		IF(RT_HAS_CLOCK_GETTIME)
            TARGET_LINK_LIBRARIES(SparkPlug_Common "rt")
			SET(HAVE_CLOCK_GETTIME 1)
		ELSE()
			MESSAGE(WARNING "Can't find clock_gettime! Please report this case the developers!")
		ENDIF()
	ENDIF()
ENDIF()

IF(WIN32)
    TARGET_LINK_LIBRARIES(sparkplug-core "winmm")
ENDIF()


FILE(GLOB PublicHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
SET_TARGET_PROPERTIES(sparkplug-core PROPERTIES PUBLIC_HEADER "${PublicHeaders}")
SET_TARGET_PROPERTIES(sparkplug-core PROPERTIES VERSION "${VERSION}")

INSTALL(TARGETS sparkplug-core
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION "include/SparkPlug"
)

if(UNIX)
    SET(PKG_DEPS "")
    SET(PKG_LIBS "")
    SET(LIB_NAME "sparkplug-core")

    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/sparkplug-core.pc.in
        ${CMAKE_BINARY_DIR}/sparkplug-core.pc @ONLY
    )

    INSTALL(
        FILES ${CMAKE_BINARY_DIR}/sparkplug-core.pc
        DESTINATION lib${LIB_SUFFIX}/pkgconfig
    )
endif()
