.PHONY:all
all:dict_server dict_client

dict_server:dict_server.cc
	g++ $^ -o $@ -lpthread -std=c++11 

dict_client:dict_client.cc
	g++ $^ -o $@ -lpthread -std=c++11

.PHONY:clean
clean:
	rm dict_server dict_client


