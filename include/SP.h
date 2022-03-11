#ifndef SP_H
#define SP_H

#include <iostream>
#include <cmath>
#include <vector>
#include "graph.h"

enum result { 
	SP_UNCONVERGED, 
	SP_CONVERGED,
	CONTRADICTION,
	NO_CONTRADICTION
};

class SolverSP{

	public:
		int totalIt;

		result SID(Graph* graph, int t_max, float precision, float f);
		result surveyPropagation(Graph* grafo, int t_max, float precision);	
		double SP_UPDATE(Edge* edge);
		void calculateProducts();
		bool clean(Variable* var);
		bool unitPropagation(Function* clause);
		void computeBias(Variable* var);
		bool assignVariable(Variable* var, lbool value);

};
#endif //SP_H
