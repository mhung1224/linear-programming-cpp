#ifndef CLASSES_H
#define CLASSES_H
 
#include <iostream>
#include <string>
#include <vector>

const double eps = 1e-5;

// A class to store the problem
class Problem { 
    private:
        // Goal = 1: Minimize, Goal = 2: Maximize
        int goal; 
        // Number of variables
        int var_amount; 
        // Variable names
        std::vector<std::string> vars_name; 
        // Objective function index
        std::vector<double> obj_func_idx; 
        // Variable constraints
        std::vector<std::vector<double>> var_consts; 
        // Sign constraints of equality/ inequality
        std::vector<std::string> eq_sign_consts;
        // Free coefficients
        std::vector<double> f_coeffs; 
        // Sign constraints of variables
        std::vector<std::string> sign_consts; 
    public:
        // Constructor
        Problem(); 
        // Set problem's goal
        void set_goal(int goal);
        // Get problem's goal
        int get_goal();
        // Set the amount of variables in the problem
        void set_var_amount(int num);
        // Get the amount of variables in the problem
        int get_var_amount();
        // Get the objective function index
        std::vector<double> get_objective_function();
        // Get variable constraints 
        std::vector<std::vector<double>> get_var_constraints();
        // Get free coefficients
        std::vector<double> get_free_coefficients();
        // Get the variables
        std::vector<std::string> get_variables();
        // Add the problem by inputting
        void add_Problem();
        // Display the problem. 0 for not clearing the screen, 1 for clearing the screen
        void display(bool clear_screen); 
        // Normalize the problem
        Problem normalize_problem();
};

// A class to store the problem in dictionary form
class Dict { 
    protected:
        // Basic variables
        std::vector<std::string> b_vars;    
        // Non-basic variables
        std::vector<std::string> nb_vars;
        // Objective function values
        std::vector<double> nb_var_obj; 
        // Equality constraints matrix
        std::vector<std::vector<double>> eq_consts_matrix; 
    public:
        Dict() {};
        // Set basic variales in the dictionary
        void set_basic_vars(std::vector<std::string> b_vars);
        // Get basic variales in the dictionary
        std::vector<std::string> get_basic_vars();
        // Set non-basic variales in the dictionary
        void set_non_basic_vars(std::vector<std::string> nb_vars);
        // Get non-basic variales in the dictionary
        std::vector<std::string> get_non_basic_vars();
        // Set objective function index in the dictionary
        void set_objective_function(std::vector<double> nb_var_obj);
        // Get objective function index the dictionary
        std::vector<double> get_objective_function();
        // Set the equality constraint matrix in the dictionary
        void set_eq_consts_matrix(std::vector<std::vector<double>> eq_consts_matrix);
        // Get the equality constraint matrix in the dictionary
        std::vector<std::vector<double>> get_eq_consts_matrix();
        // Find the pivot position, using Bland's rule
        std::vector<int> find_pivot(); 
        // Pivot the dictionary
        void pivot_dict(int row, int col); 
        // Display the dictionary
        void display(); 
};

class Phase_1: public Dict {
    private:
        // 1:Feasible, 2:Infeasible
        int status; 
    public:
        Phase_1();
        // Add problem to solve
        void add_Problem(Problem nor_Prob);
        // Set the status of the solving process
        void set_status(int status);
        // Get the status of the solving process
        int get_status();
        // Solve the problem in Phase 1
        void solve();
};

class Phase_2: public Dict {
    private:
        // 1:Optimal, 2:Feasible, 3:Unbounded
        int status; 
    public:
        Phase_2();
        // Get Phase_1 optmial dictionary
        void get_Phase1(Phase_1 P1);
        // Set the status of the solving process
        void set_status(int status);
        // Get the status of the solving process
        int get_status();
        // Handle the dictionary from Phase 1 and get new objective function index
        void handle_vars(Problem nor_Prob);
        // Solve the problem in Phase 2
        void solve();
};

// Two-Phase Simplex Method
class S2P_Method {
    private:
        // The goal of the initial problem
        int problem_goal;
        // Optimal variables
        std::vector<std::string> opt_vars_list;
        // Optimal variable's values
        std::vector<double> opt_vars_val;
        // Optimal value
        double opt_val;
        // 1: Optimal, 2: Infeasible, 3: Feasible, 4: Unbounded
        int status; 
    public:
        S2P_Method();
        // Set the goal of the initial problem
        void set_problem_goal(int goal);
        // Get the goal of the initial problem
        int get_problem_goal();
        // Set the status of the problem
        void set_status(int status);
        // Get the status of the problem
        int get_status();
        // Get the optimal results
        void get_optimal_solution(Phase_2 P2);
        // Solve the problem
        void solve(Problem Prob);
        // Display the results
        void display();
};

#endif