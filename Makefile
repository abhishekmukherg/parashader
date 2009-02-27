# CSCI-4530/6530 Advanced Computer Graphics, Spring 2009
# Simple Makefile for Homework 3
# for g++ on unix or cygwin platforms

SHELL = /bin/bash

# ===============================================================

default:
	@echo "Please specify a target (unix, cygwin_lib, cygwin_a, cygwin_x, or osx)"

# Linux or FreeBSD
unix: CC                = g++ -g -O3 -Wall -pedantic
unix: INCLUDE_PATH      = -I/usr/X11R6/include -L/usr/local/include
unix: LIB_PATH          = -L/usr/X11R6/lib -L/usr/local/lib
unix: LIBS              = -lm -lGL -lGLU -lglut
unix: all

# Cygwin with native Windows libraries, using .lib files from vstudio and glut download
cygwin_lib: VCLIB_PATH      = /c/PROGRA~1/VSNET.2K3/Vc7/PlatformSDK/Lib
cygwin_lib: GLUTLIB_PATH    = /c/GLUT
cygwin_lib: CC              = g++ -g -O3 -Wall -pedantic
cygwin_lib: INCLUDE_PATH    = -I/usr/include/w32api -I/usr/include/opengl
cygwin_lib: LIB_PATH        = -L/usr/lib/w32api
cygwin_lib: LIBS            = -lm ${VCLIB_PATH}/OPENGL32.LIB ${VCLIB_PATH}/GLU32.LIB ${GLUTLIB_PATH}/glut32.lib
cygwin_lib: all

# Cygwin with native Windows libraries
cygwin_a: CC              = g++ -g -O3 -Wall -pedantic
cygwin_a: INCLUDE_PATH    = -I/usr/include/w32api -I/usr/include/opengl
cygwin_a: LIB_PATH        = -L/usr/lib/w32api
cygwin_a: LIBS            = -lm -lopengl32 -lglu32 -lglut32 
cygwin_a: all

# Cygwin with X (probably software emulation)
cygwin_x: CC            = g++ -g -O3 -Wall -pedantic
cygwin_x: INCLUDE_PATH  = -I/usr/X11R6/include -I/usr/include/opengl
cygwin_x: LIB_PATH      = -L/usr/X11R6/lib
cygwin_x: LIBS          = -lm -lGL -lglu -lglut
cygwin_x: all

# Mac OS X
osx: CC			= g++ -g -Wall -pedantic
osx: LIBS		= -framework GLUT -framework OpenGL
osx: all

# ===============================================================

SRCS	= main.cpp matrix.cpp camera.cpp glCanvas.cpp mesh.cpp edge.cpp radiosity.cpp face.cpp raytree.cpp raytracer.cpp sphere.cpp material.cpp image.cpp
OBJS	= $(SRCS:.cpp=.o)
EXE	= render

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE) 

depend:
	$(CC) $(INCLUDE_PATH) -E -M $(SRCS) > Makefile.depend

clean:
	rm -f *~ *bak *.o  $(EXE) $(EXE).exe Makefile.depend

# ===============================================================
# compilation rules

$(EXE): Makefile $(OBJS)
	$(CC) $(INCLUDE_PATH) -o $@ $(OBJS) $(LIB_PATH) $(LIBS) 

.cpp.o: Makefile
	$(CC) $(INCLUDE_PATH) $< -c -o $@

# ===============================================================

-include Makefile.depend
