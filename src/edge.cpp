#include "edge.h"
#include "variable.h"
#include "function.h"

void Edge::setSurvey(double survey){
	this->survey = survey;
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

void Edge::calculateProducts(){

}
