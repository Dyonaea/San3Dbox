cmake_minimum_required(VERSION 3.16)
project(San3Dbox VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER:FILEPATH=C:/mingw64/mingw64/mingw64/bin/g++.exe)

# ─────────────────────────────────────────────
# Dépendances via FetchContent
# ─────────────────────────────────────────────
include(FetchContent)

# GLFW (fenêtre + input)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# GLAD (chargement des fonctions OpenGL)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# GLM (maths 3D)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# ASIO (réseau)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG master
)
FetchContent_MakeAvailable(asio)

# ─────────────────────────────────────────────
# Sources
# ─────────────────────────────────────────────
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cpp
    include/${PROJECT_NAME}/*.hpp
    include/${PROJECT_NAME}/*/*.hpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${asio_SOURCE_DIR}/asio/include
)

# ─────────────────────────────────────────────
# Liens
# ─────────────────────────────────────────────
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 ws2_32 mswsock)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL pthread)
endif()
