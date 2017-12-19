cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE None)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL -lGLEW -lpthread -lSDL2 -lSDL2_ttf -lSDL2_image -std=gnu++11 -fpermissive -g")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /usr/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /usr/lib)

file(GLOB_RECURSE SOURCE_FILES
	"Source/*"
)

add_definitions(-DLINUX)

include_directories(/home/andrei/Documents/Programming/Libraries/glm/glm)
include_directories(/usr/local/include)
include_directories(Source)

project(Engine)

add_executable(
	Engine
	${SOURCE_FILES}
)