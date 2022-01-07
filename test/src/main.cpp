#include "aslov.h"
#include <cstdio>
#include <set>
#include <cstring>
#include <iostream>
#include <algorithm>

struct SI{
	std::string name;
	int count;
};

int main() {
	const std::string BEGIN="<strong>";
	const std::string END="</span></span>";
	const int N=8;
	std::string s,s1,s2;
	//std::stringstream buffer;
	std::size_t p,p1,p2,p3;
	int i,j,k,pk;
	std::set<std::string> names[N];
	std::vector<SI> v;
	std::vector<SI>::iterator it;

	for(j=0;j<N;j++){
		//http://duma.gov.ru/duma/deputies/8/
		std::ifstream f("c:\\downloads\\gd"+std::to_string(j+1)+".html");
		assert(f.is_open());
		std::stringstream buffer;//declare here
		//buffer.clear();
		buffer << f.rdbuf();
		s=buffer.str();
		p=0;
		i=0;
		std::set<std::string>& set=names[j];
		while ( (p=s.find(BEGIN,p))!=std::string::npos){
			p+=BEGIN.length();
			p1=s.find(END,p);
			assert(p1!=std::string::npos);
			i++;
			s1=s.substr(p, p1-p);
			p2=s1.find('<');
			assert(p2!=std::string::npos);
			p3=s1.find_last_of('>');
			assert(p3!=std::string::npos);
			p3++;
			p+=p3;
			s2=s1.substr(0, p2)+" "+s1.substr(p3);
//			if(set.find(s2)!=set.end()){
//				printl(utf8ToLocale(s2));
//			}
			set.insert(s2);
			//printl(utf8ToLocale(s2),"@");
		}
		for(auto&x:set){
			it=std::find_if(v.begin(), v.end(), [&x](auto&a){return a.name==x;});
			if( it == v.end()) {
				v.push_back({x,1});
			}
			else{
				it->count++;
			}
		}
		printzn("gd",j+1," different",set.size()," all",i," size",toString(s.length(),','));
	}

	std::ofstream o("out.html");
	o<<"<html><head><style>.c0{ background-color: Cornsilk; }.c1 { background-color: Azure; } table{border-collapse: collapse;} </style></head><body><table border=1>\n";
/*
	table {
	    border-spacing: 0px;
	    border:1px solid black;
	}
	table tr td {border:1px solid black;}
*/
	std::sort(v.begin(),v.end(),[](auto&a,auto&b){
		if(a.count==b.count){
			return a.name < b.name;
		}
		else{
			return a.count > b.count;
		}
	});

	i = 1;
	j=k=1;
	pk=-1;
	for (auto &a : v) {
		if(a.count!=pk){
			k=1;
			pk=a.count;
		}
		o<<"<tr class='c"<<a.count%2<<"'><td>"<<j<<"<td>"<<k<<"<td>"<<a.name<<"<td>"<<a.count<<"\n";
		j++;
		k++;
		if (a.count == N) {
			printan(i, utf8ToLocale(a.name));
			i++;
		}
	}
	o<<"</table></body></html>";

	for(i=8;i>0;i--){
		j=std::count_if(v.begin(),v.end(),[&i](auto&a){return a.count==i;});
		printan(j,i)
	}
	printan(v.size())
/*
	gd1 different462 all463 size787,055
	gd2 different481 all482 size813,193
	gd3 different465 all467 size809,463
	gd4 different470 all471 size829,877
	gd5 different481 all483 size876,668
	gd6 different521 all523 size928,283
	gd7 different491 all491 size864,214
	gd8 different450 all450 size929,707
*/
	//printl(s.length(),i);

}
