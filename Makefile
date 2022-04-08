INC=./include
SRC=./src
BIN=./bin
OBJ=./obj

OPT=-Wall -g -std=c++1z #-O2 -fopenmp

build: $(BIN)/main_exe 

all: clean build

$(BIN)/main_exe: $(OBJ)/main.o $(OBJ)/variable.o $(OBJ)/function.o $(OBJ)/edge.o $(OBJ)/graph.o $(OBJ)/SP.o $(OBJ)/random.o
	g++ $(OPT) -g -I$(INC) $^ -o $@ 

$(OBJ)/main.o: $(SRC)/main.cpp $(INC)/SP.h 
	g++ $(OPT) -g -I$(INC) -c $< -o $@ 

$(OBJ)/SP.o: $(SRC)/SP.cpp $(INC)/graph.h 
	g++ $(OPT) -g -I$(INC) -c $< -o $@ 

$(OBJ)/graph.o: $(SRC)/graph.cpp $(INC)/variable.h $(INC)/function.h $(INC)/edge.h
	g++ $(OPT)  -g -I$(INC) -c $< -o $@ 

$(OBJ)/edge.o: $(SRC)/edge.cpp $(INC)/variable.h $(INC)/function.h
	g++ $(OPT) -g -I$(INC) -c $< -o $@ 

$(OBJ)/function.o: $(SRC)/function.cpp $(INC)/variable.h
	g++ $(OPT) -g -I$(INC) -c $< -o $@ 

$(OBJ)/variable.o: $(SRC)/variable.cpp $(INC)/function.h $(INC)/edge.h
	g++ $(OPT) -g  -I$(INC) -c $< -o $@ 

$(OBJ)/random.o: $(SRC)/random.cpp 
	g++ $(OPT) -g  -I$(INC) -c $< -o $@ 

clean:
	rm -rf $(BIN)/* $(OBJ)/*
