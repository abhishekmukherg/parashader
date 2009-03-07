all:
	cmake -DCMAKE_BUILD_TYPE=Release .
	make
	cp objects/* .
	ln -s src/render

clean:
	for i in objects/*; do rm -f $(basename $i); done
	rm -f render

.PHONY: all clean
