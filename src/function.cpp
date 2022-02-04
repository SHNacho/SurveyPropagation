#include "function.h"
#include "edge.h"
#include "variable.h"

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
void Function::removeNeighbor(int var_id){
	bool found = false;
	int pos = 0;
	for(int i = 0; i < neighborhood.size() && !found; ++i){
		if(var_id == neighborhood[i]->getVariable()->getId()){
			neighborhood[i]->getVariable()->removeNeighbor(this->id);
			neighborhood.erase(neighborhood.begin() + pos);
			bool found = true;
		}
	}

}

//---------------------------------------//
void Function::removeNeighborhood(){
	//while(!(this->neighborhood.empty())){
	//	neighborhood[0]->getVariable()->removeNeighbor(this->id);
	//	this->neighborhood.erase(neighborhood.begin());
	//}
	for(int i = 0; i < neighborhood.size(); ++i){
		neighborhood[i]->getVariable()->removeNeighbor(this->id);
	}

	neighborhood.clear();
}
