all: client server_my
client: client.cpp connectAPI.cpp
	g++ connectAPI.cpp client.cpp -o client -std=c++11
server: server.cpp
	g++ server.cpp -o server_my -std=c++11
clean:
	rm server client