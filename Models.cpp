#include "Classes.h"
#include "ExFuncs.h"
#include <algorithm>
#include <cmath>

void Dict::set_basic_vars(std::vector<std::string> b_vars) {
	this->b_vars = b_vars;
}

std::vector<std::string> Dict::get_basic_vars() {
    return this->b_vars;
}

void Dict::set_non_basic_vars(std::vector<std::string> nb_vars) {
	this->nb_vars = nb_vars;
}

std::vector<std::string> Dict::get_non_basic_vars() {
    return this->nb_vars;
}

void Dict::set_objective_function(std::vector<double> nb_var_obj) {
	this->nb_var_obj = nb_var_obj;
}

std::vector<double> Dict::get_objective_function() {
    return this->nb_var_obj;
}

void Dict::set_eq_consts_matrix(std::vector<std::vector<double>> eq_consts_matrix) {
	this->eq_consts_matrix = eq_consts_matrix;
}

std::vector<std::vector<double>> Dict::get_eq_consts_matrix() {
    return this->eq_consts_matrix;
}

std::vector<int> Dict::find_pivot() {
    int col = -1;
    std::string var = {};
    for (int i = 1; i < this->nb_var_obj.size(); i++) {
        if (this->nb_var_obj[i] < 0) {
            col = i;
            var = this->nb_vars[i];
            break;
        }
    } // Get first negative value of the objective function, and its index is the column to pivot
    for (int i = col + 1; i < this->nb_var_obj.size(); i++) {
        if (this->nb_var_obj[i] < 0) {
            if (this->nb_vars[i][0] != var[0]) {
                if (this->nb_vars[i] > var) {
                    col = i;
                    var = this->nb_vars[i];
                }
            }
            if (this->nb_vars[i][0] == var[0]) {
                if (this->nb_vars[i] < var) {
                    col = i;
                    var = this->nb_vars[i];
                }
            }
        }
    } // Get the actual column to pivot
    double min = 1e7;
    int row = -1;
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        if (this->eq_consts_matrix[i][col] >= 0) {
            continue;
        }
        min = this->eq_consts_matrix[i][0] / -this->eq_consts_matrix[i][col];
        row = i;
        var = this->b_vars[i + 1];
        break;
    } // Get the first positive value of b_i / -a_ij, and its index is the row to pivot
    for (int i = row + 1; i < this->eq_consts_matrix.size(); i++) {
        if (this->eq_consts_matrix[i][col] >= 0) {
            continue;
        }
        if ((this->eq_consts_matrix[i][0] / -this->eq_consts_matrix[i][col]) - min < -eps) {
            row = i;
            min = this->eq_consts_matrix[i][0] / -this->eq_consts_matrix[i][col];
            var = this->b_vars[i + 1];
        }
    } // Get the actual min b_i / -a_ij value
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        if (this->eq_consts_matrix[i][col] >= 0) {
            continue;
        }
        if (std::abs((this->eq_consts_matrix[i][0] / -this->eq_consts_matrix[i][col]) - min) < eps) {
            if (this->b_vars[i + 1][0] != var[0]) {
                if (this->b_vars[i + 1] > var) {
                    row = i;
                    var = this->b_vars[i + 1];
                }
            }
            if (this->b_vars[i + 1][0] == var[0]) {
                if (this->b_vars[i + 1] < var) {
                    row = i;
                    var = this->b_vars[i + 1];
                }
            }
        }
    } // Get the actual row to pivot
    std::vector<int> pos = {row, col};
    return pos;
}

void Dict::pivot_dict(int row, int col) {
    std::swap(b_vars[row + 1], nb_vars[col]); // Swap the entering and leaving variables
    double value = this->eq_consts_matrix[row][col]; // Get the pivot value
    this->eq_consts_matrix[row][col] = -1; // Set the pivot value in matrix to -1
    for (int i = 0; i < this->eq_consts_matrix[row].size(); i++) {
        this->eq_consts_matrix[row][i] /= -value;
    } // Get a new vector for the row
    value = this->nb_var_obj[col];
    this->nb_var_obj[col] = 0;
    for (int i = 0; i < this->nb_var_obj.size(); i++) {
        this->nb_var_obj[i] += value * this->eq_consts_matrix[row][i];
    } // Handle with the objective function
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        if (i == row) {
            continue;
        }
        value = this->eq_consts_matrix[i][col];
        this->eq_consts_matrix[i][col] = 0;
        for (int j = 0; j < this->eq_consts_matrix[i].size(); j++) {
            this->eq_consts_matrix[i][j] += value * this->eq_consts_matrix[row][j];
        }
    } // Handle with the rest of the constraints
}

