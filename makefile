MAINPROG=game
CC=gcc
CXX=g++
CPPFLAGS=-g -std=c++17
LFLAGS=
CFLAGS=-g
VPATH = src
BUILDDIR = build
TESTDIR = test
TARGET=$(BUILDDIR)/$(MAINPROG)
CPPS=$(wildcard $(VPATH)/*.cpp)
LINK=g++ $(CPPFLAGS)
OBJS=$(CPPS:%.cpp=%.o)




all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -MMD -o $@ -c $(VPATH)/*.cpp

$(TARGET): $(OBJS)
	$(LINK) $(FLAGS) -o $(TARGET) $^ $(LFLAGS)

run:
	./$(TARGET)

clean:
	-/bin/rm -rf *.d *.o $(TARGET)
