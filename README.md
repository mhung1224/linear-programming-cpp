# :package: Introduction <br>
This repository contains a simple project to solve **Linear Progamming(LP)** problems, which is implemented just using `C++`. <br>
# :newspaper: Description <br>
The method I choose to solve LP problems is the **Two-Phase simplex algorithm**. <br>
Also, I use **Bland's rule** to choose the variables instead of the normal rule to **avoid cycling**. <br>
The solving processes will be displayed in the *dictionary form*, not the *tabular form*.
# :bulb: Installation <br>
Make sure you installed `GCC` on your computer.
If you don't, here is the [MinGW](https://sourceforge.net/projects/mingw/) installation file to install it. <br>

First, you need to download all files to your computer. <br>
Then, open your *Terminal* in the same place as source files and run this command:
``` shell
g++ main.cpp ExFuncs.cpp Problems.cpp Models.cpp -o LPSolver.exe
```
Finally, just run the *LPSolver* file to run the program.
## :question: How to input the problem into the program ? <br>
Suppose you have a LP problem like this: <br>
$$Minimize/Maximize$$ $$\sum_{i = 1}^{n} c_i x_i$$

\[
\begin{aligned}
\text{subject to} \\
\sum a_i x_i & \leq b_i \quad \text{for all } i \in M_1, \\
\sum a_i x_i & \geq b_i \quad \text{for all } i \in M_2, \\
\sum a_i x_i & = b_i \quad \text{for all } i \in M_3, \\
x_i & \geq 0 \quad \text{for all } i \in N_1, \\
x_i & \leq 0 \quad \text{for all } i \in N_2, \\
x_i & \text{ is free for all } i \in N_3.
\end{aligned}
\]

and $M_1 \cup M_2 \cup M_3 = N_1 \cup N_2 \cup N_3 = \{1, \ldots, n\}$. <br>
$$
# :heavy_exclamation_mark: Notes <br>
During the calculating process, maybe sometime you can see some phase has abnormal values, but it's okay.