void Dict::display() {
    // Find the longest variable name and its length
    std::string max_var = this->b_vars[0];
    for (int i = 1; i < this->b_vars.size(); i++) {
		if (this->b_vars[i].length() > max_var.length()) {
			max_var = this->b_vars[i];
		}
	}
    int max_len = max_var.length();
    // Find the longest number(by column) and its length
    std::vector<int> max_length = {};
    for (int i = 0; i < this->nb_var_obj.size(); i++) {
        max_length.push_back(findLength(std::abs(this->nb_var_obj[i])));
    }
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        for (int j = 0; j < this->eq_consts_matrix[i].size(); j++) {
            if (findLength(std::abs(this->eq_consts_matrix[i][j])) > max_length[j]) {
                max_length[j] = findLength(std::abs(this->eq_consts_matrix[i][j]));
            }
        }
    }
    // Display the objective function
    std::cout << this->b_vars[0];
    printChars(' ', max_len - this->b_vars[0].length());
    std::cout << " = ";
    printNumber(this->nb_var_obj[0], max_length[0] + 1, findDecimals(std::abs(this->nb_var_obj[0])));
    std::cout << this->nb_vars[0];
    for (int i = 1; i < this->nb_vars.size(); i++) {
        if (this->nb_var_obj[i] < 0) {
            std::cout << " - ";
        }
        else {
            std::cout << " + ";
        }
        printNumber(std::abs(this->nb_var_obj[i]), max_length[i], findDecimals(std::abs(this->nb_var_obj[i])));
        std::cout << this->nb_vars[i];
    }
    // Print the '-' diving line
    int hyphenNum = max_len + 3 + max_length[0] + 1 + 3 * (this->nb_vars.size() - 1);
    for (int i = 1; i < this->nb_vars.size(); i++) {
        hyphenNum += max_length[i] + this->nb_vars[i].length();
    }
    std::cout << std::endl;
    printChars('-', hyphenNum);
    std::cout << std::endl;
    // Display the equality constraints
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        std::cout << this->b_vars[i + 1];
        printChars(' ', max_len - this->b_vars[i + 1].length());
        std::cout << " = ";
        printNumber(this->eq_consts_matrix[i][0], max_length[0] + 1, findDecimals(std::abs(this->eq_consts_matrix[i][0])));
        std::cout << this->nb_vars[0];
        for (int j = 1; j < this->nb_vars.size(); j++) {
            if (this->eq_consts_matrix[i][j] < 0) {
				std::cout << " - ";
			}
			else {
				std::cout << " + ";
			}
            printNumber(std::abs(this->eq_consts_matrix[i][j]), max_length[j], findDecimals(std::abs(this->eq_consts_matrix[i][j])));
            std::cout << this->nb_vars[j];
        }
        std::cout << std::endl;
    }
}

Phase_1::Phase_1() {
	this->set_status(1);
}

void Phase_1::add_Problem(Problem nor_Prob) {
    // Initialize the non-basic variables
    this->nb_vars.push_back({});
    std::vector<std::string> vars = nor_Prob.get_variables();
    for (int i = 0; i < vars.size(); i++) {
        this->nb_vars.push_back(vars[i]);
    }
    this->nb_vars.push_back("x0");
    // Add the non-basic variables coefficients
    for (int i = 0; i < this->nb_vars.size() - 1; i++) {
        this->nb_var_obj.push_back(0);
    }
    this->nb_var_obj.push_back(1);
    // Initialize the basic variables
    this->b_vars.push_back("*");
    for (int i = 0; i < nor_Prob.get_var_constraints().size(); i++) {
        this->b_vars.push_back("w" + std::to_string(i + 1));
    }
    // Initialize the equality constraints matrix
    std::vector<double> f_coeffs = nor_Prob.get_free_coefficients();
    std::vector<std::vector<double>> eq_matrix = nor_Prob.get_var_constraints();
    for (int i = 0; i < eq_matrix.size(); i++) {
        eq_matrix[i].push_back(-1);
    }
    for (int i = 0; i < eq_matrix.size(); i++) {
        this->eq_consts_matrix.push_back({});
        this->eq_consts_matrix[i].push_back(f_coeffs[i]);
        for (int j = 0; j < eq_matrix[i].size(); j++) {
            this->eq_consts_matrix[i].push_back(-eq_matrix[i][j]);
        }
    }
    this->set_status(1);
}

