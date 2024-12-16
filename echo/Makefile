CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = echo_client

all: $(TARGET)

$(TARGET): echo_client.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) echo_client.o

echo_client.o: echo_client.cpp
	$(CXX) $(CXXFLAGS) -c echo_client.cpp

clean:
	rm -f *.o $(TARGET)




