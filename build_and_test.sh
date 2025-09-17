#!/bin/bash
set -e
mkdir -p build

cmake -S . -B build

echo ""
echo "-------------- Building project --------------"
echo ""
cmake --build build -j$(nproc)


echo ""
echo "-------------- Running tests 🧪 --------------"
echo ""
cd build
ctest --output-on-failure
cd ..


echo ""
echo "✅ All tests finished."
echo "Run the example bin using ./bin/example"