//#include <cstdio>
#include <cstring>
#include <thread>
#include <atomic >

#include "aslov.h"

//#include <filesystem>
//using namespace std::filesystem;

std::atomic_int atom;
//int max=20;
std::vector<int> v;

void f(int n){
	int i,j=0,k;
	while((i=atom--) >=0){
		j++;

		double v=4.3;
		for(k=0;k<10000;k++){
			v*=k;
		}
	}
	v[n]=j;
	printzn("t",n," j=",intToString(j))


//	while((i=atom++)<max){
//		printzn("t",n," i=",i)
//	}
}

int main (int argc, char** argv){
//	aslovInit(argv);

/*

	int a,r;

	a=10;
	r=(a--);//10
	printl(r)

	a=10;
	r=--a;//9
	printl(r)

	a=10;
	r=a-=1;//10
	printl(r)
*/

	atom=600000-1;//max
	int i;
	int cores=getNumberOfCores();
	printl(cores)

	std::vector<std::thread> vt;

	for (i=0; i<cores; ++i){
	    vt.push_back(std::thread(f,i));
	    v.push_back(0);
	}

	for (auto& a : vt){
		a.join();
	}

	i=0;
	for (auto& a : v){
		i+=a;
	}
	printl(intToString(i))



/*
	int x=2,y;

	x=2;
	y=x++;
	printl(x,y)

	x=2;
	y=x+=1;
	printl(x,y)
*/


/*
	printsi("@@","ab",12,'*')
	printai("ab",12,'*')
	printzi("ab",12,'*')
*/

//	std::string s=fileGetContent("c:/downloads/1/a.txt");
//	printl(s.length());


}

/*
	std::string root="c:/Users/user/git/bridge/bridge/src";
	VString v;
	std::string s;
	int c=0;

	for (auto& p : recursive_directory_iterator(root)) {
		if (!is_directory(p)) {
			auto f=p.path().string();
			auto name=getFileInfo(f,FILEINFO::NAME);
			s=fileGetContent(f);
			auto vs=split(s,"\n");
			for(auto& a:vs){
				if(a.find("virtual")!=std::string::npos && a.find("virtual ~")==std::string::npos){
					g_print("%-60s %s\n",trim(a).c_str(),name.c_str());
					c++;
				}
			}
		}
	}

	printan(c);
*/

