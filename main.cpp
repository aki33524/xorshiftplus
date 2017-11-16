#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <thread>
#include <set>
#include <cassert>
using namespace std;

typedef unsigned long long Int;

const int T = 5;
const int C = 1<<T;
int N = 16;

Int O1 = 17962250324576395206LL;
Int xmask = 0;

map<int, int> masktoi;
vector<Int> xcheckers(N), ms1, ms2;
vector<vector<Int>> bits(N);
vector<vector<pair<Int, Int>>> mivs(C);
set<Int> mits;

void input(){
	for(int i=0; i<4; i++){
		int x; cin >> x;
		xmask |= 1<<(63-x);
	}
	for(int i=0; i<N; i++){
		cin >> xcheckers[i];
		masktoi[xcheckers[i]] = i;
		for(int j=0; j<64; j++){
			Int x; cin >> x;
			bits[i].push_back(x);
		}
	}
	for(int j=0; j<64; j++){
		Int x; cin >> x;
		ms1.push_back(x);
	}
	for(int j=0; j<64; j++){
		Int x; cin >> x;
		ms2.push_back(x);
	}	
}

void process_y2(Int lb, Int ub, int ti){
	for(Int y2=lb; y2<ub; y2++){
		// Int c2 = O1 % (1LL<<32);
		Int c2 = 2332385222LL;
		Int x2;
		bool carry;
		if(y2 < c2){
			x2 = (c2 - y2);
			carry = false;
		}else{
			x2 = (c2 + (1LL<<32) - y2);
			carry = true;
		}
		int p = masktoi[x2&xmask];

		Int mv = 0;
		for(int i=0; i<64; i++)
			mv += ((bits[p][i] + __builtin_popcount(y2 & ms2[i]))%2) * (1LL<<(63-i));
		mv ^= x2;

		mivs[ti].emplace_back(mv, y2);
	}
	sort(mivs[ti].begin(), mivs[ti].end());	
}

void process_y1(Int lb, Int ub){
	for(Int y1=lb; y1<ub; y1++){
		// TODO: 繰り上がりで場合分け
		for(Int carry=0; carry<=1; carry++){
			// Int c1 = O1>>32;
			Int c1 = 4182162304LL;
			c1  -= carry;
			Int x1;
			if(y1 < c1)
				x1 = (c1 - y1);
			else
				x1 = (c1 + (1LL<<32) - y1);

			Int mv = 0;
			for(int i=0; i<64; i++)
				mv += (__builtin_popcount(y1 & ms1[i])%2) * (1LL<<(63-i));
			mv ^= (x1<<32);

			for(int ti=0; ti<C; ti++){
				Int lb = 0;
				Int ub = mivs[ti].size();
				while(ub > lb + 1){
					Int mid = (lb + ub)/2;
					if(mivs[ti][mid].first <= mv)
						lb = mid;
					else
						ub = mid;
				}
				if(mivs[ti][lb].first == mv){
					cout << y1 << " " <<  mivs[ti][lb].second << " " << mv << endl;
				}
			}
		}
	}
}

int main(){
	input();

	vector<thread> threads;

	cout << "Checking All y2" << endl;
	int ti = 0;
	int X = 32;
	for(Int y2=0; y2<(1LL<<X); y2+=(1LL<<(X-T))){
		thread th(process_y2, y2, y2+(1LL<<(X-T)), ti);
		threads.push_back(move(th));
		ti++;
	}
	for(auto& t: threads)
		t.join();
	
	cout << "Checking All y1" << endl;
	threads.clear();
	for(Int y1=0; y1<(1LL<<X); y1+=(1LL<<(X-T))){
		thread th(process_y1, y1, y1+(1LL<<(X-T)));
		threads.push_back(move(th));
	}
	for(auto& t: threads)
		t.join();

	return 0;
}