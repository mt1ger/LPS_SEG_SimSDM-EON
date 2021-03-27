#ifndef _H_TXTPARSE
#define _H_TXTPARSE

#include <cmath>
#include <string>
#include <vector>

using namespace std;

class txtParse {
	public:
		txtParse () {}
		~txtParse () {}
		void parse (vector<int> &PotentialCore, vector<int> &PotentialErlang, vector<int> &PatentialSeed, double Mu, string path);
};

#endif

