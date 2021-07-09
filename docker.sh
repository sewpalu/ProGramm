#!/bin/sh

touch grammars.json

docker run \
  -p 5900 \
  -v "$PWD/i18n:/root/i18n" \
  -v "$PWD/grammars.json:/root/grammars.json" \
  -v "$PWD/resources:/root/resources" \
  -it \
  pro_gramm $@

