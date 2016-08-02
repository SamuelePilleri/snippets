#include <iostream>
#include <functional>

// Only requested for ax in main and join
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

inline string join(const vector<string>& vec, const char* delim)
{
	ostringstream res;
	copy(vec.begin(), vec.end(), ostream_iterator<string>(res, delim));
	return res.str();
}

/**
 * Compile time string hashing.
 *
 * If using inside a switch-case statement, just don't care about collisions
 * since, if any, they are reported as an error by the compiler.
 *
 * See: https://stackoverflow.com/questions/2111667/compile-time-string-hashing
 */
constexpr unsigned int chash(char const *str)
{
	return *str ?
		(static_cast<unsigned int>(*str) + 33 * chash(str + 1)) :
		5381;
}

void parse(int argc, char const *argv[], function<void( char const *param, function<string()> next )>&& callback )
{
	int i = 1;
	
	auto n = [&i, &argc, &argv]() -> string {
		return (i < argc) ? argv[i++] : "";
	};
	
	while(i < argc)
	{
		callback(argv[i++], n);
	}
}

void parse(int argc, char const *argv[], function<void( char const *param, function<string()> next, function<string()> peek )>&& callback )
{
	int i = 1;
	
	auto n = [&]() -> string {
		return (i < argc) ? argv[i++] : "";
	};

	auto p = [&]() -> string {
		return (i < argc) ? argv[i] : "";
	};
	
	while(i < argc)
	{
		callback(argv[i++], n, p);
	}
}

int main(int argc, char const *argv[])
{
	vector<string> ax;
	parse(argc, argv, [&ax](char const *param, function<string()> next){
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
				ax.push_back(string(param));
				break;
		}
	});
	cout << "Args: " << join(ax, " ") << endl;

	// -------------------------------------------------------------------------
	printf("--------------------------------------------------------------------------------\n");

	vector<string> eax;
	parse(argc, argv, [&](char const *param, function<string()> next, function<string()> peek){
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
				eax.push_back(string(param));
				break;
		}
	});
	cout << "Args: " << join(eax, " ") << endl;
	return 0;
}
