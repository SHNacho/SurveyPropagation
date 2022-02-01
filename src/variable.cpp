#include "variable.h"


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

// TODO
double Variable::calculateBias(){
	return 0.0;
}

