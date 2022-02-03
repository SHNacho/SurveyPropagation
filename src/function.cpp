#include "function.h"
#include "edge.h"

Function::Function(int id){
	this->id = id;
}

Function::~Function(){
	for(Edge* e : neighborhood){
		delete e;
	}
	neighborhood.clear();
}

vector<Edge*> Function::getNeighborhood(){
	return neighborhood;
}

void Function::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
}

//---------------------------------------//
void Function::removeNeighbor(Edge* neigh){
	bool found = false;
	int pos = 0;
	for(int i = 0; i < neighborhood.size() && !found; ++i){
		if(neigh == neighborhood[i]){
			pos = i;
			found = true;
		}
	}

	neighborhood.erase(neighborhood.begin() + pos);
}
