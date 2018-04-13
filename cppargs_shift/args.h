#ifndef ARGS_SHIFT_H
#define ARGS_SHIFT_H

#include <functional>

#if __cplusplus >= 201402L

constexpr unsigned int chash(char const *str)
{
	unsigned int hash = 5381;
	while(*str)
		hash = ((hash << 5) + hash) + static_cast<unsigned int>(*str++);
	return hash;
}

#else

static constexpr unsigned int __trdjb(char const *str, const unsigned int hash)
{
	return *str ?
		__trdjb(str + 1, ((hash << 5) + hash) + static_cast<unsigned int>(*str))
		: hash;
}

constexpr unsigned int chash(char const *str)
{
	return __trdjb(str, 5381);
}

#endif

void parse(const int argc, char const *argv[], std::function<void( char const *param, const std::function<std::string()>& next )>&& callback )
{
	int i = 1;
	
	auto n = [&i, &argc, &argv]() -> std::string {
		return (i < argc) ? argv[i++] : "";
	};
	
	while(i < argc)
	{
		callback(argv[i++], n);
	}
}

void parse(const int argc, char const *argv[], std::function<void( char const *param, const std::function<std::string()>& next, const std::function<std::string()>& peek )>&& callback )
{
	int i = 1;
	
	auto n = [&]() -> std::string {
		return (i < argc) ? argv[i++] : "";
	};

	auto p = [&]() -> std::string {
		return (i < argc) ? argv[i] : "";
	};
	
	while(i < argc)
	{
		callback(argv[i++], n, p);
	}
}

#endif /* ARGS_SHIFT_H */
