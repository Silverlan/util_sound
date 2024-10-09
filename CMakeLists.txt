include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

set(PROJ_NAME util_sound)
pr_add_library(${PROJ_NAME} SHARED)

pr_add_dependency(${PROJ_NAME} vfilesystem TARGET PUBLIC)
pr_add_dependency(${PROJ_NAME} mpaheader TARGET)
pr_add_dependency(${PROJ_NAME} vorbis TARGET)
pr_add_dependency(${PROJ_NAME} vorbisfile TARGET)
pr_add_dependency(${PROJ_NAME} ogg TARGET)

pr_init_module(${PROJ_NAME})

pr_add_compile_definitions(
	${PROJ_NAME}
		-DUSND_DLL
)

pr_finalize(${PROJ_NAME})
