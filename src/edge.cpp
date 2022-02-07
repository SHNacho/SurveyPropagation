#include "edge.h"
#include "variable.h"
#include "function.h"
#include "random.h"

Edge::Edge(Variable* variable, Function* function, bool negated){
	this->variable = variable;
	this->function = function;
	this->negated = negated;
	this->survey = Randfloat(0.0000, 1.0000);
	this->converged = false;
	this->enabled = true;
}

void Edge::setSurvey(double survey){
	this->survey = survey;
}

void Edge::setConverged(bool conv){
	converged = conv;
}

const double Edge::getSurvey(){
	return this->survey;
}

Variable* Edge::getVariable(){
	return this->variable;
}

Function* Edge::getFunction(){
	return this->function;
}

//TODO: Se puede optimizar ya que en cada iteración de SP_UPDATE
//		se llama a esta función para todas las aristas, por lo que 
//		podemos almacenar los subproductos en las variables
//		y no se repite el cálculo si dos aristas tienen la misma
//		variable.
//		Este cálculo no valdría para la siguiente llamada a 
//		SP_UPDATE
void Edge::calculateProducts(){
	double subproduct_u = 1.0,
		   subproduct_s = 1.0,
		   subproduct_0 = 1.0;
	
	// Calculamos los subproductos para la ecuación 26
	vector<Edge*> pos_neigh = this->variable->enabledPositiveNeighborhood();
	vector<Edge*> neg_neigh = this->variable->enabledNegativeNeighborhood();
	vector<Edge*> total_neigh = this->variable->enabledNeighborhood();
	
	// Si la variable está negada en la clausula
	if(negated){
		if(pos_neigh.size() > 0)
			for( Edge* neigh : pos_neigh )
				subproduct_u *= (1 - neigh->survey);
		else
			subproduct_u = 1.0;

		if(neg_neigh.size() > 0)
			for( Edge* neigh : neg_neigh )
				subproduct_s *= (1 - neigh->getSurvey());
		else
			subproduct_s = 1.0;
	}
	// Si la variable no está negada en la cláusula
	else{
		if(pos_neigh.size() > 0)
			for( Edge* neigh : pos_neigh )
				subproduct_s *= (1 - neigh->survey);
		else
			subproduct_s = 1.0;

		if(neg_neigh.size() > 0)
			for( Edge* neigh : neg_neigh )
				subproduct_u *= (1 - neigh->getSurvey());
		else
			subproduct_u = 1.0;
	}

	if(total_neigh.size() > 0){
		for( Edge* neigh : total_neigh ){
			subproduct_0 *= (1 - neigh->survey);
		}
	}else{
		subproduct_0 = 1;
	}

	// Asignamos el valor de los productos a cada variable
	variable->setPu((1-subproduct_u)*subproduct_s);
	variable->setPs((1-subproduct_s)*subproduct_u);
	variable->setP0(subproduct_0);
}
















