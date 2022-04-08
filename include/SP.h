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
	NO_CONTRADICTION,
	SAT,
	WALKSAT
};

class SolverSP{

	public:
		int totalIt;
		const double ZERO = 0.0000000001;

		SolverSP();

		result SID(Graph* graph, int t_max, float precision, float f);
		result surveyPropagation(Graph* grafo, int t_max, float precision);	
		double SP_UPDATE(Function* clause);
		void calculateProducts();
		bool clean(Variable* var);
		bool unitPropagation(Function* clause);
		void computeBias(Variable* var);
		bool assignVariable(Variable* var, lbool value);
		/**
		 * Para cada variable i del grafo, calcula los productos
		 * Pi(1 - eta->j), j in V+(i)
		 * Pi(1 - eta->j), j in V-(i)
		 **/
		void computeSubProducts(Graph* graph);
		void randomInit(Graph* graph);

};
#endif //SP_H
