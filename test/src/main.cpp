#include "aslov.h"
#include <cstdio>
#include <numeric>

using T=std::pair<int,double>;
using VT=std::vector<T>;

int main() {
	VT v;
	v.push_back({10,23.});
	v.push_back({10,1.1});
	v.push_back({0,8.8});

	double sum=std::accumulate(v.begin(), v.end(),0., [&] (auto&a,auto&b) {
		printi
	    return a + b.second;
	});

	printl(sum);

	int i=-1;
	printl(i?"f":"s")

	return 0;
}
