#include <iostream>
#include <set>
#include <cmath>
#include <filesystem>
#include <regex>
#include <sys/stat.h>
#include "aslov.h"
#include "Permutations.h"

using uchar = unsigned char;
#define STR(n) toString(n,'.')
#define OUT_START(s) std::cout <<LNG<<" "<<__func__<<" "<<s<<"\n"<< std::flush;

struct WM {
	std::string word;
	uint32_t mask;
};

//user predefined constants
const bool EN = 1;
/* 1, 2, 3 step1, 2|2p, 3
 * 4 - checkSameMaskLength()
 * 5 - check5() recursive search (letters can repeat) use MAX_WORDS.
 * 6 - check6() recursive search all letters are different use MAX_WORDS.{
 * EN MAX_WORDS<5 not found. MAX_WORDS=5 found
 * RU MAX_WORDS<5 not found. MAX_WORDS=5 found
 * check for MAX_WORDS=5,6
 * }
 */
const int OPTION = 4;
const int MAX_WORDS = 6;//for OPTION=5|6
const int STEP3_MAXL = EN ? 29 : 35;//for OPTION=3
const int MAX_SUM_LENGTH = MAX_WORDS <= 3 ? INT_MAX : (EN ? 34 : 42);//for OPTION=5
const bool CUT_ON_START = 0;//for OPTION=5

//for OPTION=5 need when cann't cover all alphabet for MAX_WORDS=2 || MAX_WORDS=3 && EN
//#define USE_MIN_LETTERS

#ifdef USE_MIN_LETTERS
const int MIN_LETTERS = EN ? (MAX_WORDS==2?21:25) : 27;
#endif

//ifdef MOST_POPULAR works slower
//#define MOST_POPULAR

/*
 * MAX_WORDS=2 EN=1
	demographiques intersubjectively bits=21 fkwxz suml=31
	if check all patterns848 chars21 time0.3

 * MAX_WORDS=2 EN=0
	вертикально-подъемный южуралэнергостроймеханизация bits=27 бфчшщ suml=49
	подъелдыкивать южуралэнергостроймеханизация bits=27 бфчшщ suml=42
	if check all patterns4 chars28 time4.2

 * MAX_WORDS=3 EN=1 bits=25
 	 many patterns

 * MAX_WORDS=3 EN=0 all letters
 	 many patterns
 *
 */

const int LETTERS = EN ? 26 : 32;
const uint32_t END_MASK = ((1ull << LETTERS) - 1);
const uchar FIRST_CHAR = EN ? 'a' : 'а';
const std::string LNG = EN ? "en" : "ru";

std::vector<WM> letters[LETTERS];
std::string allwords[MAX_WORDS];
std::map<uint32_t,std::string> allmap;
clock_t begin;
int first_index;
std::ofstream filecheck;
int gfound=0;

#include "pangrams.h"

void check5(int level, uint32_t mask, uint32_t leftlen) {
	int i, j = 0, min = INT_MAX;
	if(level==-1){
		j=first_index;
	}
	else{
		for (i = 0; i < LETTERS; i++) {
			if (!((mask >> i) & 1)) {
				if (min > int(letters[i].size())) {
					min = int(letters[i].size());
					j = i;
				}
			}
		}
	}
	level++;
	auto& l=letters[j];
	for (auto &e : l) {
		if (e.word.length() <= leftlen) {
			uint32_t m = mask | e.mask;
#ifdef USE_MIN_LETTERS
			int bits = 0;
#endif
			std::string s;
			allwords[level] = e.word;
			if (m == END_MASK
#ifdef USE_MIN_LETTERS
					|| (bits = __builtin_popcount(m)) >= MIN_LETTERS
#endif
							) {
				for (i = j = 0; i <= level; i++) {
					j += allwords[i].length();
					s += allwords[i] + " ";
				}
				if (j <= MAX_SUM_LENGTH ) {
#ifdef USE_MIN_LETTERS
					if (bits) {
						s += "bits=" + std::to_string(bits) + " ";
						for (i = 0; i < LETTERS; i++) {
							if (!((m >> i) & 1)) {
								s += char(FIRST_CHAR + i);
							}
						}
						s += ' ';
					}
#endif
					s += "suml=" + std::to_string(j) + "\n";
					filecheck << s;
					std::cout << s << std::flush;
					gfound++;
				}
				continue;
			}

			if (level == MAX_WORDS - 1) {
				continue;
			}
			check5(level, m, leftlen-e.word.length());
			if (level == 0) {
				static int g = 0;
				g++;
				if ((MAX_WORDS==3 && g % 100 == 0) || MAX_WORDS>3) {
					std::cout<< e.word<<" ";
					outTimeInfo(g, letters[first_index].size());
				}
			}
		}
	}
}

