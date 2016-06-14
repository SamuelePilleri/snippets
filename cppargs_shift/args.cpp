#include <iostream>
#include <functional>

// Only requested for ax in main and join
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

string join(const vector<string>& vec, const char* delim)
{
    ostringstream res;
    copy(vec.begin(), vec.end(), ostream_iterator<string>(res, delim));
    return res.str();
}

void parse(int argc, const char *argv[], function<void( string&& param, function<string()> next )>&& callback )
{
	int i = 1;
	
	auto n = [&i, &argc, &argv]() -> string {
		return (i < argc) ? string(argv[i++]) : "";
	};
	
	while(i < argc)
	{
		callback(string(argv[i++]), n);
	}
}

int main(int argc, const char *argv[])
{
	vector<string> ax;
	parse(argc, argv, [&ax](string&& param, function<string()> next){
		/*switch(param)
		{
			case "-h":
			case "--help":
				break;
			
			case "-v":
			case "--version":
				break;
			
			default:
				break;
		}*/
		if(param == "-e" || param == "--experimental")
			cerr << "-e: " << next() << endl;
		else if(param == "-f" || param == "--flag")
			cerr << "-f: true" << endl;
		else
			ax.push_back(param);
		
	});
	cerr << join(ax, " ") << endl;
	return 0;
}
