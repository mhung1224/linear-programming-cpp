#include "Classes.h"
#include "ExFuncs.h"
#include <cmath>

void Problem::set_goal(int goal) {
    this->goal = goal;
}

int Problem::get_goal() {
    return this->goal;
}

void Problem::set_var_amount() {
    this->var_amount = this->obj_idx.size();
}

int Problem::get_var_amount() {
    return this->var_amount;
}

void Problem::add_object_index() {
    std::string temp = {};
    while (true) {
        std::cin >> temp;

        if (temp == "*") {
            break;
        }

        this->obj_idx.push_back(std::stod(temp));
    }

    //Get variables amount
    this->set_var_amount();
}

std::vector<double> Problem::get_object_index() {
    return this->obj_idx;
}

void Problem::add_constraints() {
    std::string temp = {};
    int idx = 0;
    while (true) {
        this->var_constrs.push_back({}); // Initialize var_constrs with an empty vector

        for (int i = 0; i < this->var_amount; i++) {
            std::cin >> temp;

            if (temp == "*") {
                var_constrs.pop_back();
                return;
            }

            this->var_constrs[idx].push_back(std::stod(temp));
        }

        std::cin >> temp;
        this->eq_sign_constrs.push_back(temp);

        std::cin >> temp;
        this->f_coeffs.push_back(std::stod(temp));

        ++idx;
    }
}

std::vector<std::vector<double>> Problem::get_var_constrs() {
    return this->var_constrs;
}

std::vector<std::string> Problem::get_eq_sign_constrs() {
    return this->eq_sign_constrs;
}

std::vector<double> Problem::get_f_coeffs() {
    return this->f_coeffs;
}

void Problem::add_sign_constrs() {
    std::string temp = {};

    for (int i = 0; i < this->var_amount; i++) {
        std::cin >> temp;
        this->sign_constrs.push_back(temp);
    }
}

std::vector<std::string> Problem::get_sign_constrs() {
    return this->sign_constrs;
}

void Problem::add_variables() {
    std::string temp = {};

    for (int i = 0; i < this->var_amount; i++) {
        temp = "x" + std::to_string(i + 1);
        this->vars.push_back(temp);
    }
}

std::vector<std::string> Problem::get_variables() {
    return this->vars;
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

    //Show objective function
    if (this->goal == 1) {
        std::cout << "Min   ";
    }
    else {
        std::cout << "Max   ";
    }

    // Initialize the vector to store the length of each element in the problem
    std::vector<int> max_length = {};
    for (int i = 0; i < this->obj_idx.size(); i++) {
        max_length.push_back(findLength(std::abs(this->obj_idx[i])));
    }
    max_length.push_back(findLength(std::abs(this->f_coeffs[0])));

    //Find the maximum length of each element in the problem
    for (int i = 0; i < this->var_constrs.size(); i++) {
        for (int j = 0; j < this->var_constrs[i].size(); j++) {
            if (findLength(std::abs(this->var_constrs[i][j])) > max_length[j]) {
                max_length[j] = findLength(std::abs(this->var_constrs[i][j]));
            }
        }
    }
    for(int i = 1; i < this->f_coeffs.size(); i++) {
		if(findLength(std::abs(this->f_coeffs[i])) > max_length.back()) {
			max_length.back() = findLength(std::abs(this->f_coeffs[i]));
		}
	}
    
    // Print the first object index element
    printNum(this->obj_idx[0], max_length[0] + 1, findDecimals(std::abs(this->obj_idx[0])));
    std::cout << this->vars[0];
    
    for (int i = 1; i < this->var_amount; i++) {
        if (this->obj_idx[i] < 0) {
            std::cout << " - ";
        }
        else {
            std::cout << " + ";
        }
        printNum(std::abs(this->obj_idx[i]), max_length[i], findDecimals(std::abs(this->obj_idx[i])));
        std::cout << this->vars[i];
    }
    std::cout << std::endl;

    //Show eq constraints
    for (int i = 0; i < this->var_constrs.size(); i++) {
        printChars(' ', 6);

        printNum(this->var_constrs[i][0], max_length[0] + 1, findDecimals(std::abs(this->var_constrs[i][0])));
	    std::cout << this->vars[0];

        for (int j = 1; j < this->var_constrs[i].size(); j++) {
            if (this->var_constrs[i][j] < 0) {
                std::cout << " - ";
            }
            else {
                std::cout << " + ";
            }
            printNum(std::abs(this->var_constrs[i][j]), max_length[j], findDecimals(std::abs(this->var_constrs[i][j])));
            std::cout << this->vars[j];
        }
        std::cout << " ";
        if (this->eq_sign_constrs[i] == "=") {
            std::cout << " ";
        }
        std::cout << this->eq_sign_constrs[i] << " ";
        printNum(this->f_coeffs[i], max_length.back() + 1, findDecimals(std::abs(this->f_coeffs[i])));
        std::cout << std::endl;
    }

    //Show variable constraints
    for (int i = 0; i < this->var_amount; i++) {
        if (this->sign_constrs[i] == "<=") {
            std::cout << this->vars[i] << " <= 0 | ";
        }
        else if (this->sign_constrs[i] == ">=") {
            std::cout << this->vars[i] << " >= 0 | ";
        }
    }
    std::cout << std::endl;
}

