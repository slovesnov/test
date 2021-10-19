#include <thread>
#include <atomic>
#include <iostream>

#include "aslov.h"

/* 0 - [0..upper) up from 0 to upper-1
 * 1 - [0..max] up from 0 to max
 * 2 - [0..upper) down from upper-1 to 0
 * 3 - [0..max] down from max to 0
 */
#define OPTION 3
const bool test=0;//one thread show all info

const int VALUE=3*1000*1000;

#if (OPTION&1)==1
const int max = VALUE;
#else
const int upper=VALUE+1;
#endif

std::atomic_int atom;
std::vector<std::pair<int,uint64_t>> v;

//count sum of numbers
void f(int t) {
	int i, j = 0;
	uint64_t k=0;
#if OPTION==0
	auto _upper=upper;
	while ((i = atom++) < _upper ) {
#elif OPTION==1
	auto _max=max;
	while ((i = atom++) <= _max ) {
#elif OPTION==2
	while ((i = --atom) >= 0) {
#else
	while ((i = atom--) >= 0) {
#endif
		j++;
		k+=i;
		if(test){
			printzn("thread",t," n=",i)
		}
	}
	v[t] = {j,k};
}

#define TS(a) toString(a,',')

int main (int argc, char** argv){
//	aslovInit(argv);
	int i,t;
	uint64_t sum,u;
	std::vector<std::thread> vt;
	const int cores = test ? 1 : getNumberOfCores();

#if OPTION<2
	atom=0;
#elif (OPTION&1)==1
	atom=max;
#else
	atom=upper;
#endif

	v.resize(cores);
	for (i = 0; i < cores; ++i) {
		vt.push_back(std::thread(f, i));
	}

	for (auto& a : vt){
		a.join();
	}

	t=0;
	sum=0;
	for (auto& a : v){
		printzn("thread", t, ", numbers proceed ", TS(a.first), ", sum ",TS(a.second))
		sum+=a.second;
		t++;
	}

	u=VALUE;
	u*=VALUE+1;
	u/=2;
	printzn("total=",TS(sum)," sum(formula)=",TS(u)," ", u==sum ?"ok":"error")
}

