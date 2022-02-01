#include "variable.h"
#include "edge.h"


Variable::Variable(int id){
	this->id = id;
}

vector<Edge*> Variable::getNeighborhood(){
	return neighborhood;
}

vector<Edge*> Variable::getPosNeighborhood(){
	return positiveNeighborhood;
}

vector<Edge*> Variable::getNegNeighborhood(){
	return negativeNeighborhood;
}

void Variable::setP0(double p0){
	this->p0 = p0;
}

void Variable::setPs(double ps){
	this->ps = ps;
}

void Variable::setPu(double pu){
	this->pu = pu;
}

void Variable::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
	
	if(neigh->isNegated())
		this->negativeNeighborhood.push_back(neigh);
	else
		this->positiveNeighborhood.push_back(neigh);
}

// TODO
double Variable::calculateBias(){
	return 0.0;
}

