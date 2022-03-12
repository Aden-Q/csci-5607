CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: raytracer
clean:
	rm -f *.o *.h.gch raytracer
test: raytracer
	./raytracer
.PHONY: all clean test

raytracer: raytracer.o utils.o scene.o color.o material_color.o texture.o bump.o sphere.o cylinder.o triangle.o ray.o bump.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)