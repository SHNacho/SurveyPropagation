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
}

//--------------------------------//
void Edge::initRandomSurvey(){
	survey = Randfloat(0.0, 1.0);
}

//void Edge::calculateProducts(){
//	double subproduct_u = 1.0,
//		   subproduct_s = 1.0,
//		   subproduct_0 = 1.0;
//	
//	// Calculamos los subproductos para la ecuación 26
//	vector<Edge*> pos_neigh = this->variable->ena;
//	vector<Edge*> neg_neigh = this->variable->getNegNeighborhood();
//	vector<Edge*> total_neigh = this->variable->getNeighborhood();
//	
//	// Para el vecindario positivo
//	if(pos_neigh.size() > 0){
//		for( Edge* neigh : pos_neigh ){
//			// Si la variable está negada en la cláusula
//			if(negated)
//				// Calculamos el producto (V(a,u))
//				subproduct_u *= (1 - neigh->survey);
//			// Si no está negada
//			else
//				// Calculamos el producto (V(a,s))
//				subproduct_s *= (1 - neigh->survey);	
//		}
//	// Si no hay vecindario los productos valen 1
//	} else{
//		subproduct_s = 1;
//		subproduct_u = 1;
//	}
//		
//	// Para el vecindario negativo
//	if(neg_neigh.size() > 0){
//		for( Edge* neigh : neg_neigh ){
//			// Si la variable está negada en la cláusula
//			if(negated)
//				// Calculamos el producto (V(a,s))
//				subproduct_s *= (1 - neigh->survey);
//			// Si no está negada
//			else
//				// Calculamos el producto (V(a,u))
//				subproduct_u *= (1 - neigh->survey);	
//		}
//	// Si no hay vecindario, los productos valen 1
//	}else{
//		subproduct_s = 1;
//		subproduct_u = 1;
//	}
//	
//	if(total_neigh.size() > 0){
//		for( Edge* neigh : total_neigh ){
//			subproduct_0 *= (1 - neigh->survey);
//		}
//	}else{
//		subproduct_0 = 1;
//	}
//
//	// Asignamos el valor de los productos a cada variable
//	variable->pu = (1-subproduct_u)*subproduct_s;
//	variable->ps = (1-subproduct_s)*subproduct_u;
//	variable->p0 = subproduct_0;
//}
















