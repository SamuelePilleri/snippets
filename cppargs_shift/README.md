A pure functional command line argument parser written in modern C++ and inspired by Bash.

Requires C++11 for lambda and `constexpr` support, works better with C++14.

```cpp
#include <iostream>
#include "args.h"

#define IS_INTEGER(s) (s.find_first_not_of("0123456789") == std::string::npos)

using namespace std;

int main(int argc, char const *argv[])
{
    parse(argc, argv, [&](char const *param, auto& next, auto& peek){
        switch(chash(param))
        {
            case chash("-v"):
            case chash("--version"):
                cout << "Version 1.0" << endl;
                break;

            case chash("-n"):
            case chash("--number"):
                if( IS_INTEGER( peek() ))
                    cout << "Valid number " << next() << endl;
                else
                    cout << "Invalid number " << next() << endl;
                break;

            default:
                cout << "Invalid param " << param << endl;
                break;
        }
    });
    return 0;
}
```

# Considerations

In Bash the standard approach to argument parsing is the following:

```bash
NUMBER=""
UNPARSED=""

while test $# -gt 0 # Iterate over argv
do
    case "$1" in
        -v|--version)
            echo "Version 1.0"
        ;;

        -n|--number)
            NUMBER="$2"
            shift # Consume argument
        ;;

        *)
            UNPARSED="$UNPARSED $1"
        ;;
    esac
    shift # Consume argument
done

```

It has the advantages of being easy to read, straightforward to write and simple to maintain. This library tries to replicate such conditions in an efficient way while preserving readability and compactness.

## Compile-time string hashing

In order to be able to use a `switch - case` statement (which is way [more efficient](https://stackoverflow.com/a/98024) than an `if - else if` ladder) we need to deal only with values that are known constants at compile-time. As suggested by [this answer](https://stackoverflow.com/a/2112111) on StackOverflow, C++11 has the ability of evaluating functions at compile-time when they are marked `constexpr`. Hence, this library implements a compile-time string hashing function which can be used in a _switch-case_ statement to achieve the desired outcome:

```cpp
switch(chash(param))
{
    case chash("foo"):
        // ...
        break;

    case chash("bar"):
        // ...
        break;

    default:
        break;
}
```

In order to keep the code simple, the chosen method is the [DJB2 hashing function](http://www.cse.yorku.ca/~oz/hash.html) described by Daniel Bernstein in 1991 which can be implemented with a few lines of code. It is important to note that, in this scenario, there is no need to care about collisions since, if any, they are reported as an error by the compiler.

Due to the limits of C++11, the `chash()` function has to be recursive, which can be problematic when the input size grows, even if the function is [tail recursive](https://stackoverflow.com/q/9258525). C++14 introduces [extended constexpr](https://isocpp.org/wiki/faq/cpp14-language#extended-constexpr) and this library provides an implementation that will make use of the new feature to workaround the above-mentioned problem; the compiler will choose a suitable implementation based on the standard selected.