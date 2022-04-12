#include "variable.h"
#include "function.h"
#include "edge.h"
#include "cmath"


Variable::Variable(int id){
	this->id = id;
	this->value = false;
	this->assigned = false;
}

//---------------------------------------//
vector<Edge*> Variable::enabledNeighborhood(){
	vector<Edge*> enabled_neighborhood;
	for(Edge* e : neighborhood){
		if(e->isEnabled()){
			enabled_neighborhood.push_back(e);		
		}
	}
	return enabled_neighborhood;
}

//---------------------------------------//
vector<Edge*> Variable::enabledPositiveNeighborhood(){
	vector<Edge*> enabled_neighborhood;
	for(Edge* e : positiveNeighborhood){
		if(e->isEnabled()){
			enabled_neighborhood.push_back(e);		
		}
	}
	return enabled_neighborhood;
}

//---------------------------------------//
vector<Edge*> Variable::enabledNegativeNeighborhood(){
	vector<Edge*> enabled_neighborhood;
	for(Edge* e : negativeNeighborhood){
		if(e->isEnabled()){
			enabled_neighborhood.push_back(e);		
		}
	}
	return enabled_neighborhood;
}

//---------------------------------------//
void Variable::setValue(bool val){
	this->value = val;
	if(!value){
		for(Edge* e : negativeNeighborhood){
			Function* f = e->getFunction();
			f->satisfy();
		}
	} else if (value) {
		for(Edge* e : positiveNeighborhood){
			Function* f = e->getFunction();
			f->satisfy();
			//f->dissable();
		}
	}

	assigned = true;
}

//---------------------------------------//
void Variable::addNeighbor(Edge* neigh){
	this->neighborhood.push_back(neigh);
	
	if(neigh->isNegated())
		this->negativeNeighborhood.push_back(neigh);
	else
		this->positiveNeighborhood.push_back(neigh);
}

//---------------------------------------//
void Variable::removeNeighbor(int func_id){
	vector<Edge*>::iterator it;
	bool found = false;
	bool neg;

	if(neighborhood.size() > 0){
		for(it = neighborhood.begin(); it !=  neighborhood.end() && !found; it++){
			if((*it)->getFunction()->getId() == func_id){
				neg = (*it)->isNegated();
				neighborhood.erase(it);	
				found = true;
			}
		}
	}

	found = false;
	vector<Edge*>::iterator end_it;
	vector<Edge*>* neighborhood_ptr;

	if(neg){
		it = negativeNeighborhood.begin();
		end_it = negativeNeighborhood.end();
		neighborhood_ptr = &negativeNeighborhood;
	}else{
		it = positiveNeighborhood.begin();
		end_it = positiveNeighborhood.end();
		neighborhood_ptr = &positiveNeighborhood;
	}
	
	for(; it !=  end_it && !found; it++){
		if((*it)->getFunction()->getId() == func_id){
			neighborhood_ptr->erase(it);	
			found = true;
		}
	}
}

//---------------------------------------//
double Variable::calculateBias(){
	positiveBias = (1.0 - positive_prod) * negative_prod;
	negativeBias = (1.0 - negative_prod) * positive_prod;
	nullBias = positive_prod * negative_prod;

	double sum = positiveBias + negativeBias + nullBias;

	positiveBias = positiveBias / sum;
	negativeBias = negativeBias / sum;
	nullBias = 1 - positiveBias - negativeBias;

	return (positiveBias - negativeBias);
}

//---------------------------------------//
void Variable::fix(){
	if(positiveBias > negativeBias)
		this->setValue(true);
	else
		this->setValue(false);
}

//---------------------------------------//
bool compareVars(Variable* v1, Variable* v2){
	double v1_pbias = v1->getPosBias();
	double v1_nbias = v1->getNegBias();
	double v2_pbias = v2->getPosBias();
	double v2_nbias = v2->getNegBias();

	double v1_diff = fabs(v1_pbias - v1_nbias);
	double v2_diff = fabs(v2_pbias - v2_nbias);

	// Si v1 ya está asignada la mandamos al final
	if(v1->isAssigned()) return false;
	// Si v2 ya está asignada, v1 va delante
	if(v2->isAssigned()) return true;

	return (v1_diff > v2_diff);
}
















