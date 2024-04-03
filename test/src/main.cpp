#include <cmath>
#include <iostream>
#include <cstdio>
#include "aslov.h"
#include <cstring>


const int en=0;
//const std::string w="прислоняться";
const std::string m_entryText="попадья";
//const std::string m_entryText="абосло";


typedef unsigned char uchar;

bool checkOrderedWord(const std::string &s, const int* a, size_t l) {
	size_t i, j = 0;
	for (i = 0; i < s.length(); i++) {
		auto v = a[j*256+uchar(s[i])];
		if (!v || ((j += v) >= l && i+1<s.length())) {
			return false;
		}
	}
	return true;
}

int main() {
	std::string s, s1,lng=en ? "en" : "ru";
	std::ifstream infile(
			"C:/Users/user/git/words/words/words/" + lng + "/words.txt");
	size_t i,j;
	bool b;
	int* m_template_a=new int[m_entryText.length()*256];

	for(j=0;j<m_entryText.length();j++){
		auto a=m_template_a+j*256;
		memset(a,0,256);
		s=m_entryText.substr(j);
		for (i = 0; i < s.length(); i++) {
			uchar u = s[i];
			if (!a[u]) {
				a[u] = i + 1;
			}
		}
	}
//	b=checkOrderedWord("бо",m_template_a,m_entryText.length());
//	printl(b);
//	exit(1);

	i=0;
	while (std::getline(infile, s)) {
		//auto l = s.length();
//		printl(s)
//		fflush(stdout);
		if(checkOrderedWord(s,m_template_a,m_entryText.length())){
			printl(s)
					i++;
		}

	}
	printl("words",i)

	delete[]m_template_a;

}

