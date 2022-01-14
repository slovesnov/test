#include "aslov.h"
#include <cstdio>
#include <numeric>//iota
#include <cstring>
#include <iostream>
#include <cmath>

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
	ALL,NT,TRUMP
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
							|| (option == EndgameType::NT && l[0] <= l[1] && l[1] <= l[2]
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

std::string binaryCodeString(int c, int miminumPrintBits = 0) {
	assert(miminumPrintBits<=32);
	const int f=64;
	char b[128], h[50], *pb=b+f, *ph=h;
	itoa(c, b+f, 2);
	int i,l = strlen(b+f);
	if (miminumPrintBits && (i=miminumPrintBits-l)>0 ) {
		memset(b+f-i, '0', i);
		pb-=i;
	}
	l = strlen(pb) & 1;
	for (i = 0; *pb != 0; i++) {
		if ((i & 1) == l && i != 0) {
			*ph++ = '_';
		}
		*ph++ = *pb++;
	}
	*ph = 0;
	return h;
}

void rotate(int n,int bits,int a[3]){
	int i,j,r;

	int m_w[43];
	i=0;
	for(int& a:m_w){
		a=i%4;
		i++;
	}

	assert(bits % 2 == 0);
	for (j = 0; j < 3; j++) {
		r = 0;
		for (i = 0; i < bits / 2; i++) {
			r |= m_w[((n>>(2*i)) & 3)+j+1] << (2 * i);
		}
		a[j] = r;
	}
}


int main() {

//	int i,j;//,k,l;
//	char byte;

//	int a[]={0,2,5};
//	for(i=0;i<3;i++){
//		j=a[i];
//		printl(binaryCodeString(j));
//		printl(binaryCodeString(j,2));
//		printl(binaryCodeString(j,5));
//		printl(binaryCodeString(j,6));
//	}

/*
	srand(time(NULL));
	const int bits=12;
	const int mask=(1<<bits)-1;

	// 1<<k = 2^k 2^3=1000 2^3-1 7
	clock_t begin=clock();
	for(j=0;j<369600;j++){
		i=(rand()&mask) | ((rand()&mask)<<bits);
		rotate(i, 2*bits,a);
	}
	printl(timeElapse(begin));
*/

/*
	int n,a[3];
	VVInt v;
	for(i=0;i<2;i++){
	for(n=1;;n++){
		bool bridge=i==0;
		for(j=0;j<3;j++){
			v=suitLengthVector( n, bridge,EndgameType(j));
			if(v.empty()){
				goto l144;
			}
			if(!j){
				printf("\n<tr><td>%d",n);
			}
			a[j]=v.size();
			printf("<td>%d</td>",a[j]);
		}
		printf("<td>%d",a[1]+(bridge?1:2)*a[2]);
	}
		l144:;
		printf("\n");
	}
*/

//	printl(binaryCodeString(i,24));
//	for(auto b:a){
//		printl(binaryCodeString(b,24));
//	}

 	const int digits=6;
 	const bool latex=0;
	int i,j,l;
	uint64_t r;
	BigUnsigned bi;
	std::string s,s1;

	for(j=0;j<2 ;j++){
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
			if(latex){
				prints(" & ",l,toString(i*r,',',digits),(bi*i).toString(digits, ',')+format(" $\\approx%c.%c \\cdot 10^{%d}$",s[0],s[1],int(s.length()-1)))
				printan("\\\\")
			}
			else{
				s1=formats("</td><td>",l, (bi*i).toString(digits, ',')+format(" &asymp; %c.%c&sdot;10<sup>%d</sup>",s[0],s[1],int(s.length()-1)));
//				s1=formats("</td><td>",l,"\\("+(bi*i).toString(digits, ',')+format("\\approx%c.%c \\cdot 10^{%d}\\)",s[0],s[1],int(s.length()-1)));
				printzn("<tr><td>",s1,"</td>")
			}
		}
	}


}
