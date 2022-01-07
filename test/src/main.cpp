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
	uint64_t r, r1;
	int i, j, n;
	for (n = 3; n < 10; n++) {
		for (j = n; j < 10; j++) {
			r = getBinomialCoefficient(n, j);
			r1 = 0;
			for (i = n-1; i < j; i++) {
				r1 += getBinomialCoefficient(n-1, i);
			}
			if(r!=r1){
				assert(0);
			}
			printl(n,j,r==r1)
		}
	}

/*
	int n;
	int i,j;
	uint64_t r;

	for(j=0;j<2;j++){
		const bool bridge=j==0;
		printl(bridge?"bridge":"preferans")
		n=1;
		while((i=ca(n, bridge))){
			r=i*getBinomialCoefficient(n,3*n)*getBinomialCoefficient(n,2*n);
			if(bridge){
				r*=getBinomialCoefficient(n,4*n);
			}
			//printan(n,"&",i,"\\\\")
			printan(n,"&",toString(r,','),"\\\\")
			n++;
		}
	}
*/


}
