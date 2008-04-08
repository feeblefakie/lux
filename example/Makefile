all: index.cpp search.cpp
	g++ -o index index.cpp -L/usr/local/lib -llux
	g++ -o search search.cpp -L/usr/local/lib -llux

clean:
	rm -f *.o index search
