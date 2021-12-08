#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <vector>

using namespace std;
class Edge;
class Variable 
{
	private:
		int id; 
		bool value;
		// Conjunto de nodos vecinos (nodos función)
		vector<Edge*> neighborhood;
		// Subconjunto de nodos del vecindario en los que 
		// la variable aparece negada
		vector<Edge*> positiveNeighborhood;
		// Subconjunto de nodos del vecindario en los que 
		// la variable NO aparece negada
		vector<Edge*> negativeNeighborhood;

		double ps;
		double pu;
		double p0;

	public:
		//TODO
		vector<Edge*> getNeighborhood();
		//TODO
		vector<Edge*> getPosNeighborhood();
		//TODO
		vector<Edge*> getNegNeighborhood();
		//TODO
		double getPs();
		//TODO
		double getPu();
		//TODO
		double getP0();
		//TODO
		void setPs(double ps);
		//TODO
		void setPu(double pu);
		//TODO
		void setP0(double p0);

};


#endif //VARIABLE_H
