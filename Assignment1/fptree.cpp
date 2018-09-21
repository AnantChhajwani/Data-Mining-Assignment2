#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <cstdlib>

using namespace std;

map <int, int> freq;

vector <int> flist;
map <int, int> flist_index;
vector <bool> flist_occur;

int NUM_TRANSACTIONS, NUM_ITEMS, MIN_SUPPORT;

vector <int> node_label;
vector <int> node_count;
vector <int> par;
vector <vector <int> > chd;
vector <vector <int> > ans;

int NUM_NODES;

bool cmp(vector <int> lhs, vector <int> rhs) {
	if(lhs.size() < rhs.size()) return true;
	if(lhs.size() > rhs.size()) return false;
	for(int i = 0; i < lhs.size(); i++) {
		if(lhs[i] != rhs[i]) return lhs[i] < rhs[i];
	}
	return true;
}

void complete(string outputfile) {
	ofstream outfile;
	outfile.open(outputfile);
	for(int i = 0; i < ans.size(); i++) {
		for(int j = 0; j < ans[i].size(); j++) {
			outfile << ans[i][j] << " ";
		}
		outfile << endl;
	}
	outfile.close();
}

void first_pass(string inputfile) {
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
	vector <pair <int, int> > frlist;
	for(int i = 0; i < NUM_ITEMS; i++) {
		if(freq[i] * 100 >= NUM_TRANSACTIONS * MIN_SUPPORT)
			frlist.push_back(make_pair(freq[i], i));
	}
	sort(frlist.begin(), frlist.end());
	flist_occur.resize(NUM_ITEMS);
	for(int i = 0; i < NUM_ITEMS; i++) flist_occur[i] = false;
	for(int i = frlist.size() - 1; i >= 0; i--) {
		flist.push_back(frlist[i].second);
		flist_index.insert(make_pair(frlist[i].second, frlist.size() - i - 1));
		flist_occur[frlist[i].second] = true;
	}
}

void insert_path(vector <int> v) {
	int nd = 0;
	for(int i = 0; i < v.size(); i++) {
		bool found = false;
		
		for(int j = 0; j < chd[nd].size(); j++) {
			if(node_label[chd[nd][j]] == v[i]) {
				nd = chd[nd][j];
				node_count[nd]++;
				found = true; 
				break;
			}
		}
		if(!found) {
			for(int j = i; j < v.size(); j++) {
				par.push_back(nd);
				chd[nd].push_back(NUM_NODES);
				chd.push_back({});
				node_label.push_back(v[j]);
				node_count.push_back(1);
				nd = NUM_NODES;
				NUM_NODES++;
			}
			break;
		}
	}
}

void second_pass(string inputfile) {
	node_count.push_back(-1);
	node_label.push_back(-1);
	par.push_back(-1);
	chd.push_back({});
	NUM_NODES++;

	ifstream infile (inputfile);
	string line;
	vector <int> v;
	while(getline(infile, line)) {
		stringstream stream(line);
		while(true) {
			int n;
			stream >> n;
			if(!stream) break;
			if(flist_occur[n]) v.push_back(flist_index[n]);
		}
		sort(v.begin(), v.end());
		insert_path(v);
		v.clear();
	}
	infile.close();
}

void frequent_itemsets(int idx, vector <int> cur, map <int, int> sel_cnt) {
	for(int i = idx - 1; i >= 0; i--) {
		map <int, int> sel_cnt1;
		int tot = 0;
		for(auto it : sel_cnt) {
			int nd = it.first;
			int val = it.second;
			while(nd > 0) {
				if(node_label[nd] == i) {
					if(sel_cnt1.count(nd) > 0) sel_cnt1[nd] += val;
					else sel_cnt1.insert(make_pair(nd, val));
					tot += val;
					break;
				}
				nd = par[nd];
			}
		} 
		if(tot * 100 >= NUM_TRANSACTIONS * MIN_SUPPORT) {
			vector <int> cur1;
			for(int i = 0; i < cur.size(); i++) cur1.push_back(cur[i]);
			cur1.push_back(i);
			ans.push_back(cur1);
			frequent_itemsets(i, cur1, sel_cnt1);
		}
	}
}

int main(int argc, char* argv[]) {
	clock_t t;
	t = clock();
	string inputfile = argv[1];
	string outputfile = argv[2];
	MIN_SUPPORT = atoi(argv[3]);
	first_pass(inputfile);
	second_pass(inputfile);
	for(int i = 0; i < flist.size(); i++) {
		vector <int> t;
		t.push_back(i);
		ans.push_back(t);
	}
	for(int i = 0; i < flist.size(); i++) {
		vector <int> cr;
		cr.push_back(i);
		map <int, int> mm;
		for(int j = 0; j < NUM_NODES; j++) {
			if(node_label[j] == i) {
				if(mm.count(j) > 0) mm[j] += node_count[j];
				else mm.insert(make_pair(j, node_count[j]));
			}
		}
		frequent_itemsets(i, cr, mm);
	}

	for(int i = 0; i < ans.size();i++) {
		for(int j = 0; j < ans[i].size(); j++) {
			ans[i][j] = flist[ans[i][j]];
		}
	}
	for(int i = 0; i < ans.size(); i++) {
		sort(ans[i].begin(), ans[i].end());
	}
	sort(ans.begin(), ans.end(), cmp);
	complete(outputfile);
	t = clock() - t;
	cout << fixed << setprecision(6) << ((float)t)/CLOCKS_PER_SEC;
	return 0;
}