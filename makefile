CXX := g++
AR := ar
ARFLAGS := crv
INCLUDE_PATH := -I ./src
CXXFLAGS := -Wall -std=c++17 -g

LIBDIR := build/lib
BINDIR := build/bin

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

.PHONY : all
all : jugg token_print

TARGET := core

$(TARGET) : $(OBJS)
	@echo "making core...";
	mkdir -p $(LIBDIR)/lib;
	$(AR) $(ARFLAGS) $(LIBDIR)/lib$(TARGET).a $^;

%.o : %.cpp
	$(CXX) -o $@ -c $< -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS)

-include $(DEPS)

jugg : tools/jugg.cc $(TARGET)
	@echo "making jugg...";
	mkdir -p $(BINDIR);
	$(CXX) -o $(BINDIR)/$@ $< $(INCLUDE_PATH) $(CXXFLAGS) -MMD -MP -L$(LIBDIR) -l$(TARGET);

token_print : tools/token_print.cc $(TARGET)
	@echo "making token_print";
	mkdir -p $(LIBDIR);
	$(CXX) -o $(BINDIR)/$@ $< $(INCLUDE_PATH) $(CXXFLAGS) -MMD -MP -L$(LIBDIR) -l$(TARGET);

.PHONY : clean
clean :
	@rm -rf $(OBJS) $(DEPS) build
