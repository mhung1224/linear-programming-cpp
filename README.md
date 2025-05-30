# 1. Description <br>
This repository contains a simple program implemented using `C++` to solve **Linear Progamming(LP)** problems. <br>
The chosen method to solve LP problems is the **Two-Phase simplex algorithm**, combined with **Bland's rule** for variable selections to **avoid cycling**. <br>
The solving process will be displayed in *dictionary form*, not in *tabular form*.

# 2. How to use <br>
You will input **n + 3** line into the program, where **n** is the number of the equality(eq) and inequality(ineq) constraints.
- The first line: Enter 1 for a minimization problem, or 2 for a maximization problem.
- The next line: Enter the indices of the objective function indices, ending with a "*".
- The next n lines: Input eq/ ineq constraints by inputting variable indicies, signs and the free coefficients.
  + The last constraints will end with *.
- The last line: Input variable sign constraints.
  + If you want to input free variables, input it as `f`
    * Note that for any variable indicies is equal to 0, input it as 0 instead of skipping it.
    * The number of sign constraints must be equal to the objective function indicies and the eq/ ineq constraint variable indicies. <br>

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

# 3. Notes <br>
During the calculating process, maybe some dictionaries in two phases will have abnormal values since the chosen pivot point 
has its value approximately 0, but it's fine. <br>
