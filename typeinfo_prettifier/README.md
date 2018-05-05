Prettify C++ `std::type_info::name` string to get human-readable type info at runtime.

```cpp
bar::quxx a;
cout << "Original   : " << typeid(a).name() << endl;
cout << "Prettified : " << typeid(a) << endl;
```

Output:

```
$ g++ main.cpp -Wall -pedantic -O3 && ./a.out 
Original   : N3bar4quxxE
Prettified : bar::quxx
```

This result is achieved by overloading `std::ostream::operator<<` to enable name demangling where supported. It is a bad practice, however, to rely on a type name at runtime and thus should only be used for debugging or logging purposes (hence the `ostream` operator). If you need to store the result in a string just do:

```cpp
ostringstream oss;
oss << typeid(a);
string t = oss.str();
```

Inspired by Eli Bendersky's [article](https://eli.thegreenplace.net/2015/programmatic-access-to-the-call-stack-in-c/) and this [question](https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname) on StackOverflow.
