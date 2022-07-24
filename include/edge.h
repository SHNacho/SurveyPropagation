#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <vector>

class Variable;
class Clause;

class Edge
{


	public:
		//==========================//
		// Declaración de variables //
		//==========================//
		Variable* variable;
		Clause* clause;

		bool negated;
		double survey;

		bool enabled;

		//==========================//
		// Declaración de funciones //
		//==========================//
		Edge(Variable* variable, Clause* clause, bool negated);
		void calculateProducts();
		inline void Dissable(){enabled = false;}
		void initRandomSurvey();
};

#endif //EDGE_H
