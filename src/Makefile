all: render

%.d: %.cpp
	rm -f $@; \
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


SOURCES = edge.cpp face.cpp image.cpp main.cpp material.cpp matrix.cpp mesh.cpp raytracer.cpp sphere.cpp controller.cpp
EXECUTABLE = render
include $(SOURCES:.cpp=.d)
$(EXECUTABLE): $(SOURCES:.cpp=.o)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

SOURCES_PPM = output.cpp test_ppm.cpp
EXECUTABLE = ppm
include $(SOURCES_PPM:.cpp=.d)
$(EXECUTABLE): $(SOURCES_PPM:.cpp=.o)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean: 
	rm -f $(SOURCES:.cpp=.d) $(SOURCES:.cpp=.o) $(EXECUTABLE)

.PHONY: clean
