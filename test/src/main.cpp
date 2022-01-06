#include "aslov.h"
#include <cstdio>
#include <set>
#include <cstring>
#include <iostream>
//#include <numeric>

int main() {
	//http://duma.gov.ru/duma/deputies/8/
	const std::string BEGIN="<strong>";
	const std::string END="</span></span>";
	const int N=8;
	std::string s,s1,s2;
	//std::stringstream buffer;
	std::size_t p,p1,p2,p3;
	int i,j;
	std::set<std::string> names[N];

	for(j=0;j<N;j++){
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
		printzn("gd",j+1," different",set.size()," all",i," size",toString(s.length(),','));
	}

	i=0;
	for(auto v:names[0]){
		for(j=1;j<N;j++){
			std::set<std::string>& set=names[j];
			if(set.find(v)==set.end()){
				break;
			}
		}
		if(j==N){
			printl(++i,utf8ToLocale(v));
		}
	}
/*
	gd1 different462 all463 size787,055
	gd2 different481 all482 size813,193
	gd3 different465 all467 size809,463
	gd4 different470 all471 size829,877
	gd5 different481 all483 size876,668
	gd6 different521 all523 size928,283
	gd7 different491 all491 size864,214
	gd8 different450 all450 size929,707
	1 Грешневиков Анатолий Николаевич        main.cpp:60 main()
	2 Жириновский Владимир Вольфович         main.cpp:60 main()
	3 Зюганов Геннадий Андреевич             main.cpp:60 main()
	4 Морозов Олег Викторович                main.cpp:60 main()
	5 Харитонов Николай Михайлович           main.cpp:60 main()
*/

	//printl(s.length(),i);
}
