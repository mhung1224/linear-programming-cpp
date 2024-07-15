#include "Classes.h"
#include "ExFuncs.h"
#include <cmath>

Problem::Problem() {
    this->set_goal(1);
}

void Problem::set_goal(int goal) {
    this->goal = goal;
}

int Problem::get_goal() {
    return this->goal;
}

void Problem::set_var_amount(int num) {
    this->var_amount = num;
}

int Problem::get_var_amount() {
    return this->var_amount;
}

std::vector<double> Problem::get_objective_function() {
    return this->obj_func_idx;
}

std::vector<std::vector<double>> Problem::get_var_constraints() {
    return this->var_consts;
}

std::vector<double> Problem::get_free_coefficients() {
    return this->f_coeffs;
}

std::vector<std::string> Problem::get_variables() {
    return this->vars_name;
}

void Problem::add_Problem() {
    // Set the problem's goal
    int goal;
    std::cout << "Input the problem:\n";
    std::cin >> goal;
    this->set_goal(goal); 
    // Add the objective function indices
    std::string chars = {};
    while (true) {
        std::cin >> chars;
        if (chars == "*") {
            break;
        }
        this->obj_func_idx.push_back(std::stod(chars));
    }
    // Set variables amount
    this->set_var_amount(this->obj_func_idx.size());
    // Add constraints
    int idx = 0;
    while (true) {
        bool need_break = false;
        this->var_consts.push_back({}); // Initialize var_consts with an empty vector
        for (int i = 0; i < this->get_var_amount(); i++) {
            std::cin >> chars;
            if (chars == "*") {
                var_consts.pop_back();
                need_break = true;
                break;
            }
            this->var_consts[idx].push_back(std::stod(chars));
        }
        if (need_break) {
            break;
        }
        std::cin >> chars;
        this->eq_sign_consts.push_back(chars);
        std::cin >> chars;
        this->f_coeffs.push_back(std::stod(chars));
        ++idx;
    } 
    // Add sign constraints
    for (int i = 0; i < this->get_var_amount(); i++) {
        std::cin >> chars;
        this->sign_consts.push_back(chars);
    }
    // Add variable names
    for (int i = 0; i < this->get_var_amount(); i++) {
        this->vars_name.push_back("x" + std::to_string(i + 1));
    }
}

void Problem::display(bool clear_screen) {
    // Print the inputted/normalized problem
    if(clear_screen) {
		clearScreen();
        std::cout << "Inputted Problem\n";
	}
    else {
        std::cout << "\nNormalized Problem\n";
    }
    // Show objective function
    if (this->goal == 1) {
        std::cout << "Min   ";
    }
    else {
        std::cout << "Max   ";
    }
    // Initialize the vector to store the length of the first element(by column) in the problem
    std::vector<int> max_length = {};
    for (int i = 0; i < this->obj_func_idx.size(); i++) {
        max_length.push_back(findLength(std::abs(this->obj_func_idx[i])));
    }
    max_length.push_back(findLength(std::abs(this->f_coeffs[0])));
    //Find the maximum length of each element in the problem(by column)
    for (int i = 0; i < this->var_consts.size(); i++) {
        for (int j = 0; j < this->var_consts[i].size(); j++) {
            if (findLength(std::abs(this->var_consts[i][j])) > max_length[j]) {
                max_length[j] = findLength(std::abs(this->var_consts[i][j]));
            }
        }
    }
    for(int i = 1; i < this->f_coeffs.size(); i++) {
		if(findLength(std::abs(this->f_coeffs[i])) > max_length.back()) {
			max_length.back() = findLength(std::abs(this->f_coeffs[i]));
		}
	}
    // Print the first object index
    printNumber(this->obj_func_idx[0], max_length[0] + 1, findDecimals(std::abs(this->obj_func_idx[0])));
    std::cout << this->vars_name[0];
    // Print leftover object indicies
    for (int i = 1; i < this->var_amount; i++) {
        if (this->obj_func_idx[i] < 0) {
            std::cout << " - ";
        }
        else {
            std::cout << " + ";
        }
        printNumber(std::abs(this->obj_func_idx[i]), max_length[i], findDecimals(std::abs(this->obj_func_idx[i])));
        std::cout << this->vars_name[i];
    }
    std::cout << std::endl;
    // Show eq constraints
    for (int i = 0; i < this->var_consts.size(); i++) {
        printChars(' ', 6);
        printNumber(this->var_consts[i][0], max_length[0] + 1, findDecimals(std::abs(this->var_consts[i][0])));
	    std::cout << this->vars_name[0];
        for (int j = 1; j < this->var_consts[i].size(); j++) {
            if (this->var_consts[i][j] < 0) {
                std::cout << " - ";
            }
            else {
                std::cout << " + ";
            }
            printNumber(std::abs(this->var_consts[i][j]), max_length[j], findDecimals(std::abs(this->var_consts[i][j])));
            std::cout << this->vars_name[j];
        }
        std::cout << " ";
        if (this->eq_sign_consts[i] == "=") {
            std::cout << " ";
        }
        std::cout << this->eq_sign_consts[i] << " ";
        printNumber(this->f_coeffs[i], max_length.back() + 1, findDecimals(std::abs(this->f_coeffs[i])));
        std::cout << std::endl;
    }
    // Show variable constraints
    for (int i = 0; i < this->var_amount; i++) {
        if (this->sign_consts[i] == "<=") {
            std::cout << this->vars_name[i] << " <= 0 | ";
        }
        else if (this->sign_consts[i] == ">=") {
            std::cout << this->vars_name[i] << " >= 0 | ";
        }
    }
    std::cout << std::endl;
}

