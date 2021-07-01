.PHONY := default run build test config clean justrun gdb gdb-gtk i18n-de

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

i18n-de:
	build/lib/wxrc -g -o .supp_messages resources/gui.xrc
	xgettext -C -n -k_ src/* include/* .supp_messages -p i18n/de -o pro_gramm.pot
	msgmerge -U i18n/de/pro_gramm.po i18n/de/pro_gramm.pot
	poedit i18n/de/pro_gramm.po

