.PHONY := run build config test

default: build

run: build
	build/form_lang gui

build: config
	$(MAKE) -C build

test: build
	$(MAKE) -C build test

config:
	mkdir -p build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

clean:
	rm -rf build

justrun:
	build/form_lang gui >/dev/null

