all: render

%.d: %.cpp
	rm -f $@; \
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

CC=mpicc
CXX=mpic++

SOURCES = edge.cpp face.cpp image.cpp main.cpp material.cpp matrix.cpp mesh.cpp raytracer.cpp sphere.cpp controller.cpp
EXECUTABLE = render
LIBS = -lmpi++
INCLUDES = -I/usr/lib/openmpi/include

CXXFLAGS := $(CXXFLAGS) $(INCLUDES)
include $(SOURCES:.cpp=.d)
$(EXECUTABLE): $(SOURCES:.cpp=.o)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) $(INCLUDES) -o $@

clean: 
	rm -f $(SOURCES:.cpp=.d) $(SOURCES:.cpp=.o) $(EXECUTABLE)

.PHONY: clean
