cmake_minimum_required(VERSION 3.0)

include_directories (../../include)

add_executable			(TcpServer server.cpp)
target_link_libraries	(TcpServer ipc)

add_executable			(TcpClient client.cpp)
target_link_libraries	(TcpClient ipc)