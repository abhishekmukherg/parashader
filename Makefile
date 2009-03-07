all:
	cmake -DCMAKE_BUILD_TYPE=Release .
	make
	cp objects/* .
	ln -s src/render

unix: all
cygwin_lib: all
cygwin_a: all
cygwin_x: all
osx: all

clean:
	for i in objects/*; do rm -f $(basename $i); done
	rm -f render

.PHONY: all clean
