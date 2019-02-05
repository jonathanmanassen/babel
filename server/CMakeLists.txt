cmake_minimum_required(VERSION 3.5)

project(server)

include_directories(include ${CMAKE_CURRENT_BINARY_DIR}/include)

set (SRC src/BabelServer.cpp src/Connect.cpp src/ContactList.cpp src/NetworkUser.cpp src/Server.cpp
     include/Connect.h include/ContactList.h include/NetworkUser.h include/Server.h)

add_executable(server
	${SRC}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fpermissive")

if (UNIX)
set(LIBRARIES
  boost_system
)
endif (UNIX)

if (WIN32)
set(LIBRARIES
)
endif (WIN32)

target_link_libraries(
  server
  ${LIBRARIES}
)

if (UNIX)

add_custom_target(
fclean
COMMAND rm -rf CMakeFiles/ CMakeCache.txt cmake_install.cmake install_manifest.txt Makefile server
)

endif (UNIX)
