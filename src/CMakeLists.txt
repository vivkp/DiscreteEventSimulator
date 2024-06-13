add_library(core core.cpp)
add_library(event event.cpp)
add_library(request request.cpp)
add_library(simulation simulation.cpp)
add_library(system system.cpp)
add_library(thread thread.cpp)

add_executable(main main.cpp)
target_link_libraries(main simulation core event request system thread)