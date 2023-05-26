#include"testlib.h"
#include<bits/stdc++.h>
using namespace std;

int main(int argc, char** argv){
	registerTestlibCmd(argc, argv);
	const string sECReT_oUtPUT = "a9100fda0a7059a979d560b8550f715d4ee391ff9b8b680b2f87b26a69ee5a5e";
	if(ouf.readToken() != sECReT_oUtPUT) quitf(_wa, "Wrong Output Format(Program is killed unexpectedly).");
	int T=inf.readInt();
	while (T--){
		double usr_ans = ouf.readDouble(-1-1e-9, 1e18);
		double std_ans = ans.readDouble(-1-1e-9, 1e18);
		if (fabs(usr_ans - std_ans) > 1e-6 * max(1.0, std_ans))
			 quitf(_wa, "Wrong Answer.");
	}
	if(ouf.readToken() != sECReT_oUtPUT) quitf(_wa, "Wrong Output Format(Program is killed unexpectedly).");
	quitf(_ok, "Correct.");
	return 0;
}
