CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: raytracer
clean:
	rm -f *.o *.h.gch ray_casting
test: raytracer
	./ray_casting
.PHONY: all clean test

raytracer: raytracer.o utils.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)