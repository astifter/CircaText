#!/bin/bash
set -ex

pebble build
pushd test; ./german_fuzzy_text_test.sh; popd;
if [[ -n "$1" ]]; then
echo -n "Press [Enter] to install, [Ctrl-C] to exit."; read -n 1 -s;
pebble install --phone "$1"
pebble logs --phone "$1"
fi
