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
		bool converged;

	public:
		Edge(Variable* variable, Function* function, bool negated);
		void setSurvey(double survey);
		void setConverged(bool conv);
		bool hasConverged(){return converged;}
		const double getSurvey();
		const bool isNegated(){return negated;};
		Variable* getVariable();
		Function* getFunction();
		void calculateProducts();
		// TODO
		~Edge();
	private:
		void initRandomSurvey();
};

#endif //EDGE_H
