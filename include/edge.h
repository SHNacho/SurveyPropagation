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
		// TODO
		void setSurvey(double survey);
		// TODO
		const double getSurvey();
		// TODO
		Variable* getVariable();
		// TODO
		Function* getFunction();
		// TODO
		void calculateProducts();
};

#endif //EDGE_H
