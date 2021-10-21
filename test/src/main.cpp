#include "aslov.h"

#define ASLOV_PRINT_VARIABLE(a) printan(formatz(#a," = ",a))
#define ASLOV_PRINT_VARIABLE_I(a) printai(formatz(#a," = ",a))
#define ASLOV_CONCATENATE(a, b)   a##b
#define ASLOV_FOR_EACH_NARG(...) ASLOV_FOR_EACH_NARG_(__VA_ARGS__, ASLOV_FOR_EACH_RSEQ_N())
#define ASLOV_FOR_EACH_NARG_(...) ASLOV_FOR_EACH_ARG_N(__VA_ARGS__)
#define ASLOV_FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define ASLOV_FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ASLOV_FOR_EACH_1(f,f1,x, ...) f1(x)
#define ASLOV_FOR_EACH_2(f,f1,x, ...) f(x)ASLOV_FOR_EACH_1(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH_3(f,f1,x, ...) f(x)ASLOV_FOR_EACH_2(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH_4(f,f1,x, ...) f(x)ASLOV_FOR_EACH_3(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH_5(f,f1,x, ...) f(x)ASLOV_FOR_EACH_4(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH_6(f,f1,x, ...) f(x)ASLOV_FOR_EACH_5(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH_7(f,f1,x, ...) f(x)ASLOV_FOR_EACH_6(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH_8(f,f1,x, ...) f(x)ASLOV_FOR_EACH_7(f,f1,__VA_ARGS__)
#define ASLOV_FOR_EACH(N, f,f1,x, ...) ASLOV_CONCATENATE(ASLOV_FOR_EACH_,N)(f,f1,x, __VA_ARGS__)
#define printv(...) ASLOV_FOR_EACH(ASLOV_FOR_EACH_NARG(__VA_ARGS__), ASLOV_PRINT_VARIABLE, ASLOV_PRINT_VARIABLE, __VA_ARGS__)
#define printvi(...) ASLOV_FOR_EACH(ASLOV_FOR_EACH_NARG(__VA_ARGS__), ASLOV_PRINT_VARIABLE, ASLOV_PRINT_VARIABLE_I, __VA_ARGS__)

struct D{
	int a;
	std::string b;
	D(){
		a=4;
		b="df";
	}
};
std::ostream& operator <<(std::ostream& o, const D& d) {
	return o << d.a<<" "<<d.b;
}

int main(int argc, char *argv[]) {
	int i = 3;
	std::string s="abc";
	char c='#';

	printv(i,c,s)
	printvi(s)

	D d;
	printv(d.a,d.b)
	printvi(d);

}
