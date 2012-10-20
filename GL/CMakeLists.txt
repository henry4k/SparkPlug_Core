AddModule(${ModulePrefix}gl)

RequiredPackage(OpenGL)
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
	LINK_LIBRARIES(${ModulePrefix}gl ${OPENGL_gl_LIBRARY})

RequiredPackage(GLEW)
	INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
	LINK_LIBRARIES(${ModulePrefix}gl ${GLEW_LIBRARY})

InstallModule(${ModulePrefix}gl)