#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <vector>

using namespace std;
class Edge;

enum lbool{
	True = 1,
	False = 0,
	Unassigned = -1
};

class Variable 
{
	public:
		//==========================//
		// Declaración de variables //
		//==========================//
		int id; 
		lbool value;
		// Conjunto de nodos vecinos (nodos función)
		// se identifican por su arista
		vector<Edge*> neighborhood;
		// Subconjunto de nodos del vecindario en los que 
		// la variable NO aparece negada
		vector<Edge*> positiveNeighborhood;
		// Subconjunto de nodos del vecindario en los que 
		// la variable aparece negada
		vector<Edge*> negativeNeighborhood;

		// Productos para SP-UPDATE
		double ps;
		double pu;
		double p0;

		// Sesgos
		double negativeBias;
		double positiveBias;
		double nullBias;

		//==========================//
		// Declaración de funciones //
		//==========================//
		Variable(int id);
		void addNeighbor (Edge* neigh);
		vector<Edge*> enabledNeighborhood();
		double calculateBias();
		void setValue(const lbool value);
		void setValue(const bool value);
		void fix();

		friend bool operator==(const Variable& lhs, const Variable& rhs);
};


#endif //VARIABLE_H
