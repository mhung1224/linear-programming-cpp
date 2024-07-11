#ifndef CLASSES_H
#define CLASSES_H
 
#include <iostream>
#include <string>
#include <vector>

// A class to store the problem
class Problem { 
private:
    // Goal = 1: Minimize, Goal = 2: Maximize
    int goal; 
    // Number of variables
    int var_amount; 
    // Variable names
    std::vector<std::string> vars; 
    // Objective function index
    std::vector<double> obj_idx; 
    // Variable constraints
    std::vector<std::vector<double>> var_constrs; 
    // Equality/inequality's sign constraints
    std::vector<std::string> eq_sign_constrs;
    // Free coefficients
    std::vector<double> f_coeffs; 
    // Variable's sign constraints
    std::vector<std::string> sign_constrs; 

public:
    // Constructor
    Problem() {}; 
    // Set the goal
    void set_goal(int goal);
    // Get the goal
    int get_goal();
    // Set the variable amount
    void set_var_amount();
    // Get the variable amount
    int get_var_amount();
    // Add the objective function index
    void add_object_index();
    // Get the objective function index
    std::vector<double> get_object_index();
    // Add the constraints
    void add_constraints();
    // Get the constraints
    std::vector<std::vector<double>> get_var_constrs();
    // Add the equality/inequality's sign constraints
    std::vector<std::string> get_eq_sign_constrs();
    // Get the free coefficients
    std::vector<double> get_f_coeffs();
    // Add the variable's sign constraints
    void add_sign_constrs();
    // Get the variable's sign constraints
    std::vector<std::string> get_sign_constrs();
    // Add the variables
    void add_variables();
    // Get the variables
    std::vector<std::string> get_variables();
    // Add the problem
    void add_Problem();
    // Display the problem. 0 for not clearing the screen, 1 for clearing the screen
    void display(bool clear_screen); 
    // Normalize the problem
    Problem normalize_problem();
};

// A dictionary class to store the problem
class Dict { 
protected:
    // Basic variables
    std::vector<std::string> dep_var; 
    // Non-basic variables
    std::vector<std::string> indep_var;
    // Objective function values
    std::vector<double> indep_var_obj; 
    // Equality constraints matrix
    std::vector<std::vector<double>> eq_constrs_matrix; 
public:
    Dict() {};
    void set_dep_var(std::vector<std::string> dep_var);
    std::vector<std::string> get_dep_var();
    void set_indep_var(std::vector<std::string> indep_var);
    std::vector<std::string> get_indep_var();
    void set_indep_var_obj(std::vector<double> indep_var_obj);
    std::vector<double> get_indep_var_object();
    void set_eq_constrs_matrix(std::vector<std::vector<double>> eq_constrs_matrix);
    std::vector<std::vector<double>> get_eq_constrs_matrix();
    std::vector<int> find_pivot(); //Find the pivot
    void pivot_dict(int row, int col); //Pivot the dictionary
    void display(); //Display the dictionary
};

class Phase_1: public Dict {
private:
    // 1:Feasible, 2:Infeasible
    int status; 
public:
    Phase_1() {};
    Phase_1(Problem nor_Prob);
    void set_status(int status);
    int get_status();
    void solve();
};

class Phase_2: public Dict {
private:
    // 1:Optimal, 2:Feasible, 3:Unbounded
    int status; 
public:
    Phase_2() {};
    Phase_2(Phase_1 result);
    void set_status(int status);
    int get_status();
    void solve();
    // Handle the dictionary from Phase 1
    void handle_vars(Problem nor_Prob); 
};

// Two-Phase Simplex Method
class S2P_Method {
private:
    Phase_1 Phase1;
    Phase_2 Phase2;
    Problem Prob;
    // Optimal variables
    std::vector<std::string> opt_vars_list;
    // Optimal variable's values
    std::vector<double> opt_vars_val;
    // Optimal value
    double opt_val;
    // 1: Optimal, 2: Infeasible, 3: Feasible, 4: Unbounded
    int status; 
public:
    S2P_Method(Problem Prob);
    // Solve the problem
    void solve();
    // Set the status of the problem
    void set_status(int status);
    // Get the status of the problem
    int get_status();
    // Get the optimal results
    void get_opt_vals();
    // Display the results
    void display();
};

#endif