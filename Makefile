.PHONY := default run build test config clean justrun gdb gdb-gtk i18n-de package

default: build

run: build
	build/pro_gramm gui

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
	build/pro_gramm gui # >/dev/null

gdb:
	gdb --args build/pro_gramm gui

gdb-gtk:
	G_DEBUG=fatal-warnings gdb --args build/pro_gramm gui

i18n-de:
	build/lib/wxrc -g -o .supp_messages resources/gui.xrc
	xgettext -C -n -k_ src/* include/* .supp_messages -p i18n/de -o pro_gramm.pot
	msgmerge -U i18n/de/pro_gramm.po i18n/de/pro_gramm.pot
	poedit i18n/de/pro_gramm.po

package: package_

package_:
	ln -s package pro_gramm
	rm -f pro_gramm.zip
	zip -r pro_gramm.zip pro_gramm
	rm pro_gramm

