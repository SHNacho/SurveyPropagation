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
	SAT
};

class SolverSP{

	public:
		const int ZERO = 1e-6;

		bool SID(Graph* graph, int t_max, float precision, float f);
		bool surveyPropagation(Graph* grafo, int t_max, float precision, int & totalIt);
		bool assignVar(Graph* fg, Variable* var, bool val);
		double SP_UPDATE(Edge* edge);
		result unitPropagation(Graph* graph, Function* clause);
		void calculateProducts(Graph* graph);
		void updateOldSurvey(Graph* graph);
		void randomInit(Graph* graph);
		/**
		 * Dada una variable, la cual ha sido fijada a un valor
		 * concreto, elimina del grafo todas las aristas que
		 * contengan dicha variable así como las aristas que 
		 * contengan clausulas que contengan la variable.
		 * Además elimina los vecinos de las cláusulas satisfechas
		 * y elimina solo el vecino que apunta a la variable de las
		 * cláusulas no satisfechas
		 */
		bool clean(Graph* graph, Variable* var);
		/**
		 * @brief Calcula los suproductos Pi(1 - survey)
		 * 
		 * @param fg Factor graph
		 */
		void computeSubProducts(Graph* fg);
		void computeProducts(Edge* edge, double & a, double & b, double & c);
		bool walksat(Graph* graph, int MAX_TRIES, int MAX_FLIPS);
		Variable* pickVar(Function* clause);
};

#endif //SP_H
