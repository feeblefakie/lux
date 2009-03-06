CPP=g++
#objs=server client cluster_client cluster_dispatcher
#objs=cluster_server cluster_client cluster_dispatcher client_searcher client_indexer sq tp libluxd.so
objs=cluster_server cluster_dispatcher client_searcher client_indexer sq tp libluxd.so
CPPFLAGS=-O2

all: $(objs)

cluster_server: cluster_server.cpp index_server.cpp handler.cpp update-protocol.pb.cpp server_config_helper.cpp server-config.pb.cpp
	$(CPP) $(CPPFLAGS) -g -Wall cluster_server.cpp index_server.cpp handler.cpp update-protocol.pb.cpp server_config_helper.cpp server-config.pb.cpp -o $@ -I../.. -lpthread -DDEBUG -L../.libs -llux -L/usr/local/lib -lprotobuf

cluster_client: cluster_client.cpp cluster_searcher.cpp
	$(CPP) $(CPPFLAGS) -g -Wall cluster_client.cpp cluster_searcher.cpp -o $@ -I../.. -DDEBUG -L../.libs -llux -L/usr/local/lib -levent -I/usr/local/include

cluster_dispatcher: cluster_dispatcher.cpp dispatcher.cpp cluster_searcher.cpp handler.cpp server-config.pb.cpp server_config_helper.cpp
	$(CPP) $(CPPFLAGS) -g -Wall cluster_dispatcher.cpp dispatcher.cpp cluster_searcher.cpp handler.cpp server-config.pb.cpp server_config_helper.cpp -o $@ -I../.. -DDEBUG -L../.libs -llux -L/usr/local/lib -levent -lprotobuf -I/usr/local/include

client_searcher: client_searcher.cpp searcher.cpp server_config_helper.cpp server-config.pb.cpp
	$(CPP) $(CPPFLAGS) -g -Wall client_searcher.cpp searcher.cpp server_config_helper.cpp server-config.pb.cpp -o $@ -I../.. -DDEBUG -L../.libs -llux -L/usr/local/lib -lprotobuf

client_indexer: client_indexer.cpp indexer.cpp update-protocol.pb.cpp
	$(CPP) $(CPPFLAGS) client_indexer.cpp indexer.cpp update-protocol.pb.cpp -o $@ -DDEBUG -L/usr/local/lib -lprotobuf -lpthread -I../..

libluxd.so: searcher.cpp
	$(CPP) $(CPPFLAGS) -g -Wall -fPIC -c searcher.cpp -I../.. -DDEBUG
	$(CPP) $(CPPFLAGS) -g -Wall -shared -o $@ searcher.o -I../.. -DDEBUG -L../.libs -llux

sq: sq.cpp searcher.cpp server_config_helper.cpp server-config.pb.cpp
	$(CPP) $(CPPFLAGS) -g -Wall sq.cpp searcher.cpp server_config_helper.cpp server-config.pb.cpp -o $@ -I../.. -DDEBUG -L../.libs -llux -L/usr/local/lib -lprotobuf

tp: tp.cpp searcher.cpp server_config_helper.cpp server-config.pb.cpp
	$(CPP) -g -Wall tp.cpp searcher.cpp server_config_helper.cpp server-config.pb.cpp -o $@ -I../.. -DDEBUG -L../.libs -llux -L/usr/local/lib -lprotobuf

clean:
	rm -f $(objs) 
