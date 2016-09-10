cmake_minimum_required(VERSION 3.3)
project(Schism)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
if(UNIX)
    find_package(OpenAL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(DevIL REQUIRED)
    find_package(Freetype REQUIRED)
    #find_package(PythonLibs REQUIRED)
    #find_package(Lua REQUIRED)
endif(UNIX)

#/mnt/Windows/Users/AllFather/Downloads/Libraries/include
set(SYSDIR /mnt/Windows/Users/AllFather/Documents/)
#set(SYSDIR L:/Users/AllFather/Documents/Wretched/include)
#"main.cpp"
file(GLOB_RECURSE SOURCE_FILES
        ${SYSDIR}/Wretched/src/*.cpp
        )

#need python, GL, libsndfile, and freetype2 include directories
include_directories(
        ${OPENAL_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        ${IL_INCLUDE_DIR}
        "/usr/include/"
        ${SYSDIR}/Wretched/include
        #"L:/Users/AllFather/Downloads/Libraries/include"
)

link_libraries("/usr/lib/")
add_executable(Schism ${SOURCE_FILES} main.cpp PlayerScript.cpp InterfaceIds.h SerialMessenger.cpp SerialMessenger.h Identifier.cpp Identifier.h AuthServer.cpp MainServer.cpp DatabaseServer.cpp)
#add_executable(Schism_Server ${SOURCE_FILES} SchismServer.cpp PlayerScript.cpp InterfaceIds.h)
#ddadd_executable(Schism_Client ${SOURCE_FILES} SchismClient.cppZZ PlayerScript.cpp InterfaceIds.h)
target_link_libraries(Schism
        #"L:/Users/AllFather/Downloads/Libraries/Win64"
        ${OPENGL_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${GLEW_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${IL_LIBRARIES}
        ${ILU_LIBRARIES}
        pugixml
        sfml-system
        sfml-window
        sndfile
        assimp
        )