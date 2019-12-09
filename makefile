all: client server_my
client: client.cpp connectAPI.cpp
	g++ connectAPI.cpp client.cpp -o client -std=c++11
server_my: srv.cpp server.cpp
	g++ srv.cpp server.cpp -o server_my -std=c++11 -pthread
clean:
	rm server_my client