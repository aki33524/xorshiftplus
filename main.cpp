#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

typedef unsigned long long Int;

int N = 16;

Int xmask    = 0b1000111;

map<int, int> masktoi;
vector<Int> xcheckers(N), ms1, ms2;
vector<vector<Int>> bits(N);

// Int xchecker = 0b0000000;

// vector<Int> ms1 = {930741146, 160193342, 3455741324, 2076565666, 600356157, 864258987, 2257103378, 28212017, 3434637963, 2338082915, 3965967531, 283610620, 1489845356, 569604114, 1547265624, 1497165050, 4048478155, 263695428, 1270547557, 730503402, 3359943829, 1401522309, 3914482723, 593316373, 4035286308, 1870447092, 1553097111, 93565287, 836748519, 4041051933, 984917322, 4261614288, 2896207535, 2762547492, 615657472, 3993824992, 2672804698, 2897256449, 2114855412, 0, 1913660113, 1606856948, 4218025691, 1870055118, 1120093459, 0, 2740280321, 3540992624, 2796054891, 1904959986, 4066245205, 326346831, 162213989, 1808959235, 3633740070, 3386298337, 1965290011, 0, 1913660113, 3433122375, 228338413, 0, 0, 0};
// vector<Int> ms2 = {585705935, 2716701240, 1408583989, 1671671184, 3083516944, 606635997, 1859970657, 3874373723, 215279124, 3705082566, 1319517632, 2724393664, 3840234399, 4159701915, 753048041, 1854951804, 4095748704, 891774437, 2162363948, 3488615983, 2262678286, 531291899, 2371391076, 3011664061, 3350112477, 3684103382, 1533512647, 2705540065, 537236693, 3496107689, 1398457867, 11258514, 413445617, 721373061, 2860249677, 473803027, 2585381004, 2005198455, 2702572530, 526402, 3722145277, 1057835679, 2610771136, 905834678, 556726904, 32, 3946773686, 828744278, 2192058084, 2888861815, 1126789097, 3536219845, 1563294207, 596142278, 719681407, 978918229, 3851160941, 0, 3722406364, 2590519028, 1372332723, 0, 0, 0};
// vector<int> vs = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0};

// Int to_Int(vector<Int> &v){
// 	int n = v.size();

// 	Int res = 0;
// 	for(int i=0; i<n; i++)
// 		res += v[i] * (1LL<<(n-i));
// 	return res;
// }

void input(){
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

int main(){
	input();

	vector<pair<Int, Int>> miv;
	
	cout << "Checking All y2" << endl;
	for(Int y2=0; y2<(1LL<<32); y2++){
		if(y2 % 100000000 == 0)
			cout << y2 << endl;

		Int c2 = 4047404220LL;
		Int x2;
		bool carry;
		if(y2 < c2){
			x2 = (c2 - y2);
			carry = false;
		}else{
			x2 = (c2 + (1LL<<32) - y2);
			carry = true;
		}
		// if((x2&xmask) != xchecker)
		// 	continue;
		int p = masktoi[x2&xmask];

		Int mv = 0;
		for(int i=0; i<64; i++)
			mv += ((bits[p][i] + __builtin_popcount(y2 & ms2[i]))%2) * (1LL<<(63-i));
		
		miv.emplace_back(mv, y2);
	}

	int n = miv.size();
	sort(miv.begin(), miv.end());

	cout << "Checking All y1" << endl;
	for(Int y1=0; y1<(1LL<<32); y1++){
		if(y1 % 100000000 == 0)
			cout << y1 << endl;

		// TODO: 繰り上がりで場合分け
		Int c1 = 1967260508LL;
		Int x1;
		if(y1 < c1)
			x1 = (c1 - y1);
		else
			x1 = (c1 + (1LL<<32) - y1);

		Int mv = 0;
		for(int i=0; i<64; i++)
			mv += (__builtin_popcount(y1 & ms1[i])%2) * (1LL<<(63-i));

		Int lb = 0;
		Int ub = n;
		while(ub > lb + 1){
			Int mid = (lb + ub)/2;
			if(miv[mid].first <= mv)
				lb = mid;
			else
				ub = mid;
		}
		if(miv[lb].first == mv){
			cout << "y1: " << y1 << endl;
			cout << "y2: " << miv[lb].second << endl;
		}
	}

	return 0;
}