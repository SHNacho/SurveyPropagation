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

	int graph_size = g.getVariables().size();
	for(int i = 0; i < graph_size; ++i){
		Variable* var = g.getVariables()[i];
		int rand = Randint(0,1);
		var->setValue(rand);
		g.clean(var);
	}
	cout << "Limpio" << endl;
	return 0;
}
