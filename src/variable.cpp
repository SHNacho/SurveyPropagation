#include "variable.h"
#include "function.h"
#include "edge.h"


Variable::Variable(int id){
	this->id = id;
	this->value = -1;
}

void Variable::setValue(int val){
	this->value = val;
	if(val == 0){
		for(Edge* e : negativeNeighborhood){
			Function* f = e->getFunction();
			f->satisfy();
		}
	} else if (val == 1) {
		for(Edge* e : positiveNeighborhood){
			Function* f = e->getFunction();
			f->satisfy();
		}
	}
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

double Variable::calculateBias(){

	double positive_subprod = 1.0,
		   negative_subprod = 1.0,
		   subprod 			= 1.0;

	double positive_aux = 0.0,
		   negative_aux = 0.0,
		   null_aux     = 0.0;

	for(Edge* neigh : this->positiveNeighborhood){
		positive_subprod *= (1 - neigh->getSurvey());
	}

	for(Edge* neigh : this->negativeNeighborhood){
		negative_subprod *= (1 - neigh->getSurvey());
	}

	for(Edge* neigh : this->neighborhood){
		subprod *= (1 - neigh->getSurvey());
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
		this->setValue(1);
	else
		this->setValue(0);
}

bool operator==(const Variable& lhs, const Variable& rhs){
	return lhs.id == rhs.id;
}
















