CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = daytime_client

all: $(TARGET)

$(TARGET): daytime_client.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) daytime_client.o

daytime_client.o: daytime_client.cpp
	$(CXX) $(CXXFLAGS) -c daytime_client.cpp

clean:
	rm -f *.o $(TARGET)


