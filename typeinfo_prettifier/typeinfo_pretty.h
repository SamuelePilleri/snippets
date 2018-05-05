#ifndef TYPEINFO_PRETTY_H
#define TYPEINFO_PRETTY_H

#include <typeinfo>

// Quick __cxa_demangle check, boost::core::demangle has a better one
#if (defined(__has_include) && __has_include(<cxxabi.h>)) || defined(__GNUG__)

#include <ostream>
#include <cstdlib>
#include <cxxabi.h>

std::ostream& operator<<(std::ostream& lhs, const std::type_info& rhs)
{
	int status = -4;
	const char* name = rhs.name();
	char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
	
	if(status == 0)
		lhs << res;
	else
		lhs << name;
	
	std::free(res);
	
	return lhs;
}

#else /* doesn't have __cxa_demangle */

inline std::ostream& operator<<(std::ostream& lhs, const std::type_info& rhs)
{
	lhs << rhs.name();
	return lhs;
}

#endif /* has __cxa_demangle */

#endif /* TYPEINFO_PRETTY_H */
