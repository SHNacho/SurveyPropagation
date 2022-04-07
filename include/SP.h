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

	const double ZERO = 10e-6;

	bool SID(Graph* graph, int t_max, float precision, float f);
	bool surveyPropagation(Graph* grafo, int t_max, float precision, int & totalIt);
	double SP_UPDATE(Function* clause);
	result unitPropagation(Graph* graph);
	void calculateProducts(Graph* graph);
	void updateOldSurvey(Graph* graph);
	void randomInit(Graph* graph);
	bool walksat(Graph* graph, int MAX_TRIES, int MAX_FLIPS);

};

#endif //SP_H
