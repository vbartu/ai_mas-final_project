CC = g++

# C++ compiler option
#CXXFLAGS = -Wall -O2
CXXFLAGS =

# linker option
LDFLAGS =

# src file dir
SRC_DIR = ${CURDIR}

# obj file dir
OBJ_DIR = ${CURDIR}/build

# exec file name
TARGET = main

# Extract *.cpp in SRC_DIR with wildcard
# Get only file names with notdir
SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(SRCS:.cpp=.o)

OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: main
	@echo "Finished: main"

$(OBJ_DIR) : 
	@mkdir -p $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(OBJ_DIR)
	@$(CC) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS)
	@echo "Compiling $(notdir $<)"

$(TARGET) : $(OBJECTS)
	@$(CC) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all run
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Project cleared"

run: all
	java -jar server/server.jar -c ./main -l server/levels/MAPF01.lvl -g


-include $(DEPS)
