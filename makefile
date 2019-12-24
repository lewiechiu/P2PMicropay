all: client server
client: client.cpp connectAPI.cpp
	g++ connectAPI.cpp client.cpp -o client -std=c++11
server: srv.cpp server.cpp
	g++ srv.cpp server.cpp -o server -std=c++11 -pthread
clean:
	rm server_my client