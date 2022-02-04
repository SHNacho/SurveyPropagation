#include <fstream>
#include <ios>
#include <string>
#include <numeric>
#include <cstdlib>
#include <algorithm>

#include "graph.h"
#include "variable.h"
#include "function.h"
#include "edge.h"


using namespace std;

Graph::Graph(string file){
	ifstream ifs;
	// n -> nº de variables
	// m -> nº de cláusulas
	// k -> nº variables por cláusula
	int n, m, k;

	ifs.open(file, ifstream::in);

	if(ifs.is_open()){
		cout << "Generando factor graph..." << endl;
	}

	string line;
	getline(ifs, line, '=');
	// Obtenemos el número de variables
	getline(ifs, line);
	n = stoi(line); // Pasa char a int
	getline(ifs, line, '=');
	// Obtenemos el número de cláusulas
	getline(ifs, line);
	m = stoi(line); // Pasa char a int
	getline(ifs, line, '=');
	// Obtenemos el número de variables por cláusula
	getline(ifs, line);
	k = stoi(line); // Pasa char a int
	getline(ifs, line);
	getline(ifs, line);

	// reservamos espacio para las variables, clausulas y aristas
	this->variables.reserve(n);
	this->functions.reserve(m);
	this->edges.reserve(m * k);

	// inicializamos las variables y cláusulas
	// rellenando el vector de variables de 1 a n
	// y el de clausulas de 0 a m
	initVariables(n);
	initFunctions(m);

	// Pasamos a leer las clausulas
	for(int i = 0; i < m; ++i){
		int var;
		for(int j = 0; j < k; ++j){

			bool neg = true;
			
			ifs >> var;
			if(var > 0){
				neg = false;
			}
				
			int var_pos = abs(var) - 1;

			addEdge(variables[var_pos], functions[i], neg);
		}	

		ifs >> var;
	}

	ifs.close();

	cout << "Número de nodos variable: " << variables.size() << endl
		 << "Número de nodos cláusula: " << functions.size()  << endl
		 << "Número de aristas:        " << edges.size() << endl;
}

//----------------------------------------------//
void Graph::addEdge(Variable* var, Function* func, bool neg){
	Edge* e = new Edge(var, func, neg);
	edges.push_back(e);
	var->addNeighbor(e);
	func->addNeighbor(e);
}

//----------------------------------------------//
void Graph::initVariables(int n_variables){
	for(int i = 1; i <= n_variables; ++i){
		this->variables.push_back(new Variable(i));
	}
}

//----------------------------------------------//
void Graph::initFunctions(int n_functions){
	for(int i = 0; i < n_functions; ++i){
		this->functions.push_back(new Function(i));
	}
}

//----------------------------------------------//
void Graph::clean(Variable* fixed_var){
	int val = fixed_var->getValue();
	// Obtenemos el id de la variable fijada
	int id = fixed_var->getId();	

	vector<Edge*> neigh;
	// Si el valor asignado a la variable es 0
	// eliminamos todas las aristas que contengan
	// las cláusulas que tengan esta variable negada
	if ( val == 0){
		neigh = fixed_var->getNegNeighborhood();
	// Si el valor asignado a la variable es 1
	// eliminamos todas las aristas que contengan
	// las cláusulas que tengan esta variable NO negada
	} else {
		neigh = fixed_var->getPosNeighborhood();
	}

	for(int i = 0; i < neigh.size(); ++i){
		Function* func = neigh[i]->getFunction();
		int func_id = func->getId();

		for(int j = 0; j < edges.size(); ++j){
			int edge_func_id = edges[j]->getFunction()->getId();
			if(edge_func_id == func_id){
				func->removeNeighborhood();
				edges.erase(edges.begin() + j);
				j--;
			}
		}
	}

	// Eliminamos del grafo todas las aristas que
	// incluyan la variable
	for(int i = 0; i < edges.size(); ++i){
		int var_id = edges[i]->getVariable()->getId();
		if(var_id == id){
			edges.erase(edges.begin() + i);
			// Eliminamos el puntero que apunta a la variable
			// en la cláusula, lo que elimina el puntero
			// que apunta a la función en la variable.
			edges[i]->getFunction()->removeNeighbor(id);
		}
	}

}

















