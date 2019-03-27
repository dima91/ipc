cmake_minimum_required(VERSION 3.0)

include_directories (../../include)

add_executable			(UnixServer server.cpp)
target_link_libraries	(UnixServer ipc)

add_executable			(UnixClient client.cpp)
target_link_libraries	(UnixClient ipc)