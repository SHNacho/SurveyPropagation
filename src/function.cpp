#include "function.h"
#include "edge.h"

Function::Function(int id){
	this->id = id;
}

vector<Edge*> Function::getNeighborhood(){
	return neighborhood;
}

void Function::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
}
