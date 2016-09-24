Prettify C++ `type_info::name` string to get human-readable type info at runtime.

## Example
```cpp
bar::quxx a;
cout << "Original  : " << typeid(a).name() << endl;
cout << "Prettified: " << prettify(typeid(a).name()) << endl;
```
Output:
```
$ g++ main.cpp -Wall -pedantic -O3 && ./a.out 
Original  : N3bar4quxxE
Prettified: bar::quxx
```
