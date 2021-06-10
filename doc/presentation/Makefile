.PHONY: build run

build: presentation.adoc
	npx asciidoctor-revealjs presentation.adoc

run: build
	ruby -run -e httpd . -p 5000 -b 127.0.0.1 &