void Phase_1::set_status(int status) {
    this->status = status;
}

int Phase_1::get_status() {
    return this->status;
}

void Phase_1::solve() {
    bool all_positive = true;
    int times = 0;
    std::cout << "\n--Phase 1--\n";
    std::cout << "\nTimes: " << times << std::endl;
    this->display();
    ++times;
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        if (this->eq_consts_matrix[i][0] < -eps) {
            all_positive = false;
            break;
        }
    } 
    if (all_positive) {
        this->set_status(1);
        return;
    } // Check if all b_i are >=0; which means we can go directly to phase 2
    // Start Phase 1
    // Pivot the first time.
    double min = 1e5;
    int row = 0;
    for (int i = 0; i < this->eq_consts_matrix.size(); i++) {
        if (this->eq_consts_matrix[i][0] < min) {
            min = this->eq_consts_matrix[i][0];
            row = i;
        }
    }
    this->pivot_dict(row, this->get_non_basic_vars().size() - 1);
    std::cout << "\nTimes: " << times << std::endl;
    this->display();
    ++times;
    // Pivot from the second time, find pivot point using Bland's rule
    while(true) {
        all_positive = true;
        for (int i = 1; i < this->nb_var_obj.size(); i++) {
            if (this->nb_var_obj[i] < 0) {
                all_positive = false;
                break;
            }
        }
        if (all_positive) {
            if (std::abs(this->nb_var_obj[0]) > eps) {
                this->set_status(2);
            }
            else {
                this->set_status(1);
            }
            return;
        } // Condition to stop Phase 1
		std::vector<int> pos = this->find_pivot();
        this->pivot_dict(pos[0], pos[1]);
        std::cout << "\nTimes: " << times << std::endl;
        this->display();
        ++times;
    }
};

Phase_2::Phase_2() {
    this->set_status(1);
}

void Phase_2::get_Phase1(Phase_1 P1) {
    this->b_vars = P1.get_basic_vars();
    this->nb_vars = P1.get_non_basic_vars();
    this->nb_var_obj = P1.get_objective_function();
    this->eq_consts_matrix = P1.get_eq_consts_matrix();
    this->set_status(1);
}

void Phase_2::set_status(int status) {
    this->status = status;
}
int Phase_2::get_status() {
    return this->status;
}

void Phase_2::handle_vars(Problem nor_Prob) {
    this->b_vars[0] = "z"; // Change the objective function name to "z"
    for (int i = this->nb_var_obj.size() - 1; i > 0; i--) {
        if (std::abs(this->nb_var_obj[i]) > eps) {
            this->nb_vars.erase(this->nb_vars.begin() + i);
            this->nb_var_obj.erase(this->nb_var_obj.begin() + i);
            for (int j = 0; j < this->eq_consts_matrix.size(); j++) {
                this->eq_consts_matrix[j].erase(this->eq_consts_matrix[j].begin() + i);
            }
        }
    } // Remove variables has abs(c_j) > eps, which is not equal to zero, from the objective function in Phase 1
    std::vector<std::string> vars = nor_Prob.get_variables();
    std::vector<double> obj_func_idx = nor_Prob.get_objective_function();
    for (int i = 0; i < vars.size(); i++) {
        for (int j = 1; j < this->nb_vars.size(); j++) {
            if (vars[i] == this->nb_vars[j]) {
                this->nb_var_obj[j] += obj_func_idx[i];
            }
        }
    } // Handle with non-basic variables in nor_Prob to get new objective function
    for (int i = 0; i < vars.size(); i++) {
        for (int j = 1; j < this->b_vars.size(); j++) {
            if (vars[i] == this->b_vars[j]) {
                for (int k = 0; k < this->eq_consts_matrix[j - 1].size(); k++) {
                    this->nb_var_obj[k] += obj_func_idx[i] * this->eq_consts_matrix[j - 1][k];
                }
            }
        }
    } // Handle with basic variables in nor_Prob to get new objective function
}

