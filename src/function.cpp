#include "function.h"
#include "edge.h"
#include "variable.h"

Function::Function(int id){
	this->id = id;
	this->enabled_edges = 0;
	this->satisfied = false;
}

void Function::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
	enabled_edges++;
}

void Function::satisfy(){
	satisfied = true;

	for(Edge* e : neighborhood)
		if(e -> enabled)
			e->Dissable();
}

vector<Edge*> Function::enabledNeighborhood(){
	vector<Edge*> enabled_neigh;
	for(Edge* n : neighborhood)
		if(n->enabled)
			enabled_neigh.push_back(n);

	return enabled_neigh;
}

