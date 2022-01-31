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

		// Productos para el cálculo de los sesgos 

	public:
		Variable(int id, bool value);
		vector<Edge*> getNeighborhood();
		vector<Edge*> getPosNeighborhood();
		vector<Edge*> getNegNeighborhood();
		double getPs(){return ps;}
		double getPu(){return pu;}
		double getP0(){return p0;}
		double getPosBias(){return positiveBias;}
		double getNegBias(){return negativeBias;}
		double getNullBias(){return nullBias;}
		void setPs(double ps);
		void setPu(double pu);
		void setP0(double p0);
		//TODO: Devuelve la resta entre bias positivo y negativo
		double calculateBias();
		//TODO
		void fix(){}

};


#endif //VARIABLE_H
