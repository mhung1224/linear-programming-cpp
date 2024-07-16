# :package: Introduction <br>
This repository contains a simple project to solve **Linear Progamming(LP)** problems, which is implemented just using `C++`. <br>
# :newspaper: Description <br>
The method I choose to solve LP problems is the **Two-Phase simplex algorithm**. <br>
Also, I use **Bland's rule** to choose the variables instead of the normal rule to **avoid cycling**. <br>
The solving processes will be displayed in the *dictionary form*, not the *tabular form*.
# :bulb: Installation <br>
Here is a simple way to install this program. <br>
Make sure you installed `GCC` on your computer. 
If you did't, here is the [MinGW for Windows](https://sourceforge.net/projects/mingw/) installation file. <br>

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

Some examples: <br>
| Công thức toán học         | Dòng lệnh                      |
|-----------------------------|--------------------------------|
| Định lý Pythagoras          | `ls -l`              |
| Đạo hàm                    | `diff(f(x), x)`                |
| Tích phân                  | `integrate(f(x), x)`           |

# :heavy_exclamation_mark: Notes <br>
During the calculating process, maybe sometime you can see some phase has abnormal values, but it's okay. <br>
Maybe there are some bugs I haven't found :D
