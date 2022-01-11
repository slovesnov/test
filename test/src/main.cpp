#include "aslov.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <inttypes.h>

#include "BigUnsigned.h"
//typedef __uint128_t maxuint;

uint64_t getBinomialCoefficient(int k,int n){
	uint64_t r = 1;
	int i;
	/* for big n,k
	 * C(n,k)=n*C(n-1,k-1)/k
	 * C(n,k)=n*C(n-1,k-1)/k=(n/k)*(n-1/k-1)...(n-k+1/1)C(n-k,0); C(n-k,0)=1
	 */
	for (i = 1; i <= k; i++) {
		r *= n - k + i;
		r /= i;
	}
	return r;
}

typedef std::vector<int> VInt;
typedef std::vector<VInt> VVInt;

enum class EndgameType{
	ALL,NT,TRUMP,MISERE
};

VVInt suitLengthVector(const int n, bool bridge,EndgameType option) {
	//l[] - number of cards in suit
	int l[4];
	const int nn = (bridge ? 4 : 3) * n;
	const int up = std::min(bridge ? 13 : 8, nn);
	VVInt v;
	VInt vi;
	for (l[0] = 0; l[0] <= up; l[0]++) {
		for (l[1] = 0; l[1] <= up; l[1]++) {
			for (l[2] = 0; l[2] <= up; l[2]++) {
				l[3] = nn - l[0] - l[1] - l[2];
				if (l[3] >= 0 && l[3]<=up) {
					if (option == EndgameType::ALL
							|| ((option == EndgameType::NT || option == EndgameType::MISERE ) && l[0] <= l[1] && l[1] <= l[2]
									&& l[2] <= l[3])
							|| (option == EndgameType::TRUMP && l[1] <= l[2]
									&& l[2] <= l[3])) {
						vi.assign(l, l+4);
						v.push_back(vi);
					}
				}
			}
		}
	}
	return v;
}

int main() {
	printl(toString(12'566'400,',',3));

	const int digits=6;
	int i,j,l;
	uint64_t r;
	BigUnsigned bi;
	std::string s;

/*
	for(j=0;j<2;j++){
		const bool bridge=j==0;
		printl(bridge?"bridge":"preferans")
		l=1;
		while ((i = suitLengthVector(l, bridge, EndgameType::ALL).size())) {
			prints(" & ",l, i,suitLengthVector(l, bridge, EndgameType::NT).size(),suitLengthVector(l, bridge, EndgameType::TRUMP).size())
			printan("\\\\")
			l++;
		}
	}
*/

	for(j=0;j<2;j++){
		const bool bridge=j==0;
		printl(bridge?"bridge":"preferans")
		for(l=1;l<=(bridge?13:10);l++){
			i=(bridge?1:2)*suitLengthVector(l,bridge,EndgameType::NT).size()+suitLengthVector(l,bridge,EndgameType::TRUMP).size();
			r=getBinomialCoefficient(l,3*l)*getBinomialCoefficient(l,2*l);
			bi=r;
			if(bridge){
				r*=getBinomialCoefficient(l,4*l);
				bi*=getBinomialCoefficient(l,4*l);
			}
			s=(bi*i).toString();
			prints(" & ",l,toString(i*r,',',digits),(bi*i).toString(digits, ',')+format(" $\\approx%c.%c \\cdot 10^{%d}$",s[0],s[1],int(s.length()-1)))
//			prints(" & ",l,std::to_string(i)+" $\\cdot$ "+toString(r,',',digits)+" = "+toString(i*r,',',digits)
//					,std::to_string(i)+" $\\cdot$ "+bi.toString(digits, ',')+" = "+(bi*i).toString(digits, ','))
			printan("\\\\")
		}
	}

}
