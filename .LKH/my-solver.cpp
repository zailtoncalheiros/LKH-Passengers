#include "include/macros.h"
#include "include/pcvp-definitions.cpp"
#include "include/io-routines.cpp"
#include "include/pcvp-pathrepresentation.cpp"
#include "include/allocation-solver.cpp"

/***********************************************************************************************************/
/***** ENTRADA *****/

PCVPInstance ins;

/***********************************************************************************************************/


bool check_symmetric () {
	FORR (i, ins.vertices) {
		FORR (j, i) {
			if (ins.cost[i][j]!=ins.cost[j][i]) return false;
		}
	}
	return true;
}

void create_file_with_header (string filename) {
	ofstream myfile;
	myfile.open (filename);
	
	myfile << "NAME: instance" << endl;
	myfile << "TYPE: " << (check_symmetric()?"TSP":"ATSP") << endl;
	myfile << "DIMENSION: " << ins.vertices << endl;
	myfile << "EDGE_WEIGHT_TYPE: EXPLICIT" << endl;
	myfile << "EDGE_WEIGHT_FORMAT: FULL_MATRIX" << endl;
	myfile << "EDGE_WEIGHT_SECTION" << endl;
	FORR (i, ins.vertices) {
		FORR (j, ins.vertices) {
			myfile << ins.cost[i][j] << " ";
		}
		myfile << endl;
	}

	myfile.close();
}


void __debug_vec (vector<int> &vec) {
	FORR (i, sz(vec)) cout << vec[i] << " ";
	cout << endl;
}

vector<int> canonical_route (vector<int> route) {
	vector<int> caminho;
	int p = 0, n = sz(route);
	while (route[p]!=1 && p!=n) p++;
	if (p==n) {
		cout << "invalid route:" << endl;
		__debug_vec (route);
		exit(0);
	}
	FORR (i, sz(route)-1) {
		p = (p+1) % sz(route);
		caminho.pb(route[p]-1);
	}
	return caminho;
}	


// get_route reads the LKH's output from the standard input.
vector<int> get_route (string filename) {
	vector<int> caminho;
	string str;
	ifstream myfile (filename);
	while (myfile>>str) {
		if (str.compare("TOUR_SECTION")==0) break;
	}
	vector<int> route;
	while (myfile>>str) {
		if (str.compare("-1")==0) break;
		stringstream ss; ss << str;
		int x; ss >> x;
		route.pb (x);
	}
	return canonical_route (route);
}


double run (string filename) {
	cerr << "Reading route from " << filename << "." << endl;
	vector<int> caminho = get_route(filename);
	vector<int> caronas = allocation_solver::solve (ins, caminho);
	
	if (!satisfazCapacidadesETarifas(ins, caminho, caronas)) {
		cerr << "A configuração a seguir não apresenta uma solução válida e será reparada." << endl;
		__DEBUG_SOLUTION(caminho, caronas);
		reparar (ins, caminho, caronas);
	}
	assert(satisfazCapacidadesETarifas(ins, caminho, caronas));
	return calculaCusto (ins, caminho, caronas);
}

std::string exec(const char* cmd) {
    char buffer[1280];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

string getPrefix (string s) {
	while (s.back() != '/') {
		s.pop_back();
	}
	s.pop_back();
	return s;
}

int main (int argc, char* argv[]) {
	int teste = 1;
	while (ler_entrada (ins)) {
		vector<double> resultados, tempos;
		int tentativas = 1;
		FORR (t, tentativas) {
			auto begin_time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
			
			//Create a file with the current instance for LKH use.
			//Write into the file "input.tsp".
			char buffer[2048];
			sprintf (buffer, "%s/input.tsp", getPrefix(string(argv[0])).c_str() );
			create_file_with_header (string(buffer));	
			
			//Run LKH.
			sprintf (buffer, "DIR=\"%s\" && cd $DIR && ./LKH lkh.par 1>LKH-output.txt 2>LKH-output.txt2 3>LKH-output.txt3", getPrefix(string(argv[0])).c_str() );
			cerr << "Building route!" << endl;
			
			exec(buffer);
			

			//Run the passengers solver.
			cerr << "Adding passengers!" << endl;
			sprintf (buffer, "%s/output.sol", getPrefix(string(argv[0])).c_str());
			double aux = run(string(buffer)) / 100.;

			//Compile the results.
			double tempogasto = (double) ( chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count() - begin_time) / 1000;
			cerr << aux << " | " << tempogasto << endl;
			resultados.pb (aux); 
			tempos.pb (tempogasto);

		}
		print_results (ins, resultados, tempos, teste);
		teste++;
	}
}
