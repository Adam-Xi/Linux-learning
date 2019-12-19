FLAG=-std=c++11 -lpthread -ljsoncpp

.PHONY:all
all:calc_json_server calc_json_client

calc_json_server:calc_json_server.cc
	g++ $^ -o $@ $(FLAG)

calc_json_client:calc_json_client.cc
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm calc_json_server calc_json_client

