bin=index bindex search
all: $(bin)

index: index.cpp
	g++ -g $< -o $@ -I. -L./lux/.libs -llux -O2

bindex: bindex.cpp
	g++ -g $< -o $@ -I. -L./lux/.libs -llux -O0

search: search.cpp
	g++ -g $< -o $@ -I. -L./lux/.libs -llux -O2

clean:
	rm -f *.o $(bin)
