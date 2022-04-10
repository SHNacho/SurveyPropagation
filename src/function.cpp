#include "function.h"
#include "edge.h"
#include "variable.h"

Function::Function(int id){
	this->id = id;
	this->enabled_edges = 0;
	this->satisfied = false;
}

//---------------------------------------//
Function::~Function(){
	for(Edge* e : neighborhood){
		delete e;
	}
	neighborhood.clear();
}

//---------------------------------------//
void Function::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
	enabled_edges++;
}

//---------------------------------------//
vector<Edge*> Function::getEnabledNeighborhood(){
	vector<Edge*> v_enabled;
	for(Edge* e : neighborhood){
		if(e->isEnabled()){
			v_enabled.push_back(e);
		}
	}
	
	return v_enabled;
}

//---------------------------------------//
void Function::satisfy(){
	for(Edge* e : neighborhood){
		if(e->isEnabled())
			e->dissable();
	}
	satisfied = true;
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
			this->enabled_edges--;
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
