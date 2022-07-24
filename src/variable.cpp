#include "variable.h"
#include "clause.h"
#include "edge.h"


Variable::Variable(int id){
	this->id = id;
	this->value = Unassigned;
}

void Variable::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
	
	if(neigh->negated)
		this->negativeNeighborhood.push_back(neigh);
	else
		this->positiveNeighborhood.push_back(neigh);
}

vector<Edge*> Variable::enabledNeighborhood(){
	vector<Edge*> enabled_neighborhood;

	for(Edge * n : this->neighborhood)
		if(n->enabled)
			enabled_neighborhood.push_back(n);

	return enabled_neighborhood;
}

void Variable::setValue(const lbool value){
	this->value = value;	
}

void Variable::setValue(const bool value){
	lbool val = value ? True : False;
	this->value = val;
}

double Variable::calculateBias(){

	double positive_subprod = 1.0,
		   negative_subprod = 1.0,
		   subprod 			= 1.0;

	double positive_aux = 0.0,
		   negative_aux = 0.0,
		   null_aux     = 0.0;

	for(Edge* neigh : this->positiveNeighborhood){
		positive_subprod *= (1 - neigh->survey);
	}

	for(Edge* neigh : this->negativeNeighborhood){
		negative_subprod *= (1 - neigh->survey);
	}

	for(Edge* neigh : this->neighborhood){
		subprod *= (1 - neigh->survey);
	}

	positive_aux = (1 - positive_subprod) * negative_subprod;
	negative_aux = (1 - negative_subprod) * positive_subprod;
	null_aux     = subprod;

	positiveBias = positive_aux / (positive_aux + negative_aux + null_aux);
	negativeBias = negative_aux / (positive_aux + negative_aux + null_aux);
	nullBias     = 1 - positiveBias - negativeBias;

	return (positiveBias - negativeBias);
}

void Variable::fix(){
	if(positiveBias > negativeBias)
		this->value = True;
	else
		this->value = False;
}

bool operator==(const Variable& lhs, const Variable& rhs){
	return lhs.id == rhs.id;
}

bool compareVars(Variable* v1, Variable* v2){
	double v1_pbias = v1->positiveBias;
	double v1_nbias = v1->negativeBias;
	double v2_pbias = v2->positiveBias;
	double v2_nbias = v2->negativeBias;

	double v1_diff = abs(v1_pbias - v1_nbias);
	double v2_diff = abs(v2_pbias - v2_nbias);

	// Si v1 ya está asignada la mandamos al final
	if(v1->value != Unassigned)return false;
	// Si v2 ya está asignada, v1 va delante
	if(v2->value != Unassigned) return true;

	return (v1_diff > v2_diff);
}















