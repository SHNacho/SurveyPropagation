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
	cout << "file: " << file << endl;
	float f = atof(argv[2]);
	cout << "f: " << f<< endl;
	// Generamos el grafo
	Graph g(file);
	cout << file << ", ";
	SID(&g, 1000, 0.001, f);
	//unitPropagation(&g);

	Graph aux(file);
	if(aux.validate(g.getVariables()))
		cout << "Correcto" << endl;


	//cout << endl << "=========Resultado=========" << endl;
	//for(Variable* var : g.getVariables()){
	//	cout << "Variable " << var->getId() << " --> " << var->getValue() << endl;
	//}




	return 0;
}
