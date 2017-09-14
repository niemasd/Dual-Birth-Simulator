# use g++ compiler with C++11 support
CXX=g++
CXXFLAGS=-Wall -pedantic -O3
TOOLS=dualbirth yule

# compile all tools
all: $(TOOLS)

# dualbirth: Simulate trees under the Dual-Birth model in O(n)
dualbirth: dualbirth.cpp
	$(CXX) $(CXXFLAGS) -o dualbirth dualbirth.cpp

# yule: Simulate trees under the Yule model in O(n)
yule: yule.cpp
	$(CXX) $(CXXFLAGS) -o yule yule.cpp

# remove all compiled files
clean:
	$(RM) $(TOOLS) *.o
