.PHONY := run build

run: build
	build/form_lang

build: build/Makefile
	$(MAKE) -C build

build/Makefile:
	mkdir -p build
	cd build && cmake ..

