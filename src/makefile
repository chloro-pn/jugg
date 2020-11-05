CXX := g++
INCLUDE_PATH := -I .
CXXFLAGS := -Wall -std=c++17 -g

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

TARGET := jugg

$(TARGET) : $(OBJS)
	@echo "making jugg...";
	$(CXX) -o $@ $^ $(CXXFLAGS) 

%.o : %.cpp
	$(CXX) -o $@ -c $< -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS)

-include $(DEPS)

.PHONY : clean
clean :
	@rm -rf $(OBJS) $(DEPS) $(TARGET)
