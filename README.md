# Introduction <br>
This repository contains a simple project to solve Linear Progamming(LP) problems, which is implemented just using `C++`. <br>
# Description <br>
The method I use to solve LP problems is the Two-Phase simplex algorithm. <br>
Also, I use Bland's rule to choose the variables instead of the normal rule to avoid cycling. <br>
# :bulb: Installation <br>
First, you need to download the source files to your computer. <br>
Then, open CommandPrompt in the same place as source files and run this command:
``` shell =
g++ main.cpp ExFuncs.cpp Problems.cpp Models.cpp -o LPSolver.exe
```
Finally, just run the LPSolver file to run the program.
