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
	private:
		int id;
		int enabled_edges;
		bool satisfied;
		vector<Edge*> neighborhood;

	public:
		Function(int id);
		int getId(){return id;}
		int getEnabledEdges(){return enabled_edges;}
		bool isSatisfied(){return satisfied;}
		void satisfy(){satisfied = true;}
		vector<Edge*>& getNeighborhood(){return neighborhood;}
		void addNeighbor(Edge* neigh);
		// Elimina un vecino (en caso de que
		// la variable ya se haya asignado)
		// Se realiza en tiempo O(k) 
		// (k -> número de variables por clausula)
		void removeNeighbor(int var_id);
		void removeNeighborhood();
		~Function();
};

#endif //FUNCTION_H
