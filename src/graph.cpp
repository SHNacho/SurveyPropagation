#include <fstream>
#include <string>

#include "graph.h"
#include "variable.h"
#include "function.h"
#include "edge.h"

using namespace std;

Graph::Graph(string file){
	ifstream ifs;
	// n -> nº de variables
	// m -> nº de cláusulas
	// k -> variables por cláusula
	int n, m, k;

	ifs.open(file, ifstream::in);

	if(ifs.is_open()){
		string line;
		getline(ifs, line);

		//Obtenemos el número de variables
		getline(ifs, line);
		n = line[line.size()-1] - '0'; // Pasa char a int
		//Obtenemos el número de cláusulas
		getline(ifs, line);
		m = line[line.size()-1] - '0'; // Pasa char a int
		cout << line << endl;
		//Obtenemos el número de variables por cláusula
		getline(ifs, line);
		k = line[line.size()-1] - '0'; // Pasa char a int
	}

	cout << "n: " << n << "\nm: " << m << "\nk: " << k 
		 << endl;
}


