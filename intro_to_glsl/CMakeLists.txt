get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})

target_deploy_file(${EXERCISENAME} triangle_vshader.glsl) 
target_deploy_file(${EXERCISENAME} triangle_fshader.glsl)

target_deploy_file(${EXERCISENAME} quad_vshader.glsl) 
target_deploy_file(${EXERCISENAME} quad_fshader.glsl)
target_deploy_file(${EXERCISENAME} mrt.tga)
