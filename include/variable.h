#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <vector>

using namespace std;
class Edge;

class Variable 
{
	private:
		// id de la variable
		int id; 
		// Valor que toma la variable
		bool value;
		bool assigned;
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
		Variable(int id);

		void addNeighbor (Edge* neigh);
		double calculateBias();
		void fix();
		void removeNeighbor(int func_id);

		/////////////////////////
		// Setters and getters //
		/////////////////////////
		/**
		 * Asigna a la variable el valor 'val'.
		 * - Si el valor es 0, marca como satisfechas 
		 * todas las cláusulas de V-
		 * - Si el valor es 1, marca como satisfechas
		 * todas las cláusulas de V+
		 */
		void setValue(bool val);
		int getId(){return id;}
		int getValue(){return value;}
		vector<Edge*>& getNeighborhood(){return neighborhood;};
		vector<Edge*>& getPosNeighborhood(){return positiveNeighborhood;}
		vector<Edge*>& getNegNeighborhood(){return negativeNeighborhood;}
		vector<Edge*> enabledNeighborhood();
		vector<Edge*> enabledPositiveNeighborhood();
		vector<Edge*> enabledNegativeNeighborhood();
		double getPs(){return ps;}
		double getPu(){return pu;}
		double getP0(){return p0;}
		double getPosBias(){return positiveBias;}
		double getNegBias(){return negativeBias;}
		double getNullBias(){return nullBias;}
		void setPs(double ps){this->ps = ps;}
		void setPu(double pu){this->pu = pu;}
		void setP0(double p0){this->p0 = p0;}
		bool isAssigned(){return assigned;}
};

bool compareVars(Variable* v1, Variable* v2);

#endif //VARIABLE_H
