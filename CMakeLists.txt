cmake_minimum_required(VERSION 3.20...4.3)
project(QuantumAtom)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(QuantumAtom ${SOURCES})

target_link_libraries(QuantumAtom
    PRIVATE
    glfw
    glad::glad
    glm::glm
)

# Copy shaders next to the executable
add_custom_command(TARGET QuantumAtom POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:QuantumAtom>/shaders
)