#
# File   : Makefile
# Author : Jiayuan Mao
# Email  : mjy14@mails.tsinghua.edu.cn
# Date   : 2016-01-02 18:23:12
# This file is part of the school project RayTracing of course
# ``Advanced Computational Geometry''.
# This file is a output portable Makefile for simple c++ project.

SRC_DIR = src
INC_DIR = include
OBJ_DIR = build/obj
BIN_DIR = build/bin
LIB_DIR = build/lib
TARGET = diorama

CXX = $(ENVIRONMENT_OPTIONS) g++
#CXX = clang++

BIN_TARGET = $(BIN_DIR)/$(TARGET)
#PROF_FILE = $(BIN_TARGET).prof

INCLUDE_DIR = -I $(SRC_DIR) -I $(SRC_DIR)/include -I $(INC_DIR)

CXXFLAGS = -O3 -w
# CXXFLAGS = -O3 -w -fopenmp
# CXXFLAGS = -g
# CXXFLAGS = -pg
# CXXFLAGS += -shared
# CXXFLAGS += -fPIC

#CXXFLAGS += $(DEFINES)
CXXFLAGS += -std=c++11
#CXXFLAGS += -ansi
#CXXFLAGS += -Wall -Wextra
CXXFLAGS += $(INCLUDE_DIR)
#CXXFLAGS += $(LDFLAGS)
#CXXFLAGS += $(shell pkg-config --libs --cflags opencv)
#CXXFLAGS += -pthread -lpthread
#CXXFLAGS += -lopencv_core -lopencv_highgui -lopencv_imgproc
#CXXFLAGS += `pkg-config --libs --cflags hdf5` -lhdf5_hl -lhdf5_cpp

CXXSOURCES = $(shell find $(SRC_DIR)/ -name "*.cpp")
OBJFILES = $(addprefix $(OBJ_DIR)/,$(CXXSOURCES:.cpp=.o))
DEPFILES = $(OBJFILES:.o=.d)

.PHONY: all clean run rebuild gdb

all: $(BIN_TARGET)

$(LIB_DIR):
	mkdir $(LIB_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@echo "[CC] $< ..."
	@$(CXX) -c $< $(CXXFLAGS) -o $@

$(OBJ_DIR)/%.d: %.cpp
	@mkdir -pv $(dir $@)
	@echo "[dep] $< ..."
	@$(CXX) $(INCLUDE_DIR) $(CXXFLAGS) -MM -MT "$(OBJ_DIR)/$(<:.cpp=.o) $(OBJ_DIR)/$(<:.cpp=.d)" "$<" > "$@"

sinclude $(DEPFILES)

$(BIN_TARGET): $(OBJFILES)
	@echo "[link] $< ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJFILES) -o $@ $(CXXFLAGS)
	@echo have a nice day!

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

run: $(BIN_TARGET)
	./$(BIN_TARGET)

rebuild:
	+@make clean
	+@make

gdb: $(BIN_TARGET)
	gdb ./$(BIN_TARGET)

