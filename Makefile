DEFINES = -O2  -march=native -std=c++0x  -Wall
#DEFINES = -g -std=c++0x 
CFLAGS = -Wall $(DEFINES) 
CXX = g++
INCLUDE = include
OBJ = obj
SRC = src
BIN = bin
LINKER = g++

all : doc_view.o sample.o test1.o test.o scope_connection.o chain.o auto_delete.o
	$(LINKER) -o $(BIN)/doc_view $(OBJ)/doc_view.o 
	$(LINKER) -o $(BIN)/sample $(OBJ)/sample.o
	$(LINKER) -o $(BIN)/test1 $(OBJ)/test1.o
	$(LINKER) -o $(BIN)/test $(OBJ)/test.o
	$(LINKER) -o $(BIN)/scope_connection $(OBJ)/scope_connection.o
	$(LINKER) -o $(BIN)/chain $(OBJ)/chain.o
	$(LINKER) -o $(BIN)/auto_delete $(OBJ)/auto_delete.o

doc_view.o : $(SRC)/doc_view.cpp
	$(CXX) -c $(SRC)/doc_view.cpp -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/doc_view.o 
sample.o : $(SRC)/sample.cc
	$(CXX) -c $(SRC)/sample.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/sample.o 
test1.o : $(SRC)/test1.cc
	$(CXX) -c $(SRC)/test1.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/test1.o 
test.o : $(SRC)/test.cc
	$(CXX) -c $(SRC)/test.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/test.o 
scope_connection.o : $(SRC)/scope_connection.cc
	$(CXX) -c $(SRC)/scope_connection.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/scope_connection.o 
chain.o : $(SRC)/chain.cc
	$(CXX) -c $(SRC)/chain.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/chain.o 
auto_delete.o : $(SRC)/auto_delete.cc
	$(CXX) -c $(SRC)/auto_delete.cc -I$(INCLUDE) $(CFLAGS) -o $(OBJ)/auto_delete.o 

.PHONY : clean 
clean:
	rm -rf $(BIN)/* 
	rm -rf $(OBJ)/*
