all:
	g++ main.cpp -I/usr/local/include -L/usr/local/lib -lboost_system -lgmp -lgmpxx -I/home/emironenko/boost/boost-mt/linux-x86_64-gcc-release/include -L/home/emironenko/boost/boost-mt/linux-x86_64-gcc-release/lib -lpthread -std=c++11
