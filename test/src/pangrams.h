/*
 * pangrams.h
 *
 *  Created on: 05.11.2023
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef PANGRAMS_H_
#define PANGRAMS_H_


bool fileExists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

std::string fnameStep2(int n){
	return LNG + "/" + LNG + std::to_string(n) + ".bin";
}

std::vector<int> zeroBits(uint32_t n) {
	std::vector<int> v;
	for(int i=0;i<LETTERS;i++,n>>=1){
		if(!(n&1)){
			v.push_back(i);
		}
	}
	return v;
}

std::string getDicionaryFileName(){
	return "C:/Users/user/git/words/words/words/" + LNG + "/words.txt";
}

std::string ns(uint64_t n){
	if(n<1000){
		return std::to_string(n);
	}
	if(n<1'000'000){
		return std::to_string(n/1000)+'k';
	}
	return std::to_string(n/1'000'000)+'m';
}

void outTimeInfo(int i, int n, bool linear = false) {
	double t = double(i) / n;
	double p = 100. * (linear ? t : (1 - std::pow(1 - t, 2)));
	t = timeElapse(begin);
	/*
	 t p%
	 T 100%
	 T=t*100/p
	 left T-t=t(100/p-1)
	 */
	std::cout << format("%.1lf%% ", p) << secondsToString(t) << " left "
			<< (p ? secondsToString(t * (100 / p - 1)) : "?") << " now "
			<< timeToString("%H:%M:%S\n") << std::flush;
}

//store bitmasks to files
void step1() {
	/* bool onlyshort
	 * ru onlyshort=0 276.689, onlyshort=1 222.498
	 * */
	int i, n;
	std::set<uint32_t> set[LETTERS + 1],all;
	uint32_t mask,m;
	uchar u;
	std::string s;
	std::set<uint32_t>::const_iterator it,it1;

	std::ifstream infile(getDicionaryFileName());
	while (std::getline(infile, s)) {
		mask = 0;
		for (char c : s) {
			u = uchar(c);
			if (u != '-') {
				mask |= 1 << (u - FIRST_CHAR);
			}
		}
		//if(!onlyshort || int(s.length())<=numberOfSetBits(mask)+2){
			all.insert( mask );
		//}
	}

	n = int(all.size());
	OUT_START(STR(n))

	//TODO iterate over vector should be much faster than iterate over map, see step3() code
	i=0;
	for(it=all.begin();it!=all.end();it++,i++){
		if (((i+1) % (EN?10'000:2'000)) == 0) {
			outTimeInfo(i,n);
		}
		m=*it;
		it1=it;
		it1++;
		for(;it1!=all.end();it1++){
			//TODO mask!=m && mask!=*it1
			mask = m | *it1;
			set[__builtin_popcount(mask)].insert(mask);
		}
	}

	i = -1;
	for (auto &e : set) {
		i++;
		if (e.empty()) {
			continue;
		}
		std::ofstream f(LNG + std::to_string(i) + ".bin",
				std::ofstream::binary);
		for (auto &v : e) {
			f.write((char*) &v, sizeof(v));
		}
	}

	std::cout << "end time " << secondsToString(begin) << std::flush;
}

//store pairs of bitmasks to files
void step2() {
	const std::filesystem::path path { LNG };
	int i, j, k, l, n, t = 0;
	std::string s;
	uint32_t mask;
	std::vector<uint32_t> vc[LETTERS + 1];
	std::ofstream f;

	OUT_START("")

	for (auto const &e : std::filesystem::directory_iterator { path }) {
		i = e.file_size() / 4;
		t += i;
		s = e.path().string();
		if (e.path().extension() != ".bin") {
			continue;
		}
		std::smatch cm;
		std::regex r { "\\d+" };
		std::regex_search(s, cm, r);
		n = stoi(cm[0]);
		std::cout << n << " " << STR(i) << '\n';

		std::ifstream rf(s, std::ios::in | std::ios::binary);
		i = 0;
		while (rf.read((char*) &mask, sizeof(mask))) {
			vc[n].push_back(mask);
			i++;
		}
	}

	f.open(LNG + "templates.bin", std::ofstream::binary);
	t = 0;
	for (k = LETTERS / 2 + LETTERS % 2; k <= LETTERS; k++) {
		auto &e = vc[k];
		n = int(e.size());
		std::cout << "try" << k << " size" << STR(n) << "\n"
				<< std::flush;
		for (i = 0; i < n; i++) {
			if ((i + 1) % 250'000 == 0) {
				outTimeInfo(i, n);
			}
			for (j = i + 1; j < n; j++) {
				if ((e[i] | e[j]) == END_MASK) {
					f.write((char*) &e[i], sizeof(uint32_t));
					f.write((char*) &e[j], sizeof(uint32_t));
					t++;
				}
			}

		}
	}

	f.open(LNG + "templates1.bin", std::ofstream::binary);
	t = 0;
	for (k = 0; k <= LETTERS; k++) {
		auto &e = vc[k];
		n = int(e.size());
		std::cout << "try" << k << " size" << STR(n) << "\n"
				<< std::flush;
		for (i = 0; i < n; i++) {
			if ((i + 1) % 250'000 == 0) {
				outTimeInfo(i, n);
			}
			//l>=k+1 (because of symmetry) && l+k>=LETTERS
			for (l = std::max(k + 1, LETTERS - k); l <= LETTERS; l++) {
				auto &e1 = vc[l];
				int n1 = int(e1.size());
				for (j = 0; j < n1; j++) {
					if ((e[i] | e1[j]) == END_MASK) {
						f.write((char*) &e[i], sizeof(uint32_t));
						f.write((char*) &e1[j], sizeof(uint32_t));
						t++;
					}
				}
			}
		}
	}
	std::cout << "end time " << secondsToString(begin) << std::flush;
}

