cmake_minimum_required (VERSION 2.6)
project ("Zombie Game")
add_executable(zombie camera.cpp game.cpp main.cpp marching.cpp navigation.cpp object.cpp render.cpp)
target_link_libraries (zombie  Box2D GL GLEW SDL2 SDL2_image SDL2_ttf)