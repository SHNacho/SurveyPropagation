#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <vector>

class Variable;
class Function;

class Edge
{


	public:
		//==========================//
		// Declaración de variables //
		//==========================//
		Variable* variable;
		Function* function;

		bool negated;
		double survey;
		bool converged;

		bool enabled;

		//==========================//
		// Declaración de funciones //
		//==========================//
		Edge(Variable* variable, Function* function, bool negated);
		void calculateProducts();
		inline void Dissable(){enabled = false;}
	private:
		void initRandomSurvey();
};

#endif //EDGE_H
