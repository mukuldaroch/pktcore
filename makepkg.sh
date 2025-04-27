#!/bin/bash
set -e

echo "Cleaning previous build..."
rm -rf build

echo "Configuring project with CMake..."
cmake -S . -B build

echo "Building project..."
cmake --build build

echo "moving pktcore to test dirctory"
mv build/pktcore test
