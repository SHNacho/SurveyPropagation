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

	public:
		void setSurvey(double survey);
		const double getSurvey();
		Variable* getVariable();
		Function* getFunction();
		// TODO
		void calculateProducts();
};

#endif //EDGE_H
