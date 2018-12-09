set -e
cd examples/$1
g++ $1.cpp -o $1 -std=c++17 -I../../include
./$1