void check6(int level,uint32_t mask) {
	int i;
	if (level == MAX_WORDS - 2) {
		mask = ~mask;
		if (EN) {
			mask &= END_MASK;
		}
		auto it = allmap.find(mask);
		if (it != allmap.end()) {
			std::string s;
			gfound++;
			for (i = 0; i <= level; i++) {
				s += allwords[i] + " ";
			}
			s += it->second + "\n";
			filecheck << s;
			std::cout << s << std::flush;
		}
	}
	else{
		int j = 0, min = INT_MAX;
		if(level==-1){
			j=first_index;
		}
		else{
			for (i = 0; i < LETTERS; i++) {
				if (!((mask >> i) & 1)) {
					if (min > int(letters[i].size())) {
						min = int(letters[i].size());
						j = i;
					}
				}
			}
		}

		level++;
		if (level == MAX_WORDS - 1) {
			return;
		}
		for (auto &e : letters[j]) {
			if ((mask & e.mask) == 0) {
				allwords[level] = e.word;
//				if(level==0){
//					static int g = 0;
//					if(g++%(EN?70:20)==0){
//						std::cout<< e.word<<" ";
//						outTimeInfo(g, letters[first_index].size());
//					}
//				}
				check6(level, mask| e.mask);
			}
		}
	}
}


int main() {
	int i;
	std::string s;
	uchar u;
	uint32_t mask;
	std::set<char> sc;
	std::map<uint32_t, std::string> lettersMap[LETTERS];

	preventThreadSleep();

	begin = clock();
	if (OPTION == 1) {
		step1();
		return 0;
	}
	if (OPTION == 2) {
		step2p();
		//step2();
		return 0;
	}
	if (OPTION == 3) {
		step3();
		return 0;
	}
	if (OPTION == 4) {
		checkSameMaskLength();
		return 0;
	}

	s = LNG + format(" max_words=%d max_sum_length=%d cut_on_start=%d option=%d", MAX_WORDS,
			MAX_SUM_LENGTH, int(CUT_ON_START),OPTION);
#ifdef USE_MIN_LETTERS
	s += format(" min_chars=%d", MIN_LETTERS);
#endif
	s += "\n";
	std::cout << s << std::flush;

#ifdef MOST_POPULAR
	int most_popular = EN?4:14;//en-e ru-о
#endif

	std::ifstream infile(getDicionaryFileName());
	while (std::getline(infile, s)) {
		mask = 0;
		sc.clear();
		for (char c : s) {
			u = uchar(c);
			if (u != '-') {
				sc.insert(c);
				mask |= 1 << (u - FIRST_CHAR);
			}
		}

		if (OPTION == 6) {
			if (__builtin_popcount(mask) != int(s.length())) {
				continue;
			}
			allmap.insert( { mask, s });
		}

#ifdef MOST_POPULAR
		if( mask & (1<<most_popular)){
			u = uchar(FIRST_CHAR+most_popular);
			auto &m = lettersMap[u - FIRST_CHAR];
			auto const &it = m.find(mask);
			if (it == m.end()) {
				m[mask] = s;
			} else {
				if (it->second.length() > s.length()) {
					it->second = s;
				}
			}

		}
		else{
#endif
			for (char c : sc) {
				u = uchar(c);
				auto &m = lettersMap[u - FIRST_CHAR];
				auto const &it = m.find(mask);
				if (it == m.end()) {
					m[mask] = s;
				} else {
					if (it->second.length() > s.length()) {
						it->second = s;
					}
				}
			}
#ifdef MOST_POPULAR
		}
#endif
	}

	i = 0;
	for (auto &m : lettersMap) {
		auto &l = letters[i];
		for (auto &e : m) {
			l.push_back( { e.second, e.first });
		}
		//sort for alphabet output
		std::sort(l.begin(), l.end(), [](auto &a, auto &b) {
			return a.word < b.word;
		});
		i++;
	}

#ifdef MOST_POPULAR
	first_index = most_popular;
#else
auto it = std::min_element(std::begin(letters), std::end(letters),
		[](auto &a, auto &b) {
			return a.size() < b.size();
		});
first_index = std::distance(std::begin(letters), it);
#endif
	if (CUT_ON_START && OPTION==5) {
		std::vector<WM> v;
		VString vs;
		if (EN) {
			vs = { "consequently", "democratique" };
		} else {
			vs = {
					"аэрофотосъемочный",
//					"карчеподъемный",
//					"конъюнктурщик",
//					"аэрофотосъемка"
			};
		}
		for (auto &m : letters[first_index]) {
			if (oneOf(m.word, vs)) {
				v.push_back(m);
			}
		}
		letters[first_index] = v;
		if (v.size()!=vs.size()) {
			printel("some words not found")
#ifdef MOST_POPULAR
printel("CUT_ON_START not working with MOST_POPULAR")
#endif
			exit(1);
		}
	}

	filecheck.open(LNG + ".txt");
	if (OPTION == 5 ) {
		check5(-1, 0, MAX_SUM_LENGTH);
	}
	else{
		check6(-1, 0);
	}
	filecheck.close();
	std::cout<<"===== found "<<STR(gfound)<<"=====\n";
	checkSameMaskLength();

	std::cout << "end time " << secondsToString(begin) << std::flush;
}

