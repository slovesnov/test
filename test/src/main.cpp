#include "aslov.h"

#include <iostream>

int main(int argc, char *argv[]) {

	int ar[]={80407
			,977
			,23345
			,49263
			,22306
			,8207
			,251
//			,0
//			,0
//			,0
//			,0
	};

/*
	<table class="single"><tbody>
	<tr><th>число взяток</th><th>раскладов</th><th>вероятность</th></tr>
	<tr><td>t=0</td><td>80 407</td><td>p<sub>0</sub>=43.52%</td><tr>
	<tr><td>t=1</td><td>977</td><td>p<sub>1</sub>=0.53%</td><tr>
	<tr><td>t=2</td><td>23 345</td><td>p<sub>2</sub>=12.64%</td><tr>
	<tr><td>t=3</td><td>49 263</td><td>p<sub>3</sub>=26.66%</td><tr>
	<tr><td>t=4</td><td>22 306</td><td>p<sub>4</sub>=12.07%</td><tr>
	<tr><td>t=5</td><td>8 207</td><td>p<sub>5</sub>=4.44%</td><tr>
	<tr><td>t=6</td><td>251</td><td>p<sub>6</sub>=0.14%</td><tr>
	<tr><td>t=7</td><td>0</td><td>p<sub>7</sub>=0%</td><tr>
	<tr><td>t=8</td><td>0</td><td>p<sub>8</sub>=0%</td><tr>
	<tr><td>t=9</td><td>0</td><td>p<sub>9</sub>=0%</td><tr>
	<tr><td>t=10</td><td>0</td><td>p<sub>10</sub>=0%</td><tr>
	<tr><th>всего</th><td>184 756</td><td>100%</td><tr>
	</tbody></table>
*/

	std::string s;
	int i,t,p=3;
	double v,q[]={0,0};

	s+="<table class=\"single tc\"><tbody>\n";
	s+=format("<tr><th colspan=\"%d\" align=\"center\">%s игрока</th></tr>\n",p+1,p==3?"три":"четыре");
	s+="<tr>";
	s+="<th></th>";
	s+="<th>мизерист</th>";
	for(i=1;i<p;i++){
		s+=format("<th>ловец%d</th>",i);
	}
	s+="</tr>\n";

	s+="<tr>";
	s+="<th></th>";
	for(i=0;i<p;i++){
		s+=format("<th>S<sub>%d</sub>(t)</th>",i,p==3?2:0);
	}
	s+="</tr>\n";

	for(t=0;t<7;t++){
		s+=format("<tr><td>t=%d</td>",t);
		if(t==0){
			v=100.*(p-1)/p;
		}
		else{
			v=-100.*t*(p-1)/p;
		}
		q[0]+=ar[t]*v/184756;
		for(i=0;i<p;i++){
			s+=format("<td>%.*lf</td>",p==3?2:0,v);
			if(i==0){
				v=-v/(p-1);
				q[1]+=ar[t]*v/184756;
			}
		}
		s+="</tr>\n";
	}
	s+="</tbody></table>\n";

	printzn(s);

	//printa(q[0],q[1])
}
