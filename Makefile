DEFINES = -O2  -march=native -std=c++0x 
CFLAGS = -Wall $(DEFINES) 
CXX = g++
INCLUDE = include
OBJ = obj
SRC = src
BIN = bin
LINKER = g++

all : doc_view.o sample.o test1.o test.o
	$(LINKER) -o $(BIN)/doc_view $(OBJ)/doc_view.o 
	$(LINKER) -o $(BIN)/sample $(OBJ)/sample.o
	$(LINKER) -o $(BIN)/test1 $(OBJ)/test1.o
	$(LINKER) -o $(BIN)/test $(OBJ)/test.o

doc_view.o : $(SRC)/doc_view.cpp
	$(CXX) -c $(SRC)/doc_view.cpp -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/doc_view.o 
sample.o : $(SRC)/sample.cc
	$(CXX) -c $(SRC)/sample.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/sample.o 
test1.o : $(SRC)/test1.cc
	$(CXX) -c $(SRC)/test1.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/test1.o 
test.o : $(SRC)/test.cc
	$(CXX) -c $(SRC)/test.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/test.o 

.PHONY : clean 
clean:
	rm -rf $(BIN)/* 
	rm -rf $(OBJ)/*
