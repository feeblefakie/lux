bin=index search
all: $(bin)

index: index.cpp
	g++ -g $< -o $@ -I.. -L../lux/.libs -llux -O2 -L/usr/local/lib

search: search.cpp
	g++ -g $< -o $@ -I.. -L../lux/.libs -llux -O2 -L/usr/local/lib

clean:
	rm -f *.o $(bin)
