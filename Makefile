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
	GTK_THEME=Sweet-Mars build/form_lang gui # >/dev/null

gdb:
	gdb --args build/form_lang gui

gdb-gtk:
	G_DEBUG=fatal-warnings gdb --args build/form_lang gui

