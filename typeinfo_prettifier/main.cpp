#include <iostream>

using namespace std;

#if defined(__has_include) && __has_include(<cxxabi.h>)

#  include <cstdlib>
#  include <cstring>
#  include <cxxabi.h>

/**
 *  Prettify a type_info.name() string, based on Eli Bendersky's great article:
 *  http://eli.thegreenplace.net/2015/programmatic-access-to-the-call-stack-in-c/
 */
char* prettify_name(const char* name)
{
	// This makes the function not thread safe, but I don't care as of now.
	// In a multithreaded environment, just return `demangled`, but remember to
	// free it after use!
	static char ret[256];

	int status;
	char* demangled = abi::__cxa_demangle(name, NULL, NULL, &status);
	if(status == 0)
		std::strncpy(ret, demangled, sizeof(ret));
	else
		std::strncpy(ret, name, sizeof(ret));
	std::free(demangled);

	return ret;
}

#else
#  define prettify_name(x) x
#  pragma message "Warning: No type_info::name prettify support"
#endif

/*
 *  The header <typeinfo> must be included before using typeid (if the header is
 *  not included, every use of the keyword typeid makes the program ill-formed.)
 *  (http://en.cppreference.com/w/cpp/language/typeid) 
 */
#include <typeinfo>

namespace bar
{
	class quxx
	{

	};
}

int main(int argc, const char* argv[])
{
	bar::quxx a;
	cout << "Original  : " << typeid(a).name() << endl;
	cout << "Prettified: " << prettify_name(typeid(a).name()) << endl;
	return 0;
}
