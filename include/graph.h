// Ignacio Sánchez Herrera

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
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
		Graph(string file);
		vector<Edge*> getEdges(){return edges;}
		vector<Variable*> getVariables(){return variables;}
		void addEdge(Variable* var, Function* func, bool neg);
		// TODO
		void clean(){}
	
	private:
		void initVariables(int n_variables);
		void initFunctions(int n_functions);
};

# endif //GRAPH_H
