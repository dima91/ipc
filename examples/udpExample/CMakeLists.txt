cmake_minimum_required(VERSION 3.0)

include_directories (../../include)

add_executable			(UdpServer server.cpp)
target_link_libraries	(UdpServer ipc)

add_executable			(UdpClient client.cpp)
target_link_libraries	(UdpClient ipc)