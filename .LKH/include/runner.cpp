#ifndef _RUNNER_CPP_
#define _RUNNER_CPP_

#include "io-routines.cpp"
#include "utils/flags.cpp"

void execute_algorithm (Flags &flags, PCVPInstance &ins, double (*run)(Flags &flags)) {
    if (flags.hasFlag("irace")) {
        freopen (flags.getFlag("instance").c_str(), "r", stdin);

        while (ler_entrada (ins)) {		
			cout << "Best " << run(flags)/100. << endl;
		}
    } else {
        while (ler_entrada (ins)) {
			vector<double> resultados, tempos;
			int tentativas = (flags.hasFlag("tries") ? flags.getFlagToInt("tries") : 30);
            int teste = 1;
			FORR (t, tentativas) {
				const clock_t begin_time = clock();
				double aux = run(flags)/100.;
				double tempogasto = (double) ( clock () - begin_time ) /  CLOCKS_PER_SEC;
				cerr << aux << " | " << tempogasto << endl;
				resultados.pb (aux); 
				tempos.pb (tempogasto);
			}
			print_results (ins, resultados, tempos, teste);
			teste++;
		}
    }
}

#endif