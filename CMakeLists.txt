cmake_minimum_required(VERSION 3.5)
project(thermal_conductivity_2D)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED )

include_directories(thread_pool)


add_executable(thermal_conductivity_2D main.cpp config.cpp visualization.cpp my_queue.h config.h visualization.h thread_pool/ctpl.h)
add_executable(generate_field generate_field.cpp)


target_link_libraries(thermal_conductivity_2D Threads::Threads)
target_link_libraries(thermal_conductivity_2D  ${OpenCV_LIBS} )

