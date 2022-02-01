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
		vector<Edge*> neighborhood;

	public:
		Function(int id);
		vector<Edge*> getNeighborhood();
		void addNeighbor(Edge* neigh);
};

#endif //FUNCTION_H
