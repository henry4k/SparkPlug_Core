AddModule(${ModulePrefix}GL)
TARGET_LINK_LIBRARIES(${ModulePrefix}GL ${ModulePrefix}Common)

RequiredPackage(OpenGL)
	AddIncludeDirs(${ModulePrefix}GL ${OPENGL_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(${ModulePrefix}GL ${OPENGL_gl_LIBRARY})

RequiredPackage(GLEW)
	AddIncludeDirs(${ModulePrefix}GL ${GLEW_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(${ModulePrefix}GL ${GLEW_LIBRARIES})

InstallModule(${ModulePrefix}GL)
