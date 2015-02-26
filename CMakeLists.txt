cmake_minimum_required(VERSION 2.8)
project(icg15)

#--- Load the common ICG configuration
include(common/icg_settings.cmake)

#add_subdirectory(helloworld)

add_subdirectory(intro_to_glsl)
add_subdirectory(intro_to_glsl_SOLUTION)

#--- Homework 2
add_subdirectory(hw2p1_spiral)       
add_subdirectory(hw2p2_checkerboard) 
add_subdirectory(hw2p3_planets)      
