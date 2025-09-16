#!/bin/bash
set -e
mkdir -p build
cmake -S . -B build
echo ""
echo "-------------- Building project --------------"
echo ""
cmake --build build
echo ""
echo "-------------- Running tests 🧪 --------------"
echo ""
cd build
ctest --output-on-failure
echo ""
echo "✅ All tests finished."