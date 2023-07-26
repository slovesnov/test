#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>

/*
 *  for local
 *  -DPATH='"C:/Users/user/git/words"'
 *  or #define PATH "C:/Users/user/git/words"
 *  for remote
 *  -DPATH='"../htdocs"'
 *  or #define PATH "../htdocs"
 *
 *  #define cgi if need local or remote
*/

#ifdef CGI
#include <iconv.h>
#include <cstring>
#else
//for files compare
#include <sstream>
#endif

const std::string invalidDifference = "$";
std::vector<std::map<std::string, std::vector<std::string>>> eqmap;
std::string sout;

//subtract ordered strings
std::string sub(std::string const &minuend, std::string const &subtrahend) {
	if (minuend.length() < subtrahend.length()) {
		return invalidDifference;
	}
	if (minuend.length() == subtrahend.length()) {
		return minuend == subtrahend ? "" : invalidDifference;
	}

	std::string difference;
	auto p1 = minuend.c_str();
	auto p = subtrahend.c_str();
	for (; *p1; p1++) {
		if (*p1 == *p) {
			p++;
			if (!*p) {
				return difference + (p1 + 1);
			}
		} else if (*p1 < *p) {
			difference += *p1;
		} else {
			return invalidDifference;
		}
	}
	return invalidDifference;
}

std::string getOrderedString(std::string const &s) {
	auto o = s;
	std::sort(o.begin(), o.end());
	return o;
}

//get list of dictionary words from ordered string
std::string getUserString(std::string const &s) {
	auto &a = eqmap[s.length()].find(s)->second;
	bool f = true;
	std::string o;
	for (auto &e : a) {
		if (!f) {
			o += ' ';
		}
		o += e;
		f = false;
	}
	if (a.size() != 1) {
		o = '{' + o + '}';
	}
	return o;
}

//get list of dictionary words from ordered string
std::vector<std::pair<std::string, std::string>> getAllPairs(
		std::string const &s, std::string const &low = invalidDifference) {
	std::vector<std::pair<std::string, std::string>> v;
	for (size_t i = 1; i < s.size(); i++) {
		for (auto &e : eqmap[i]) {
			if (low == invalidDifference || low <= e.first) {
				auto a = sub(s, e.first);
				if (a != invalidDifference && e.first <= a) {
					auto &m = eqmap[a.length()];
					if (m.find(a) != m.end()) {
						v.push_back(
								{ getUserString(e.first), getUserString(a) });
					}
				}
			}
		}
	}
	return v;
}

//output all pairs to string
void printAllPairs(std::vector<std::pair<std::string, std::string>> const &v,
		bool p = 0) {
	bool first = true;
	if (p) {
		sout += "[";
	}
	for (auto &e : v) {
		if (first) {
			first = false;
		} else {
			sout += p ? ", " : "\n";
		}
		sout += e.first + " " + e.second;
	}
	if (p) {
		sout += "]";
	}
	sout += "\n";
}

#ifdef CGI
std::string encode(const std::string &s, bool toUtf8) {
	std::string r;
	const char UTF8[] = "UTF-8";
	const char CP1251[] = "cp1251";
	iconv_t cd = toUtf8 ? iconv_open(UTF8, CP1251) : iconv_open(CP1251, UTF8);
	if ((iconv_t) -1 == cd) {
		printf("error %d", __LINE__);
		perror("iconv_open");
		return r;
	}

	size_t inbytesleft = s.length();
	char *in = new char[inbytesleft];
	strncpy(in, s.c_str(), inbytesleft);
	size_t outbytesleft = inbytesleft * 2 + 1;
	char *out = new char[outbytesleft];
	char *outbuf = out;
	size_t ret = iconv(cd, &in, &inbytesleft, &outbuf, &outbytesleft);
	if ((size_t) -1 == ret) {
		printf("error %d", __LINE__);
		perror("iconv");
		return r;
	}
	*outbuf = 0;
	r = out;
	delete[] out;
	iconv_close(cd);
	return r;
}
#endif

int main(int argc, char *argv[]) {
	std::string s, s1, t, lng;
	size_t i, j;

#ifdef CGI
	printf("Content-type: text/html\n\n");
	std::getline(std::cin, t);
	const char *p = t.c_str() + 2; //t="s=word" so +2
	s = "";
	while (*p != 0) {
		if (*p == '%') {
			p++;
			//can be "%7B1%2C which means "{1," so cann't use strtol directly
			s1.assign(p, 2);
			s += stoi(s1, nullptr, 16);
			p += 2;
		} else {
			s += *p++;
		}
	}
	//sout = "6";
	s = encode(s, false);

#else
	clock_t begin = clock();
	if (argc == 1) {
		const int en = 0;
		const std::string chars2[] = { "авекмнорстух", "abekmhopctyx" };
		//const std::string chars2[] = { "москвамосква", "abekmhopctyx" };
		s = chars2[en];
	} else {
		s = argv[1]; //s - cp1251
		if (s.length() == 0) {
			std::cout << "error empty string";
			return 1;
		}
	}
#endif
	lng = isalpha(s[0]) ? "en" : "ru";
	const size_t size = s.length();
	eqmap.resize(size);
#ifndef CGI
	size_t sz[size];
#endif
	std::ifstream infile(std::string(PATH)+"/words/words/" + lng + "/words.txt");
	auto charset = getOrderedString(s);

	while (std::getline(infile, s)) {
		j = s.length();
		if (j < size) {
			s1 = getOrderedString(s);
			auto &m = eqmap[j];
			auto it = m.find(s1);
			if (it == m.end()) {
				t = sub(charset, s1);
				if (t != invalidDifference) {
					m.insert( { s1, { s } });
				}
			} else {
				it->second.push_back(s);
			}
		}
	}

#ifndef CGI
	sz[0] = j = 0;
	s = "";
	for (i = 1; i < size; i++) {
		sz[i] = 0;
		auto &m = eqmap[i];
		if (!m.empty()) {
			for (auto &a : m) {
				sz[i] += a.second.size();
				sz[0] += a.second.size();
			}
			j += m.size();
			s += ' ' + std::to_string(i) + ':' + std::to_string(m.size()) + '('
					+ std::to_string(sz[i]) + ')';
		}
	}
	sout += "words:" + std::to_string(j) + '(' + std::to_string(sz[0]) + ')' + s
			+ '\n';
#endif

	auto v = getAllPairs(charset);
	if (v.empty()) {
		sout += "no two items found\n";
	} else {
		printAllPairs(v);
	}

	sout += "----------------\n";
	for (i = 1; i < size; i++) {
		auto &m = eqmap[i];
		for (auto &e : m) {
			t = sub(charset, e.first);
			if (t != invalidDifference) {
				auto v = getAllPairs(t, e.first);
				if (!v.empty()) {
					sout += getUserString(e.first) + " ";
					printAllPairs(v, v.size() != 1);
				}
			}
		}
	}

#ifdef CGI
	s = encode(sout, true);
	std::cout << s;
#else
	std::ifstream fc("c:/slovesno/" + lng + ".txt");
	std::stringstream buffer;
	buffer << fc.rdbuf();
	if (sout != buffer.str()) {
		std::cerr << lng + " files are different";
	}
	std::ofstream f(lng + ".txt");
	f << sout;
	std::cout << "end " + lng + " time="
			<< float(clock() - begin) / CLOCKS_PER_SEC << " outlen="
			<< sout.length();
#endif
}
