CXX=clang++
CXXFLAGS=-g -std=c++11 -Wall

all: simpleppm
clean:
	rm -f *.o *.h.gch simpleppm simpleppm.bmp
test: simpleppm
	./simpleppm
.PHONY: all clean test

simpleppm: simpleppm.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)