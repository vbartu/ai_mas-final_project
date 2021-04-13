CC = g++

# C++ compiler option
CXXFLAGS = -Wall -O2

# linker option
LDFLAGS =

# src file dir
SRC_DIR = ${CURDIR}

# obj file dir
OBJ_DIR = ${CURDIR}

# exec file name
TARGET = main

# Extract *.cpp in SRC_DIR with wildcard
# Get only file names with notdir
SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(SRCS:.cpp=.o)

OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: main

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET) *.o *.d

-include $(DEPS)

