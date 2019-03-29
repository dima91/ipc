cmake_minimum_required(VERSION 3.0)

include_directories (../../include .)


add_executable			(ApiServer server.cpp)
target_link_libraries	(ApiServer ipc pthread)

add_executable			(ApiClient client.cpp)
target_link_libraries	(ApiClient ipc pthread)