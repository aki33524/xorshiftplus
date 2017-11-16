#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <thread>
#include <set>
#include <cassert>
using namespace std;

typedef unsigned long long Int;

const int T = 6;
const int C = 1<<T;
int N = 16;

Int O1 = 17962250324576395206LL;
Int xmask = 0;

map<int, int> masktoi;
vector<Int> xcheckers(N), ms1, ms2;
vector<vector<Int>> bits(N);
vector<pair<Int, Int>> miv;
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
		Int carry;
		if(y2 < c2){
			x2 = (c2 - y2);
			carry = 0;
		}else{
			x2 = (c2 + (1LL<<32) - y2);
			carry = 1;
		}
		int p = masktoi[x2&xmask];

		Int mv = 0;
		for(int i=0; i<64; i++)
			mv += ((bits[p][i] + __builtin_popcount(y2 & ms2[i]))%2) * (1LL<<(63-i));
		mv ^= x2;

		// if(mv == 9321916706471420112LL)
		// 	cout << x2 << " " << y2 << endl;

		mivs[ti].emplace_back(mv, y2);
	}
	sort(mivs[ti].begin(), mivs[ti].end());	
}

void process_y1(Int lb, Int ub){
	for(Int y1=lb; y1<ub; y1++){
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

			if(mv == 9321916706471420112LL)
				cout << x1 << " " << y1 << endl;

			Int lb = 0;
			Int ub = miv.size();
			while(ub > lb + 1){
				Int mid = (lb + ub)/2;
				if(miv[mid].first <= mv)
					lb = mid;
				else
					ub = mid;
			}
			if(miv[lb].first == mv){
				Int y2 = miv[lb].second;

				Int c2 = 2332385222LL;
				Int x2;
				Int _carry;
				if(y2 < c2){
					x2 = (c2 - y2);
					_carry = 0;
				}else{
					x2 = (c2 + (1LL<<32) - y2);
					_carry = 1;
				}
				// if(_carry == carry)
				// 	continue;

				Int x = (x1<<32) + x2;
				Int y = (y1<<32) + y2;
				
				cout << (_carry == carry) << endl;
				cout << "x: " << x << endl;
				cout << "y: " << y << endl;
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
	
	cout << "Concatinating mivs" << endl;
	miv.reserve(1LL<<32);
	for(int ti=0; ti<C; ti++){
		for(auto v: mivs[ti])
			miv.push_back(v);
	}
	sort(miv.begin(), miv.end());

	Int lb = 0;
	Int ub = miv.size();
	while(ub > lb + 1LL){
		Int mid = (lb + ub)/2;
		cout << lb << " " << ub << endl;
		cout << mid << " " << miv[mid].first << endl;

		if(miv[mid].first <= 9321916706471420112LL)
			lb = mid;
		else
			ub = mid;
	}
	cout << lb << endl;
	cout << miv[lb].second << endl;

	cout << "miv size: " << miv.size() << endl;

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