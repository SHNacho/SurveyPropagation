#include <iostream>
#include "random.h"
#include "graph.h"
#include "SP.h"
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]){

	string  prefix = "/home/nacho/Proyectos/SurveyPropagation/data/S-";
	const int N_ALPHAS = 4;
	const int N_FRACTIONS = 6;
	const int N_FILES = 50;
	string alphas[N_ALPHAS] = {"4.21", "4.22", "4.23", "4.24"};
	float fraction[N_FRACTIONS] = {0.04, 0.02, 0.01, 0.005, 0.0025, 0.00125};
	int iteraciones_medias = 0;

	Set_random(29);

	#ifdef _OPENMP
	omp_set_num_threads(1);
	#endif
	for(int i = 0; i < N_ALPHAS; ++i){

		cout << "============== Alpha " << alphas[i] << " ==============" << endl;
		string dirname = prefix + alphas[i];

		bool continuar = true;

		for(int j = 0; j < N_FRACTIONS && continuar; ++j){	
			cout << "-------------- fraction " << fraction[j] << "--------------" << endl;
			string filename;
			Graph* fg;
			int resueltos = 0;
			#pragma omp parallel for private(filename, fg)
			for(int k = 1; k <= N_FILES; ++k){
				filename = dirname + "/cnf_" + to_string(k) + ".txt";
				// Inicializamos el solver
				cout << filename << endl;
				// SolverSP solver;		
				// Generamos el factor graph a partir del archivo
				fg = new Graph(filename); 
				//cout << "Solving " << filename << endl;
				// Ejecutamos SP
				bool r = SID(fg, 1000, 0.001, fraction[j]);
				if( r ){
					#pragma omp atomic
					resueltos++;
					cout << "SAT" << endl;
					//cout << "Iteraciones: " << solver.totalSIDIterations 
				    //     << endl;
				}
				// if( r == result::SAT){
				// 	#pragma omp atomic
				// 	resueltos++;
				// 	cout << "WALKSAT" << endl;
				// 	//cout << "Iteraciones: " << solver.totalSIDIterations 
				//     //     << endl;
				// }

				fg->validate(fg->getVariables());
			}

			delete fg;

			cout << "Resueltos: " << resueltos / 50.0 << endl;
			//cout << "Iteraciones: " << iteraciones_medias / 50.0 << endl;

			if(resueltos / 50.0 == 1){
				continuar = false;
			}
		}
	}

//	if (solver.SID(fg, 0.02) == sat::AlgorithmResult::SAT)
//		cout << "SAT" << endl;


	return 0;
}