void Phase_2::solve() {
    bool all_positive = true;
    int times = 0;
    std::cout << "\n--Phase 2--\n";
    std::cout << "\nTimes: " << times << std::endl;
    this->display();
    ++times;
    while (true) {
        all_positive = true;
        for (int i = 1; i < this->nb_var_obj.size(); i++) {
            if (this->nb_var_obj[i] < -eps) {
                all_positive = false;
                break;
            }
        }
        // Check if all b_i are >=0; which means the algorithm is done.
        if (all_positive) {
            bool inf_sol_check = false;
            std::vector<int> pos = {};
            // Get indices of non-basic variables whose values = 0 and names are in form "x..."
            // And its coefficients in the constraints matrix are not all zeros.
            for (int i = 1; i < this->nb_var_obj.size(); i++) {
                if (std::abs(this->nb_var_obj[i]) < eps && this->nb_vars[i][0] == 'x') {
                    for (int j = 0; j < this->eq_consts_matrix.size(); j++) {
                        if (std::abs(this->eq_consts_matrix[j][i]) > eps) {
                            pos.push_back(i);
                        }
                    }
                }
            } 
            std::string var_1, var_2;
            var_1 = {};
            var_2 = {};
            int amount = 0;
            for (int i = 0; i < pos.size(); i++) {
                if (!(this->nb_vars[pos[i]].back() == '+' || this->nb_vars[pos[i]].back() == '-')) {
                    inf_sol_check = true;
                    break;
                } // If that variable is not a free variable, inf_sol_check = true, which means the problem is feasible.
                for (int j = 0; j < this->eq_consts_matrix.size(); j++) {
                    if (std::abs(this->eq_consts_matrix[j][pos[i]]) > eps) {
                        amount++;
                    }
                }
                if (amount >= 2) {
                    inf_sol_check = true;
                    break;
                } // If 2+ its coefficients in the constraint matrix are not 0, inf_sol_check = true, which means the problem is feasible.
                amount = 0;
            }
            // When that variable is a normalized free variable.
            for (int i = 0; i < pos.size(); i++) {
                for (int j = 1; j < this->b_vars.size(); j++) {
                    var_1 = this->b_vars[j];
                    var_1.pop_back();
                    var_2 = this->nb_vars[pos[i]];
                    var_2.pop_back();
                    if (var_1 == var_2) {
                        if (std::abs(this->eq_consts_matrix[j - 1][pos[i]] - 1) >= eps) {
                            inf_sol_check = true;
                            break;
                        } // If x_i(+/-) != x_i(-/+) + something, inf_sol_check = true, which means the problem is feasible.
                    }
                }
            }
            if (inf_sol_check) {
                this->set_status(2);
            }
            else {
                this->set_status(1);
            }
            return;
        } // Condition to stop
        std::vector<int> pos = this->find_pivot();
        if (pos[0] == -1) {
            this->set_status(3);
            return;
        } // Check Unbounded problem.
        this->pivot_dict(pos[0], pos[1]);
        std::cout << "\nTimes: " << times << std::endl;
        this->display();
        ++times;
    }
}

S2P_Method::S2P_Method() {
    this->set_status(1);
}

void S2P_Method::set_problem_goal(int goal) {
    this->problem_goal = goal;
}

int S2P_Method::get_problem_goal() {
	return this->problem_goal;
}

void S2P_Method::solve(Problem Prob) {
    this->set_problem_goal(Prob.get_goal());
    Problem nor_Prob = Prob.normalize_problem();
    nor_Prob.display(0);
    Phase_2 P2;
    {
        Phase_1 P1;
        P1.add_Problem(nor_Prob);
        P1.solve();
        if (P1.get_status() == 2) {
            this->set_status(2);
            return;
        }
        P2.get_Phase1(P1);
    }
    P2.handle_vars(nor_Prob);
    P2.solve();
	if(P2.get_status() == 3) {
		this->set_status(4);
        return;
	}
	else if(P2.get_status() == 2){
		this->set_status(3);
	}
    else {
		this->set_status(1);
	}
	this->get_optimal_solution(P2);
};

void S2P_Method::set_status(int status) {
    this->status = status;
}

int S2P_Method::get_status() {
    return this->status;
}

