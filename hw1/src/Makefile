CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: ray_casting
clean:
	rm -f *.o *.h.gch ray_casting
test: ray_casting
	./ray_casting
.PHONY: all clean test

ray_casting: ray_casting.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)