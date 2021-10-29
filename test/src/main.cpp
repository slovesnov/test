#include "aslov.h"

#include <sys/stat.h>
#include <map>
#include <filesystem>
using namespace std::filesystem;

#include <iostream>

std::string ms(time_t m){
	char b[80];
	strftime(b, 80, "%d-%b-%Y %H:%M:%S", std::localtime(&m));
	return b;
}

#define SORT 1

struct S{
	std::string p;
	time_t mtime;//old
	std::string content;
	time_t mtime1;//new
	std::string content1;


	bool operator<(S& s){
#if SORT==1
		return mtime1<s.mtime1;
#else
		return mtime<s.mtime;
#endif
	}


	std::string string(int i){
		return format("%*s\t",i,p.c_str())+ms(mtime)+"\t"+ms(mtime1);
	}
};
using VS=std::vector<S>;

#define toString1 toString

int main(int argc, char *argv[]) {
	printl(toString1(123456,' ',3));
	printl(toString1(12345,' ',3));
	printl(toString1(1234,' ',3));
	printl(toString1(123,' ',3));
	printl(toString1(12,' ',3));
//	printl(toString1(123456,' ',1));
//	printl(toString1(123456));
}

int main_(int argc, char *argv[]) {

	std::string root[]={"c:/slovesno/bridgeBad","c:/Users/user/git/bridge/bridge/src"};
	std::string s,q,c;
	VS v;
	int i,j;

//	auto s="c:/Users/user/git/"+name+"/"+name+"/src";

	i=0;
	for(auto r:root){
		for (auto& p : recursive_directory_iterator(r)) {
			if (is_directory(p)) {
				continue;
			}
			auto s=p.path().string();
			c=fileGetContent(s);
			assert(s[r.length()]=='\\');

			struct stat result;
			stat(s.c_str(), &result);
			auto mod_time = result.st_mtime;

			auto sh=s.substr(r.length()+1);
			if(i==0){
				v.push_back( { sh, mod_time, c });
			}
			else{
				auto it = std::find_if(v.begin(), v.end(), [&sh](const S &x) {
					return x.p == sh;
				});

				//it==v.end()
				//not found helper\PreferansScore.cpp 10/Oct/2021 01:04:58
				//not found helper\PreferansScore.h 10/Oct/2021 15:51:05
				if(it!=v.end()){
					it->mtime1=mod_time;
					it->content1=c;

				}

			}
		}
		i++;
	}


	std::sort(v.begin(),v.end());

	i=0;
	for(auto a:v){
		i=std::max(i,int(a.p.length()));
	}

	//12-Oct-2021 19:01:28 73 1634054488
	time_t AT=1634054488;
	j=1;
	std::map<time_t,int> map;
	for(auto a:v){
		if(a.content!=a.content1 || a.mtime1!=AT){
			continue;
		}
		auto it=map.find(a.mtime1);
		if(it==map.end()){
			map[a.mtime1]=1;
		}
		else{
			it->second++;
		}
		s=format("%3d",j);
		printan(s,a.content==a.content1,a.string(i));
		j++;
	}

	for (auto& [time, n]: map) {
		printan(ms(time),n,time);
	}


/*
	std::string s="c:\\Users\\user\\git\\stopwatch\\stopwatch\\Release\\stopwatch.exe";

	struct stat result;
	stat(s.c_str(), &result);
	auto m = result.st_mtime;

	char b[80];
	strftime(b, 80, "%m/%d/%Y %H:%M:%S", std::localtime(&m));
	printzi('[',b,']')

	strftime(b, 80, "%d %b %Y %H:%M:%S", std::localtime(&m));
	printzi('[',b,']')
*/

	return 0;

}