//output quads of strings
void step3() {
	int i, j, n, min[2],max[2];
	uint32_t mask[2],m;
	std::vector<std::pair<uint32_t, uint32_t>> v;
	//faster if use one map
	std::map<uint32_t, std::vector<std::pair<int, int>>> map;
	std::string s;
	uchar u;
	std::map<uint32_t, std::string> allm;
	std::vector<WM> allv;

	std::ifstream infile(getDicionaryFileName());
	while (std::getline(infile, s)) {
		m = 0;
		for (char c : s) {
			u = uchar(c);
			if (u != '-') {
				m |= 1 << (u - FIRST_CHAR);
			}
		}
		auto const& it=allm.find(m);
		if(it==allm.end()){
			allm[m]=s;
		}
		else{
			if(it->second.length()>s.length()){
				it->second=s;
			}
		}
	}

	std::ifstream rf(LNG+"templates.bin", std::ios::in | std::ios::binary);
	if (!rf.is_open()) {
		printei;
		return;
	}
	while (rf.read((char*) mask, sizeof(uint32_t) * 2)) {
		v.push_back( { mask[0], mask[1] });
		map.insert( { mask[0], { } });
		map.insert( { mask[1], { } });
	}

	//it's fast operation
	for(auto&e:allm){
		allv.push_back({e.second,e.first});
	}

	OUT_START(STR(allm.size())+" "+STR(map.size())+" "+STR(v.size()) )

	//iterate over vector much faster than iterate over map (about 10 times faster)
	n=int(allv.size());
	for (i = 0; i < n; i++) {
		if ((i + 1) % 25'000 == 0) {
			outTimeInfo(i, n);
		}
		for (j = i + 1; j < n; j++) {
			auto const &it = map.find(allv[i].mask | allv[j].mask);
			if (it != map.end()) {
				it->second.push_back( { i, j });
			}
		}
	}
	std::cout
			<< format("time%.1lfmin\n", (clock() - begin) / 60. / CLOCKS_PER_SEC)
			<< std::flush;

	std::ofstream f(LNG + ".txt");
	for (auto &e : v) {
		uint32_t a[] = { e.first, e.second };
		for(i=0;i<2;i++){
			min[i]= INT_MAX;
			max[i]=INT_MIN;
		}

		i = -1;
		for (auto &el : a) {
			i++;
			for (auto &p : map[el]) {
				j = allv[p.first].word.length() + allv[p.second].word.length();
				if (j < min[i]) {
					min[i] = j;
				}
				if(j>max[i]){
					max[i]=j;
				}
			}
		}
		n = min[0] + min[1];
		if (n <=STEP3_MAXL) {
			s = std::to_string(n);
			i = -1;
			for (auto &el : a) {
				i++;
				s += "{";
				for (auto &p : map[el]) {
					j = allv[p.first].word.length() + allv[p.second].word.length();
					if (j + min[!i] <= STEP3_MAXL) {
						s += allv[p.first].word + " " + allv[p.second].word
								+ std::to_string(j);
					}
				}
				s += "}";
			}
			s += "\n";
			std::cout << s;
			f << s;
		}
	}
	std::cout << "end time " << secondsToString(begin) << std::flush;
}

void loadMasks(int n, std::set<uint32_t>& m) {
	uint32_t mask;
	std::vector<uint32_t> v;
	m.clear();
	std::ifstream f(fnameStep2(n),
			std::ios::in | std::ios::binary);
	while (f.read((char*) &mask, sizeof(mask))) {
		v.push_back(mask);
	}
	std::copy(v.begin(), v.end(), std::inserter(m, m.begin()));
}

