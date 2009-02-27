SET(RENDER_SRCS
	camera.cpp
	edge.cpp
	face.cpp
	glCanvas.cpp
	image.cpp
	main.cpp
	material.cpp
	matrix.cpp
	mesh.cpp
	radiosity.cpp
	raytracer.cpp
	raytree.cpp
	sphere.cpp
	)

FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OpenGL_INCLUDE_DIRS})
SET(LIBS ${LIBS} ${OpenGL_LIBRARIES})

FIND_PACKAGE(GLUT REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIRS})
SET(LIBS ${LIBS} ${GLUT_LIBRARIES})

ADD_EXECUTABLE(render ${RENDER_SRCS})
TARGET_LINK_LIBRARIES(render ${LIBS})
