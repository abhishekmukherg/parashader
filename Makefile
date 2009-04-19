all:
	$(MAKE) -C src

unix: all
cygwin_lib: all
cygwin_a: all
cygwin_x: all
osx: all

clean:
	$(MAKE) clean -C src

.PHONY: all clean
