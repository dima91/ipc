cmake_minimum_required(VERSION 3.0)

include_directories (../../include)

add_executable			(Server server.cpp)
target_link_libraries	(Server ipc)

add_executable			(Client client.cpp)
target_link_libraries	(Client ipc)