#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <vector>

class Variable;
class Function;

class Edge
{
	private:
		Variable* variable;
		Function* function;

		bool negated;
		double survey;
		bool enabled;
		bool converged;

	public:
		Edge(Variable* variable, Function* function, bool negated);
		void setSurvey(double survey);
		void setConverged(bool conv);
		bool hasConverged(){return converged;}
		const double getSurvey();
		const bool isNegated(){return negated;}
		const bool isEnabled(){return enabled;}
		void dissable(){enabled = false;}
		Variable* getVariable();
		Function* getFunction();
		void calculateProducts();
	private:
		void initRandomSurvey();
};

#endif //EDGE_H
