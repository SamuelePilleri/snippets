#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "args.h"

using namespace std;

inline string join(const vector<string>& vec, const char *delim)
{
	ostringstream res;
	copy(vec.begin(), vec.end(), ostream_iterator<string>(res, delim));
	return res.str();
}

int main(int argc, char const *argv[])
{
	vector<string> ax;
	parse(argc, argv, [&ax](char const *param, const function<string()>& next){
		switch(chash(param))
		{
			case chash("-e"):
			case chash("--experimental"):
				cout << "Experimental: " << next() << endl;
				break;

			case chash("-f"):
			case chash("--flag"):
				cout << "Flag: true" << endl;
				break;

			case chash("-h"):
			case chash("--help"):
				cout << "Shows this help" << endl;
				break;
			
			case chash("-v"):
			case chash("--version"):
				cout << "Version: 2.0" << endl;
				break;
			
			default:
				ax.push_back(param);
				break;
		}
	});
	cout << "Args: " << join(ax, " ") << endl;

	printf("--------------------------------------------------------------------------------\n");

	vector<string> eax;
	parse(argc, argv, [&](char const *param, const function<string()>& next, const function<string()>& peek){
		switch(chash(param))
		{
			case chash("-e"):
			case chash("--experimental"):
				cout << "Experimental: " << peek() << endl;
				break;

			case chash("-f"):
			case chash("--flag"):
				cout << "Flag: true" << endl;
				break;

			case chash("-h"):
			case chash("--help"):
				cout << "Shows this help" << endl;
				break;
			
			case chash("-v"):
			case chash("--version"):
				cout << "Version: 2.0" << endl;
				break;
			
			default:
				eax.push_back(param);
				break;
		}
	});
	cout << "Args: " << join(eax, " ") << endl;
	return 0;
}
