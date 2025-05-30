# 1. Description <br>
This repository contains a simple program implemented in `C++` for solving **Linear Progamming(LP)** problems. <br>
The program uses the **Two-Phase Simplex algorithm** to find optimal solutions. <br>
To prevent cycling during pivot operations, it incorporates **Bland's Rule** for variable selection. <br>
The solving process is presented in dictionary form, rather than the tabular form, for better clarity and traceability. <br>

# 2. How to use <br>
You must provide **n + 3** lines of input to the program, where **n** is the number of equality (eq) and inequality (ineq) constraints.
- Line 1 - Problem Type: Enter `1` for a minimization problem, or `2` for a maximization problem.
- Line 2 - Objective function: Enter the indices of the objective function indices, ending with a `*`.
- Line 3 to n + 2 - Constraints: Each line represents an equality or inequality constraint. Input eq/ ineq constraints by inputting variable indicies, signs and the free coefficients. End the last constraint line with a `*`.
- Line n + 3 - Variable sign constraints: Input the sign constraints for each variable:
  + For free variables, enter `f`
    * Note that for any variable indicies is equal to 0, enter 0 - do not skip it.
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
Input the problem using the following format:
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
Input the problem using the following format:
``` shell
1
5 4 0 0 *
1 1 -1 0 = 1
1 -1 0 -1 = 5 *
>= >= >= f
```

# 3. Notes <br>
During the computation process, some dictionaries in either phase may contain abnormal or unstable values. <br>
This can occur when the selected pivot element is approximately zero, leading to rounding or numerical inaccuracies. <br>
These anomalies are expected and do not affect the correctness of the final result. <br>
