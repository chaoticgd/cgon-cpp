set -e
cd tests
g++ $(find . -name "*.cpp") -o runtest -std=c++17 -I../include -lgtest -lgtest_main -lpthread -Wall
./runtest
rm runtest
