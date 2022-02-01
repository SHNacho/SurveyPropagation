#include <fstream>
#include <string>
#include <numeric>
#include <cstdlib>

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
			if(var > 0)
				neg = false;
			
			edges.push_back(new Edge(variables[var - 1], functions[i], neg));	
		}	

		ifs >> var;
	}

	ifs.close();

	cout << "variables size: " << variables.size() << "\nfunctions size: " 
		 << functions.size() << "\nedges size: " << edges.size() 
		 << endl;
}

//----------------------------------------------//
void Graph::initVariables(int n_variables){
	for(int i = 1; i <= n_variables; ++i){
		variables.push_back(new Variable(i));
	}
}

//----------------------------------------------//
void Graph::initFunctions(int n_functions){
	for(int i = 0; i < n_functions; ++i){
		functions.push_back(new Function(i));
	}
}


