#include <iostream>
#include <set>
#include <functional>
#include <algorithm>
#include <string> 

using namespace std;

int main() {
	string s;
	std::set<string,std::less<string> > dict;
	while (cin >> s) {
		if (s.empty()) continue;
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	    string res;
	    for (int i = 0; i < s.size(); ++i)
	    {
	    	char p = s[i];
	    	if (!::isalpha(p) || p == '\n'){
	    		if (!res.empty()) dict.insert(res);
	    		res.clear();
	    	}
	    	else {
	    		res+=p;
	    	}
	    }
	    if (!res.empty()) dict.insert(res);
	}
	for(const auto &f : dict) {
		cout << f << endl;
	}
	return 0;
}