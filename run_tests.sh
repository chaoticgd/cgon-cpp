set -e
cd test
g++ $(find . -name "*.cpp") -o runtest -std=c++17 -I../include -lgtest -lgtest_main -lpthread
./runtest
rm runtest