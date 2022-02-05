#include <iostream>
#include "random.h"
#include "graph.h"
#include "SP.h"

using namespace std;

int main (int argc, char *argv[]){
	// Inicializamos la semilla para generar números aleatorios
	Set_random(29);
	// Leemos el archivo donde se encuentra el problema SAT en CNF
	string file = argv[1];
	// Generamos el grafo
	Graph g(file);

	SID(&g, 1000, 0.001);

	vector<Variable*> variables = g.getVariables();

	//cout << endl << "=========Resultado=========" << endl;
	//for(Variable* var : variables){
	//	cout << "Variable " << var->getId() << " --> " << var->getValue() << endl;
	//}

	// TODO: en el cnf_easy.txt, el vecino de la variable con id=2 no se elimina
	// y se debería eliminar al establecer el valor de la variable 1 a 0
	//cout << variables[1]->getNeighborhood().size() << endl;



	return 0;
}
