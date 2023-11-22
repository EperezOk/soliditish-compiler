#! /bin/bash

set -e
ROOT="$(dirname "$0")"

cd "$ROOT/.."

# Manda por stdin el contenido del archivo al compilador
cat "$1" | ./bin/Compiler "$2" "$3" "$4" "$5" "$6" "$7" "$8" "$9"
