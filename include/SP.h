#ifndef SP_H
#define SP_H

#include <iostream>
#include <cmath>
#include <vector>
#include "graph.h"

class SolverSP{

	public:
		bool SID(Graph* graph, int t_max, float precision);
		bool surveyPropagation(Graph grafo, int t_max, float precision);
		
		double SP_UPDATE(Edge* edge);
		void calculateProducts();
		bool clean(Variable* var);
		bool unitPropagation(Function* clause);

};
#endif //SP_H
