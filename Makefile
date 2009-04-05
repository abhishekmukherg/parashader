OBJECTS=cornell_box_diffuse_sphere.obj cornell_box.obj cornell_box_reflective_spheres.obj cornell_box_texture.obj glossy_sphere.obj green_mosaic.ppm l.obj reflective_spheres.obj rocks.ppm
all:
	cmake -DCMAKE_BUILD_TYPE=Release .
	$(MAKE)
	cp objects/* .
	ln -s src/render

unix: all
cygwin_lib: all
cygwin_a: all
cygwin_x: all
osx: all

clean:
	rm -f ${OBJECTS}
	rm -f render

.PHONY: all clean
