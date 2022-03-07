#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <vector>


using namespace std;

class Edge;
/*
 * Representa una clausula de la CNF
 */
class Function
{
	public:
		//==========================//
		// Declaración de variables //
		//==========================//
		int id;
		int enabled_edges;
		bool satisfied;
		vector<Edge*> neighborhood;

		//==========================//
		// Declaración de funciones //
		//==========================//
		Function(int id);
		inline void satisfy(){satisfied = true;}
		void addNeighbor(Edge* neigh);
		vector<Edge*> enabledNeighborhood();
};

#endif //FUNCTION_H