void Problem::add_Problem() {
    int goal;
    std::cout << "Input the object: 1 for Minimize, 2 for Maximize\n";
    std::cin >> goal;
    this->set_goal(goal);
    this->add_object_index();
    this->add_constraints();
    this->add_sign_constrs();
    this->add_variables();
}

Problem Problem::normalize_problem() {
    Problem Nor_Prob; //Initialize Normalized Problem
    Nor_Prob.set_goal(1);

    //Get parameters from the original problem
    Nor_Prob.obj_idx = this->get_object_index();
    Nor_Prob.set_var_amount();
    Nor_Prob.var_constrs = this->get_var_constrs();
    Nor_Prob.eq_sign_constrs = this->get_eq_sign_constrs();
    Nor_Prob.f_coeffs = this->get_f_coeffs();
    Nor_Prob.sign_constrs = this->get_sign_constrs();
    Nor_Prob.vars = this->get_variables();


    //Calculate the entire amount of the problem
    for (int i = 0; i < Nor_Prob.sign_constrs.size(); i++) {
        if (Nor_Prob.sign_constrs[i] == "f") {
            ++Nor_Prob.var_amount;
        }
    } 
 
    //Handle with free variables
    int idx = 0;
    std::string temp = {};

    while (idx < Nor_Prob.var_amount) {
        if (Nor_Prob.sign_constrs[idx] == "f") {
            temp = Nor_Prob.vars[idx];
            Nor_Prob.vars[idx] = temp + "+"; //Change x_i to Add x_i+
            Nor_Prob.vars.insert(Nor_Prob.vars.begin() + idx + 1, temp + "-"); //Insert x_i- next to x_i+
            Nor_Prob.sign_constrs[idx] = ">="; //Change x_i+ condition
            Nor_Prob.sign_constrs.insert(Nor_Prob.sign_constrs.begin() + idx + 1, ">="); //Insert x_i- condition

            Nor_Prob.obj_idx.insert(Nor_Prob.obj_idx.begin() + idx + 1, -Nor_Prob.obj_idx[idx]); //Insert x_i- object index

            for (int i = 0; i < Nor_Prob.var_constrs.size(); i++) { //Insert x_i- eq index
                Nor_Prob.var_constrs[i].insert(Nor_Prob.var_constrs[i].begin() + idx + 1, -Nor_Prob.var_constrs[i][idx]);
            }
            idx += 2;
            continue;
        }

        ++idx;
    }

    //Deal with "<=" variable constraints
    for (int i = 0; i < Nor_Prob.sign_constrs.size(); i++) {
        if (Nor_Prob.sign_constrs[i] == "<=") {
            Nor_Prob.vars[i].push_back('\''); //Change x_i to x_i', x_i' = -x_i
            Nor_Prob.sign_constrs[i] = ">="; //Change x_i condition
            Nor_Prob.obj_idx[i] = -Nor_Prob.obj_idx[i]; //Change x_i object index
            for (int j = 0; j < Nor_Prob.var_constrs.size(); j++) { //Change x_i eq index
                Nor_Prob.var_constrs[j][i] = -Nor_Prob.var_constrs[j][i];
            }
        }
    }

    //Handle with ">=": inequailty constraints
    for (int i = 0; i < Nor_Prob.eq_sign_constrs.size(); i++) {
        if (Nor_Prob.eq_sign_constrs[i] == ">=") {
            for (int j = 0; j < Nor_Prob.var_constrs[i].size(); j++) { //Reverse the eq index
                Nor_Prob.var_constrs[i][j] = -Nor_Prob.var_constrs[i][j];
            }
            Nor_Prob.eq_sign_constrs[i] = "<="; //Reverse sign
            Nor_Prob.f_coeffs[i] = -Nor_Prob.f_coeffs[i]; //Reverse freedom coefficients
        }
    }

    //Handle with "=": equaility constraints
    std::vector<double> temp1;

    for (int i = 0; i < Nor_Prob.eq_sign_constrs.size(); i++) {
        if (Nor_Prob.eq_sign_constrs[i] == "=") {
            temp1 = Nor_Prob.var_constrs[i];
            for (int j = 0; j < temp1.size(); j++) {
                temp1[j] = -temp1[j];
            }
            Nor_Prob.eq_sign_constrs[i] = "<="; //Change condition

            Nor_Prob.var_constrs.push_back(temp1); //Add reverse eq
            Nor_Prob.eq_sign_constrs.push_back("<="); //Add condition
            Nor_Prob.f_coeffs.push_back(-f_coeffs[i]); //Add free coefficients
        }
    }

    //Deal with the maximize goal
    if (this->get_goal() == 2) {
        for (int i = 0; i < Nor_Prob.obj_idx.size(); i++) {
            Nor_Prob.obj_idx[i] = -Nor_Prob.obj_idx[i];
        }
    }

    return Nor_Prob;
}