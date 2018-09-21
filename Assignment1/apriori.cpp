#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

vector <vector <int> > C, F;
map <int, int> freq;

int idxC, idxF;
int NUM_ITEMS, MIN_SUPPORT, NUM_TRANSACTIONS;

void initial_pass(string inputfile) {
	ifstream infile (inputfile);
	string line;
	while(getline(infile, line)) {
		stringstream stream(line);
		while(true) {
			int n;
			stream >> n;
			if(!stream) break;
			if(freq.find(n) == freq.end()) 
				freq.insert(make_pair(n, 1));
			else freq[n]++;
		}
		NUM_TRANSACTIONS++;
	}
	infile.close();
	NUM_ITEMS = freq.size();
	for(int i = 0; i < NUM_ITEMS; i++)
		C.push_back({i});
	idxC = 0;
}

void candidate_generation(int z) {
	idxC = C.size();
	for(int i = idxF; i < F.size(); i++) {
		for(int j = i + 1; j < F.size(); j++) {
			bool match = true;
			for(int k = 0; k < z - 2; k++) {
				if(F[i][k] != F[j][k]) match = false;
			}
			if(!match) continue;
			vector <int> v, w;
			for(int k = 0; k < z - 2; k++)
				v.push_back(F[i][k]);
			v.push_back(min(F[i][z - 2], F[j][z - 2]));
			v.push_back(max(F[i][z - 2], F[j][z - 2]));
			bool satisfy = true;
			for(int k = 0; k < z; k++) {
				w.clear();
				bool found = false;
				for(int l = 0; l < z; l++)
					if(l != k) w.push_back(v[l]);
				for(int l = idxF; l < F.size(); l++) {
					match = true;
					for(int m = 0; m < z - 1; m++) {
						if(F[l][m] != w[m]) match = false;
					}
					if(match) {
						found = true;
						break;
					}
				}
				if(!found) {
					satisfy = false;
					break;
				}
			}
			if(satisfy) C.push_back(v);
		}
	}
}

void complete(string outputfile) {
	ofstream outfile;
	outfile.open(outputfile);
	for(int i = 0; i < F.size(); i++) {
		for(int j = 0; j < F[i].size(); j++) {
			outfile << F[i][j] << " ";
		}
		outfile << endl;
	}
	outfile.close();
}

int main(int argc, char* argv[]) {
	string inputfile = argv[1];
	string outputfile = argv[2]; 
	MIN_SUPPORT = atoi(argv[3]);
	clock_t t;
	t = clock();
	initial_pass(inputfile);
	for(int i = 0; i < NUM_ITEMS; i++) {
		if(freq[i] * 100 >= NUM_TRANSACTIONS * MIN_SUPPORT)
			F.push_back({i});
	}
	if(F.size() == 0) {
		complete(outputfile);
		t = clock() - t;
		cout << fixed << setprecision(6) << ((float)t)/CLOCKS_PER_SEC;
		return 0;
	}
	idxF = 0;
	for(int i = 2; i <= NUM_ITEMS; i++) {
		candidate_generation(i);
		vector <int> v(NUM_ITEMS);
		vector <int> w;
		vector <int> c(C.size() - idxC);
		for(int j = 0; j < NUM_ITEMS; j++) v[j] = 0;
		for(int j = 0; j < C.size() - idxC; j++) c[j] = 0;
		ifstream infile (inputfile);
		string line;
		while(getline(infile, line)) {
			stringstream stream(line);
			while(true) {
				int n;
				stream >> n;
				if(!stream) break;
				w.push_back(n);
				v[n] = 1;
			}
			for(int j = idxC; j < C.size(); j++) {
				bool match = true;
				for(int k = 0; k < C[j].size(); k++) {
					if(v[C[j][k]] == 0) match = false;
				}
				if(match) c[j - idxC]++;
			}
			for(int i = 0; i < w.size(); i++)
				v[w[i]] = 0;
			w.clear(); 
		}
		infile.close();
		idxF = F.size();
		bool added = false;
		for(int j = idxC; j < C.size(); j++) {
			if(c[j - idxC] * 100 >= NUM_TRANSACTIONS * MIN_SUPPORT) {
				added = true;
				F.push_back(C[j]);
			}
		}
		if(!added) break;
	}
	complete(outputfile);
	t = clock() - t;
	cout << fixed << setprecision(6) << ((float)t)/CLOCKS_PER_SEC;
	return 0;
}