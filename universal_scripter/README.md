# Universal Scripter

This "tool" will let you run the same commands both on Windows (`cmd`) and Unix-like OSs (`bash`, `sh`) without any dependency like VMs, build tools or interpreters, just the OS default command prompt.
Simply run

```batch
C:\> build
```

or

```bash
$ bash build.cmd
```

and you're good to go!

## How it works

For a deep explaination, one may read [this answer](https://stackoverflow.com/a/17623721) on StackOverflow, but here's the quick and dirty version:

```batch
:<<"::CMDLITERAL"
@echo off
::CMDLITERAL

echo Hello, World!
```

Basically, everything between line 1 and line 3 is treated as a comment in most of POSIX*ish* shells while being executed as a command by Windows interpreter that recognizes lines 1 and 3 as comments; no magic anywhere. So the script (at the current state) just disables *echoes* on Windows and then jumps straight to line 5 where both systems will start issuing specified commands.

Of course, you can only write code that is valid on every OS you wish to run this on, so it is kind of limited. There would be a way (using goto jumps, specified in the SO answer) to run parts of code on Windows/\*nix only, but I don't suggest it as it tends to become a mess pretty quickly.

## But why?

Users might ask why such a tool when we have awesome `make`; the reason is quite simple: I had to run simple build commands on systems that do not have `make` installed enough times to justify looking for a similar solution.

Here are some use cases:

```bash
g++ -Wall -Wpedantic -Werror -Wextra -Wno-unused-parameter --std=c++17 -O2 main.cpp -o main
```

```bash
pandoc --smart --number-sections --listings --toc document.md -o document.pdf
```

```bash
latexmk -pdf document.tex
latexmk -c
```

Credits: [Single script to run in both Windows batch and Linux Bash? - Stack Overflow](https://stackoverflow.com/questions/17510688/single-script-to-run-in-both-windows-batch-and-linux-bash)
