cmake_minimum_required(VERSION 2.8)
project(icg15)

#--- Load the common ICG configuration
include(common/icg_settings.cmake)

#add_subdirectory(helloworld)

add_subdirectory(intro_to_glsl)
add_subdirectory(intro_to_glsl_SOLUTION)

add_subdirectory(spiral)       #HW 2 - part 1
add_subdirectory(checkerboard) #HW 2 - part 2
add_subdirectory(planets)      #HW 2 - part 3