void S2P_Method::get_optimal_solution(Phase_2 P2) {
    // Get the optimal value
    std::vector<double> nb_var_obj = P2.get_objective_function();
    this->opt_val = nb_var_obj[0];
    if (this->get_status() == 3) {
        return;
    } //If the problem is feasible, then there's nothing to do next. So, return :)
    std::vector<std::string> nb_vars = P2.get_non_basic_vars(); 
    std::vector<std::string> b_vars = P2.get_basic_vars(); 
    nb_vars.erase(nb_vars.begin()); //Remove the first element, which is a null
    b_vars.erase(b_vars.begin()); //Remove the first element, which is "z"
    std::vector<std::vector<double>> eq_consts_matrix = P2.get_eq_consts_matrix();
    for (int i = 0; i < nb_vars.size(); i++) {
        this->opt_vars_list.push_back(nb_vars[i]);
        this->opt_vars_val.push_back(0);
    } // Get the non-basic variables and their values, which are 0.
    for (int i = 0; i < b_vars.size(); i++) {
        this->opt_vars_list.push_back(b_vars[i]);
        this->opt_vars_val.push_back(eq_consts_matrix[i][0]);
    } // Get the basic variables and their values, which are the b_i values.
    for (int i = 0; i < this->opt_vars_list.size(); i++) {
        if (this->opt_vars_list[i].back() == '\'') {
            this->opt_vars_list[i].pop_back();
            this->opt_vars_val[i] *= -1;
        }
    } // Change the name of "x'" variables to "x" and change the sign of the value
    for (int i = 0; i < this->opt_vars_list.size(); i++) {
        for (int j = i + 1; j < this->opt_vars_list.size(); j++) {
            if (this->opt_vars_list[i] > this->opt_vars_list[j]) {
                std::swap(this->opt_vars_list[i], this->opt_vars_list[j]);
                std::swap(this->opt_vars_val[i], this->opt_vars_val[j]);
            }
        }
    } // Sort the variables in ascending order(by ASCII code)
    while(this->opt_vars_list[0][0] == 'w') {
		this->opt_vars_list.erase(opt_vars_list.begin());
		this->opt_vars_val.erase(opt_vars_val.begin());
	} // Remove slack variables
    int idx = 0;
    while(idx < this->opt_vars_list.size()) {
		if (this->opt_vars_list[idx][this->opt_vars_list[idx].size() - 1] == '+') {
            this->opt_vars_list[idx].pop_back(); // Remove the "+" sign
            this->opt_vars_val[idx] -= this->opt_vars_val[idx + 1];
            this->opt_vars_list.erase(this->opt_vars_list.begin() + idx + 1);
            this->opt_vars_val.erase(this->opt_vars_val.begin() + idx + 1);
		}
		idx++;
	} // Get the final values of the free variables
}

void S2P_Method::display() {
    if (this->get_status() == 1) {
        std::cout << "\nOptimal solution has been found!\n";
        std::cout << "Optimal solution: ";
        for (int i = 0; i < this->opt_vars_list.size(); i++) {
            std::cout << this->opt_vars_list[i] << " = ";
            printNumber(this->opt_vars_val[i], findLength(this->opt_vars_val[i]), findDecimals(this->opt_vars_val[i]));
            std::cout << " | ";
        }
        std::cout << std::endl;
        if (this->get_problem_goal() == 1) {
            std::cout << "Optimal value: z = ";
            printNumber(this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        else {
            std::cout << "Optimal value: z = ";
            printNumber(-this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        return;
    }
    if (this->get_status() == 2) {
        std::cout << "\nThe problem is infeasible.\n";
        std::cout << "Optimal solution: None\n";
        if (this->get_problem_goal() == 1) {
            std::cout << "Optimal value: z = +inf\n";
        }
        else {
            std::cout << "Optimal value: z = -inf\n";
        }
        return;
    }
    if (this->get_status() == 3) {
        std::cout << "\nThe problem is feasible.\n";
        std::cout << "Optimal solution: None\n";
        if (this->get_problem_goal() == 1) {
            std::cout << "Optimal value: z = ";
            printNumber(this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        else {
            std::cout << "Optimal value: z = ";
            printNumber(-this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        return;
    }
    if (this->get_status() == 4) {
        std::cout << "\nThe acceptance region is unbounded.\n";
        std::cout << "Optimal solution: None\n";
        if (this->get_problem_goal() == 1) {
            std::cout << "Optimal value: z = -inf\n";
        }
        else {
            std::cout << "Optimal value: z = +inf\n";
        }
    }
}