void step2p() {
	int i, j, k, l, t;
	std::string s;
	uint32_t mask, mask1;
	std::ofstream f;
	std::set<uint32_t> ms[2];
	std::vector<int> bi;

	OUT_START("")

	f.open(LNG + "templates.bin", std::ofstream::binary);
	t = 0;
	//can easy parallelized/split by k values
	for (k = 0; k < 4; k++) {
		for (i = 1; i <= LETTERS / 2; i++){
			j = LETTERS - i + k;
			//better to not open all the files right away, because it takes a lot of memory
			if (fileExists(fnameStep2(i)) && fileExists(fnameStep2(j))) {
				loadMasks(i, ms[0]);
				loadMasks(j, ms[1]);

				std::cout << format("%d/%d ", i, j) << ns(ms[0].size()) << "/"
						<< ns(ms[1].size()) << " t" << STR(t) << " "
						<< secondsToString(begin) << "\n" << std::flush;
				for (auto &e : ms[0]) {
					mask = ~e;
					if (EN) {
						mask &= END_MASK;
					}
					if (k) {
						mask1 = mask;
						bi = zeroBits(mask);
						Permutations p(k, bi.size(), Permutations::COMBINATION);
						for (auto &v : p) { //v is std::vector<int>
							mask = mask1;
							for (l = 0; l < k; l++) {
								mask |= 1 << bi[v[l]];
							}
							if (ms[1].find(mask) != ms[1].end()) {
								t++;
								f.write((char*) &e, sizeof(uint32_t));
								f.write((char*) &mask, sizeof(uint32_t));
							}
						}
					}
					else {
						if (mask > e && ms[1].find(mask) != ms[1].end()) {
							t++;
							f.write((char*) &e, sizeof(uint32_t));
							f.write((char*) &mask, sizeof(uint32_t));
						}
					}
				}
			}
		}
	}

	std::cout << "t" << STR(t)<< " end time " << secondsToString(begin) << std::flush;
}

uint32_t getMask(std::string s){
	uint32_t mask = 0;
	uchar u;
	for (char c : s) {
		u = uchar(c);
		if (u != '-') {
			mask |= 1 << (u - FIRST_CHAR);
		}
	}
	return mask;
}

void checkSameMaskLength(){
	std::ifstream f(LNG + ".txt", std::ios::in );
	std::string s, s1;
	std::vector<VString> so;
	std::map<uint32_t,VString> m;
	uint32_t mask;
	std::ofstream of;
	int i,j;

	if(!f.is_open()){
		printei
		return;
	}
	while (std::getline(f, s)) {
		so.push_back(split(s));
		std::sort(so.back().begin(),so.back().end());
		for (auto &e : so.back()) {
			mask = getMask(e);
			m[mask]={e};
		}
	}
	std::sort(so.begin(),so.end(),[](auto&a,auto&b){
		size_t i;
		for(i=0;i<a.size() && a[i]==b[i];i++);
		return a[i]<b[i];
	});

	std::ifstream infile(getDicionaryFileName());
	while (std::getline(infile, s)) {
		mask = getMask(s);
		auto it=m.find(mask);
		if(it!=m.end() && s!=it->second[0] && s.length()==it->second[0].length()){
//			std::cout<<forma(s,it->second[0])<<"\n";
			it->second.push_back(s);
		}
	}

/*
	s= "<table class=\"single\">\n";
	for(auto&e1:so){
		s+="<tr>";
		for(auto&e:e1){
			auto v=m[getMask(e)];
			s+=std::string("<td>")+(v.size()>1?"{":"")+joinV(v," ")+(v.size()>1?"}":"");
		}
		s+="\n";
	}
	of.open(LNG + ".html" );
	of<<s<<"</table>\n";
	//if not do close then next open not give results
	of.close();
*/

/*
	s = "gd=[";
	i = 0;
	for (auto &e1 : so) {
		if(i){
			s+=",";
		}
		s += "[";
		j=0;
		for (auto &e : e1) {
			auto v = m[getMask(e)];
			if(j){
				s+=",";
			}
			s += std::string("'") + (v.size() > 1 ? "{" : "") + joinV(v, " ")
					+ (v.size() > 1 ? "}" : "") + "'";
			j++;
		}
		s += "]";
		i++;
	}
	s += "]";
	of.open( "p2data"+std::string(EN?"1":"")+".js");
	s1=localeToUtf8(s);
	printl(of.is_open(), STR(s.length()), STR(s1.length()))
	of << s1;
*/
	s = "";
	i = 0;
	for (auto &e1 : so) {
		if(i){
			s+="\n";
		}
		j=0;
		for (auto &e : e1) {
			auto v = m[getMask(e)];
			if(j){
				s+=" ";
			}
			s += v.size() > 1 ? "{" : "";
			s1=joinV(v, " ");
			if(EN){
				s+=s1;
			}
			else{
				for (auto &e : s1) {
					s += e == ' ' ?' ':char(uchar(e) - FIRST_CHAR + 'A');
				}
			}
			s+=v.size() > 1 ? "}" : "";
			j++;
		}
		i++;
//		if(i==3){
//			break;
//		}
	}
	//binary to make \n as one char
	of.open( "p2data"+LNG+".txt",std::ios::binary);
	printl(STR(s.length()))
	of << s;

	std::cout<<"templates "<<STR(so.size())<<"\n";
}

#endif /* PANGRAMS_H_ */
