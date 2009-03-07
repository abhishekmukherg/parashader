all:
	cmake -DCMAKE_BUILD_TYPE=Release .
	make
	cp objects/* .
	ln -s src/render

.PHONY: all
