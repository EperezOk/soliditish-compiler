#! /bin/bash

set -e
ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT/.."

# Manda por stdin el contenido del archivo al compilador
cat "$1" | ./bin/Compiler
