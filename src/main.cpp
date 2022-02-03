#include <iostream>
#include "random.h"
#include "graph.h"

using namespace std;

int main (int argc, char *argv[]){
	// Inicializamos la semilla para generar números aleatorios
	Set_random(29);
	// Leemos el archivo donde se encuentra el problema SAT en CNF
	string file = argv[1];
	// Generamos el grafo
	Graph g(file);

	Variable* var = g.getFunctions()[0]->getNeighborhood()[0]->getVariable();
	g.clean(var);
	return 0;
}
