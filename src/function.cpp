#include "function.h"
#include "edge.h"

Function::Function(int id){
	this->id = id;
}

vector<Edge*> Function::getNeighborhood(){
	return neighborhood;
}
