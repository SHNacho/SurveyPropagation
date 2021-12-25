// Ignacio Sánchez Herrera

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include "variable.h"
#include "edge.h"
#include "function.h"

using namespace std;

/**
 * Grafo que representa el problema SAT
 */
class Graph 
{
	private:
		vector<Edge*> edges;
		vector<Variable*> variables;
		vector<Function*> functions;
	public:
		// TODO
		vector<Edge*> getEdges();
		// TODO
		vector<Variable*> getVariables();
		// TODO
		void clean();
};

# endif //GRAPH_H