Problem Problem::normalize_problem() {
    Problem Nor_Prob = *this;
    // Calculate the entire amount of the free variables in the problem
    for (int i = 0; i < Nor_Prob.sign_consts.size(); i++) {
        if (Nor_Prob.sign_consts[i] == "f") {
            ++Nor_Prob.var_amount;
        }
    } 
    // Handle with free variables
    int idx = 0;
    std::string var_name = {};
    while (idx < Nor_Prob.var_amount) {
        if (Nor_Prob.sign_consts[idx] == "f") {
            var_name = Nor_Prob.vars_name[idx];
            Nor_Prob.vars_name[idx] = var_name + "+"; //Change x_i to Add x_i+
            Nor_Prob.vars_name.insert(Nor_Prob.vars_name.begin() + idx + 1, var_name + "-"); //Insert x_i- next to x_i+
            Nor_Prob.sign_consts[idx] = ">="; //Change x_i+ condition
            Nor_Prob.sign_consts.insert(Nor_Prob.sign_consts.begin() + idx + 1, ">="); //Insert x_i- condition
            Nor_Prob.obj_func_idx.insert(Nor_Prob.obj_func_idx.begin() + idx + 1, -Nor_Prob.obj_func_idx[idx]); //Insert x_i- object index
            for (int i = 0; i < Nor_Prob.var_consts.size(); i++) { //Insert x_i- eq index
                Nor_Prob.var_consts[i].insert(Nor_Prob.var_consts[i].begin() + idx + 1, -Nor_Prob.var_consts[i][idx]);
            }
            idx += 2;
            continue;
        }
        ++idx;
    }
    // Handle with "<=" variable constraints
    for (int i = 0; i < Nor_Prob.sign_consts.size(); i++) {
        if (Nor_Prob.sign_consts[i] == "<=") {
            Nor_Prob.vars_name[i].push_back('\''); //Change x_i to x_i', x_i' = -x_i
            Nor_Prob.sign_consts[i] = ">="; //Change x_i condition
            Nor_Prob.obj_func_idx[i] *= -1; //Change x_i object index
            for (int j = 0; j < Nor_Prob.var_consts.size(); j++) { //Change x_i eq index
                Nor_Prob.var_consts[j][i] *= -1;
            }
        }
    }
    // Handle with ">=": inequailty constraints
    for (int i = 0; i < Nor_Prob.eq_sign_consts.size(); i++) {
        if (Nor_Prob.eq_sign_consts[i] == ">=") {
            for (int j = 0; j < Nor_Prob.var_consts[i].size(); j++) { //Reverse the eq index
                Nor_Prob.var_consts[i][j] *= -1;
            }
            Nor_Prob.eq_sign_consts[i] = "<="; //Reverse sign
            Nor_Prob.f_coeffs[i] *= -1; //Reverse freedom coefficients
        }
    }
    // Handle with "=": equaility constraints
    std::vector<double> new_eq_const;
    for (int i = 0; i < Nor_Prob.eq_sign_consts.size(); i++) {
        if (Nor_Prob.eq_sign_consts[i] == "=") {
            new_eq_const = Nor_Prob.var_consts[i];
            for (int j = 0; j < new_eq_const.size(); j++) {
                new_eq_const[j] *= -1;
            }
            Nor_Prob.eq_sign_consts[i] = "<="; //Change condition
            Nor_Prob.var_consts.push_back(new_eq_const); //Add reverse eq
            Nor_Prob.eq_sign_consts.push_back("<="); //Add condition
            Nor_Prob.f_coeffs.push_back(-f_coeffs[i]); //Add free coefficients
        }
    }
    // Handle with the maximize goal
    if (this->get_goal() == 2) {
        for (int i = 0; i < Nor_Prob.obj_func_idx.size(); i++) {
            Nor_Prob.obj_func_idx[i] *= -1;
        }
    }
    return Nor_Prob;
}