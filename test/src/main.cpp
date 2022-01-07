#include "aslov.h"
#include <cstdio>
#include <cmath>
#include <cstring>
#include <iostream>
//#include <numeric>

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

int ca(const int m, bool bridge) {
	//a[] - number of cards in suit
	int i = 0, a[4];
	const int mm = (bridge ? 4 : 3) * m;
	const int up = std::min(bridge ? 13 : 8, mm);
	for (a[0] = 0; a[0] <= up; a[0]++) {
		for (a[1] = 0; a[1] <= up; a[1]++) {
			for (a[2] = 0; a[2] <= up; a[2]++) {
				a[3] = mm - a[0] - a[1] - a[2];
				if (a[3] >= 0 && a[3]<=up) {
//					printan(a[1],a[2],a[3])
					i++;
				}
			}
		}
	}
	return i;
}

int main() {
	int n;
	int i,j;
	uint64_t r;

/*
	printl("bridge")
	for(n=1;n<5;n++){
		// /3 at the end for integer division
		r=(2*n*(16*n*n+11)/3+16*n*n+1)*getBinomialCoefficient(n,4*n)*getBinomialCoefficient(n,3*n)*getBinomialCoefficient(n,2*n);
		printan(n,"&",toString(r,','),"\\\\")
	}


	printl("preferans")
	for(n=1;n<7;n++){
		r=(3*n+1)*(3*n+2)/2*getBinomialCoefficient(n,3*n)*getBinomialCoefficient(n,2*n);
		printan(n,"&",toString(r,','),"\\\\")
	}
*/


	for(j=0;j<2;j++){
		const bool bridge=j==0;
		printl(bridge?"bridge":"preferans")
		n=1;
		while((i=ca(n, bridge))){
			if(bridge){
				r=i*getBinomialCoefficient(n,4*n);
			}
			else{
				r=i;
			}
			r*=getBinomialCoefficient(n,3*n)*getBinomialCoefficient(n,2*n);
			//printan(n,"&",i,"\\\\")
			printan(n,"&",toString(r,','),"\\\\")
			n++;
		}
	}


}
