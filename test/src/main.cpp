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

int main() {
	int i, a[4];
	int M=3;

	printl(toString(getBinomialCoefficient(10,20),','))

/*
	for(M=0;M<=8;M++){
		i=0;
		a[0]=0;
		for (a[1] = 0; a[1] <= M-a[0]; a[1]++) {
			for (a[2] = 0; a[2] <= M-a[0]-a[1]; a[2]++) {
				a[3] = M - a[0] - a[1] - a[2];
				if (a[3] >= 0 && a[3] <= M) {
					i++;
				}
				else{
					j++;
				}
			}
		}
		assert(j==0);
		printl(M,i,(M+1)*(M+2)/2);
	}
*/
	//a[2]+a[3]=n sum_i=0^n 1 = n+1

	//a[1]+a[2]+a[3]=n sum_i=0^n (i+1) = n+1+sum_i=0^n i=n+1+n*(n+1)/2=(n+1)(1+n/2)=(n+1)(n+2)/2

	//a[0]+a[1]+a[2]+a[3]=n sum_i=0^n (i+1)(i+2)/2=sum_i=0^n (i^2+3*i+2)/2=( n^3/3+n^2/2+n/6 + 3*n*(n+1)/2 + 2*(n+1) )/2
	//	( M*M*M/3+M*M/2+M/6 + 3*M*(M+1)/2 + 2*(M+1) )/2


	i=0;
	for (a[0] = 0; a[0] <= M; a[0]++) {
		for (a[1] = 0; a[1] <= M-a[0]; a[1]++) {
			for (a[2] = 0; a[2] <= M-a[0]-a[1]; a[2]++) {
				a[3] = M - a[0] - a[1] - a[2];
				assert(a[3] >= 0 && a[3] <= M);
				i++;
			}
		}

	}
	/*
	( v*v*v/3+v*v/2+v/6 + 3*v*(v+1)/2 + 2*(v+1) )/2)
		= ( v*v*v/3+v*v/2+v/6 + 3*v*(v+1)/2 + 2*v )/2 +1
		= v*( v*v/3+v/2+1./6 + 3*(v+1)/2 + 2 )/2 +1
		= v*( v*v/3+2*v+11./3)/2 +1
		= v*(v*v+11)/6 +v*v+1
	*/
	double v=M;
	printl(i,( v*v*v/3+v*v/2+v/6 + 3*v*(v+1)/2 + 2*(v+1) )/2,v*(v*v+11)/6 +v*v+1);
	printl(pow(2,16)*165/pow(2,20));

	printl(toString((1<<16)*165,',')," ",pow(2,16)*165/pow(2,20));


}
