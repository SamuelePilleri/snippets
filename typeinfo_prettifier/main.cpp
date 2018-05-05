#include <iostream>
#include <sstream>

#include "typeinfo_pretty.h"

using namespace std;

namespace bar
{
	class quxx
	{

	};
}

int main(int argc, char const *argv[])
{
	bar::quxx a;
	ostringstream oss;
	oss << typeid(a);

	cout << "Original      : " << typeid(a).name() << endl;
	cout << "Prettified    : " << typeid(a) << endl;
	cout << "ostringstream : " << oss.str() << endl;

	oss.str("");
	oss.clear();

	return 0;
}
