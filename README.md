# :package: Introduction <br>
This repository contains a simple project to solve **Linear Progamming(LP)** problems, which is implemented just using `C++`. <br>
# :newspaper: Description <br>
The method I choose to solve LP problems is the **Two-Phase simplex algorithm**. <br>
Also, I use **Bland's rule** to choose the variables instead of the normal rule to **avoid cycling**. <br>
The solving processes will be displayed in the *dictionary form*, not the *tabular form*.
# :bulb: Installation <br>
Here is a simple way to install this program. <br>
Make sure you installed `GCC` on your computer. 
If you didn't, here is the [MinGW for Windows](https://sourceforge.net/projects/mingw/) installation file. <br>

First, you need to download all files to your computer. Look at the green `Code` box. Then, click on it and choose `Download ZIP`. <br>
After downloading, just extract it. <br>
Then, open your `Terminal` in the same place as source files and run this command:
``` shell
g++ main.cpp ExFuncs.cpp Problems.cpp Models.cpp -o LPSolver.exe
```
Finally, just run the `LPSolver` file to run the program.
# :question: How to use <br>
You will input ***n + 3*** line into the program, which n is the number of *equality(eq)/ inequality(ineq) constraints*.
- The first line: Input 1 for the minimizing problem, 2 for the maximizing problem.
- The next line: Input objective function indicies, end with *.
- The next n lines: Input eq/ ineq constraints by inputting variable indicies, signs and the free coefficients.
  + The last constraints will end with *.
- The last line: Input variable sign constraints.
  + If you want to input free variables, input it as `f`
    * Note that for any variable indicies is equal to 0, just input it as 0 instead of skipping it.
    * The number of signs constraints must be equal to the objective function indicies and the eq/ ineq constraint variable indicies. <br>

**Some examples** <br>
``` math
\begin{array}{rrrrr}
\text{maximize} & -x_1 & - & 3x_2 & - & x_3 \\
\text{subject to} & 2x_1 & - & 5x_2 & + & x_3 & \leq & -5 \\
                  & 2x_1 & - & x_2 & + & 2x_3 & \leq & 4 \\
                  & x_1, & x_2, & x_3 & \geq & 0
\end{array}
```
Input the problem as:
``` shell
2
-1 -3 -1 *
2 -5 1 <= -5
2 -1 2 <= 4 *
>= >= >=
```
``` math
\begin{array}{rrrrrr}
\text{minimize} & 5x_1 & + & 4x_2 \\
\text{subject to} & x_1 & + & x_2 & - & x_3 & & & = & 1 \\
                  & x_1 & - & x_2 &  &  & + & x_4 & = & 5 \\
                  & x_1, & x_2, & x_3 & \geq & 0
\end{array}
```
Input the problem as:
``` shell
1
5 4 0 0 *
1 1 -1 0 = 1
1 -1 0 -1 = 5 *
>= >= >= f
```
# :heavy_exclamation_mark: Notes <br>
During the calculating process, maybe you will see some dictionaries in two phases have abnormal values. <br>
It happens since the chosen pivot point has its index approximately 0, but... it's okay. <br>
Maybe there are some bugs I haven't found :D
