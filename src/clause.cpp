#include "clause.h"
#include "edge.h"
#include "variable.h"

Clause::Clause(int id){
	this->id = id;
	this->enabled_edges = 0;
	this->satisfied = false;
}

void Clause::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
	enabled_edges++;
}

void Clause::satisfy(){
	satisfied = true;

	for(Edge* e : neighborhood)
		if(e -> enabled)
			e->Dissable();
}

vector<Edge*> Clause::enabledNeighborhood(){
	vector<Edge*> enabled_neigh;
	for(Edge* n : neighborhood)
		if(n->enabled)
			enabled_neigh.push_back(n);

	return enabled_neigh;
}

