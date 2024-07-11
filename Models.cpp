#include "Classes.h"
#include "ExFuncs.h"
#include <algorithm>
#include <cmath>


void Dict::set_dep_var(std::vector<std::string> dep_var) {
	this->dep_var = dep_var;
}

std::vector<std::string> Dict::get_dep_var() {
    return this->dep_var;
}

void Dict::set_indep_var(std::vector<std::string> indep_var) {
	this->indep_var = indep_var;
}

std::vector<std::string> Dict::get_indep_var() {
    return this->indep_var;
}

void Dict::set_indep_var_obj(std::vector<double> indep_var_obj) {
	this->indep_var_obj = indep_var_obj;
}

std::vector<double> Dict::get_indep_var_object() {
    return this->indep_var_obj;
}

void Dict::set_eq_constrs_matrix(std::vector<std::vector<double>> eq_constrs_matrix) {
	this->eq_constrs_matrix = eq_constrs_matrix;
}

std::vector<std::vector<double>> Dict::get_eq_constrs_matrix() {
    return this->eq_constrs_matrix;
}

std::vector<int> Dict::find_pivot() {
    int col = -1;
    std::string var = {};

    for (int i = 1; i < this->indep_var_obj.size(); i++) {
        if (this->indep_var_obj[i] < 0) {
            col = i;
            var = this->indep_var[i];
            break;
        }
    }//Get first negative value of the objective function, also get column to pivot

    for (int i = col + 1; i < this->indep_var_obj.size(); i++) {
        if (this->indep_var_obj[i] < 0) {
            if (this->indep_var[i][0] != var[0]) {
                if (this->indep_var[i] > var) {
                    col = i;
                    var = this->indep_var[i];
                }
            }
            if (this->indep_var[i][0] == var[0]) {
                if (this->indep_var[i] < var) {
                    col = i;
                    var = this->indep_var[i];
                }
            }
        }
    }

    double min = 1e7;
    int row = -1;

    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        if (this->eq_constrs_matrix[i][col] >= 0) {
            continue;
        }
        min = this->eq_constrs_matrix[i][0] / -this->eq_constrs_matrix[i][col];
        row = i;
        var = this->dep_var[i + 1];
        break;
    }

    for (int i = row + 1; i < this->eq_constrs_matrix.size(); i++) {
        if (this->eq_constrs_matrix[i][col] >= 0) {
            continue;
        }
        if ((this->eq_constrs_matrix[i][0] / -this->eq_constrs_matrix[i][col]) - min < -(1e-5)) {
            row = i;
            min = this->eq_constrs_matrix[i][0] / -this->eq_constrs_matrix[i][col];
            var = this->dep_var[i + 1];
        }
    }

    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        if (this->eq_constrs_matrix[i][col] >= 0) {
            continue;
        }
        if (std::abs((this->eq_constrs_matrix[i][0] / -this->eq_constrs_matrix[i][col]) - min) < 1e-5) {
            if (this->dep_var[i + 1][0] != var[0]) {
                if (this->dep_var[i + 1] > var) {
                    row = i;
                    var = this->dep_var[i + 1];
                }
            }
            if (this->dep_var[i + 1][0] == var[0]) {
                if (this->dep_var[i + 1] < var) {
                    row = i;
                    var = this->dep_var[i + 1];
                }
            }
        }
    }

    std::vector<int> pos = {row, col};
    return pos;
}

void Dict::pivot_dict(int row, int col) {
    std::swap(dep_var[row + 1], indep_var[col]); //Swap the entering and leaving variables
    double value = this->eq_constrs_matrix[row][col]; //Get the pivot value
    this->eq_constrs_matrix[row][col] = -1; //Set the pivot value in matrix to -1
    for (int i = 0; i < this->eq_constrs_matrix[row].size(); i++) {
        this->eq_constrs_matrix[row][i] /= -value;
    } // Get a new vector for the row

    value = this->indep_var_obj[col];
    this->indep_var_obj[col] = 0;
    for (int i = 0; i < this->indep_var_obj.size(); i++) {
        this->indep_var_obj[i] += value * this->eq_constrs_matrix[row][i];
    } //Handle with the objective function

    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        if (i == row) {
            continue;
        }
        value = this->eq_constrs_matrix[i][col];
        this->eq_constrs_matrix[i][col] = 0;
        for (int j = 0; j < this->eq_constrs_matrix[i].size(); j++) {
            this->eq_constrs_matrix[i][j] += value * this->eq_constrs_matrix[row][j];
        }
    } //Handle with the rest of the constraints
}

void Dict::display() {
    std::string max_var = this->dep_var[0];
    for (int i = 1; i < this->dep_var.size(); i++) {
		if (this->dep_var[i].length() > max_var.length()) {
			max_var = this->dep_var[i];
		}
	}
    int max_len = max_var.length();

    std::vector<int> max_length = {};

    for (int i = 0; i < this->indep_var_obj.size(); i++) {
        max_length.push_back(findLength(std::abs(this->indep_var_obj[i])));
    }
    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        for (int j = 0; j < this->eq_constrs_matrix[i].size(); j++) {
            if (findLength(std::abs(this->eq_constrs_matrix[i][j])) > max_length[j]) {
                max_length[j] = findLength(std::abs(this->eq_constrs_matrix[i][j]));
            }
        }
    }

    std::cout << this->dep_var[0];
    printChars(' ', max_len - this->dep_var[0].length());
    std::cout << " = ";

    printNum(this->indep_var_obj[0], max_length[0] + 1, findDecimals(std::abs(this->indep_var_obj[0])));
    std::cout << this->indep_var[0];

    for (int i = 1; i < this->indep_var.size(); i++) {
        if (this->indep_var_obj[i] < 0) {
            std::cout << " - ";
        }
        else {
            std::cout << " + ";
        }
        printNum(std::abs(this->indep_var_obj[i]), max_length[i], findDecimals(std::abs(this->indep_var_obj[i])));
        std::cout << this->indep_var[i];
    }

    int hyphenNum = max_len + 3 + max_length[0] + 1 + 3 * (this->indep_var.size() - 1);
    for (int i = 1; i < this->indep_var.size(); i++) {
        hyphenNum += max_length[i] + this->indep_var[i].length();
    }

    std::cout << std::endl;
    printChars('-', hyphenNum);
    std::cout << std::endl;

    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        std::cout << this->dep_var[i + 1];
        printChars(' ', max_len - this->dep_var[i + 1].length());

        std::cout << " = ";

        printNum(this->eq_constrs_matrix[i][0], max_length[0] + 1, findDecimals(std::abs(this->eq_constrs_matrix[i][0])));
        std::cout << this->indep_var[0];

        for (int j = 1; j < this->indep_var.size(); j++) {
            if (this->eq_constrs_matrix[i][j] < 0) {
				std::cout << " - ";
			}
			else {
				std::cout << " + ";
			}
            printNum(std::abs(this->eq_constrs_matrix[i][j]), max_length[j], findDecimals(std::abs(this->eq_constrs_matrix[i][j])));
            std::cout << this->indep_var[j];
        }
        std::cout << std::endl;
    }
}
/*---------------------------------------------------------------*/
Phase_1::Phase_1(Problem nor_Prob) {
    //Initialize the independent variables
    this->indep_var.push_back({});
    std::vector<std::string> vars = nor_Prob.get_variables();
    for (int i = 0; i < vars.size(); i++) {
        this->indep_var.push_back(vars[i]);
    }
    this->indep_var.push_back("x0");

    //Initialize the independent variables coefficients
    for (int i = 0; i < this->indep_var.size() - 1; i++) {
        this->indep_var_obj.push_back(0);
    }
    this->indep_var_obj.push_back(1);

    //Initialize the dependent variables
    this->dep_var.push_back("*");
    for (int i = 0; i < nor_Prob.get_var_constrs().size(); i++) {
        this->dep_var.push_back("w" + std::to_string(i + 1));
    }

    //Initialize the equality constraints matrix
    std::vector<double> f_coeffs = nor_Prob.get_f_coeffs();
    std::vector<std::vector<double>> eq_matrix = nor_Prob.get_var_constrs();
    for (int i = 0; i < eq_matrix.size(); i++) {
        eq_matrix[i].push_back(-1);
    }
    
    for (int i = 0; i < eq_matrix.size(); i++) {
        this->eq_constrs_matrix.push_back({});
        this->eq_constrs_matrix[i].push_back(f_coeffs[i]);
        for (int j = 0; j < eq_matrix[i].size(); j++) {
            this->eq_constrs_matrix[i].push_back(-eq_matrix[i][j]);
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
    Dict result;
    int times = 0;

    std::cout << "\n--Phase 1--\n";
    std::cout << "\nTimes: " << times << std::endl;
    this->display();
    ++times;
    
    double eps = 1e-5;
    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        if (this->eq_constrs_matrix[i][0] < -eps) {
            all_positive = false;
            break;
        }
    } //Check if all b_i are >=0; which means we can go directly to phase 2

    if (all_positive) {
        result.set_dep_var(this->get_dep_var());
        result.set_indep_var(this->get_indep_var());
        result.set_indep_var_obj(this->get_indep_var_object());
        result.set_eq_constrs_matrix(this->get_eq_constrs_matrix());

        this->set_status(1);
        return;
    }
    
    //Phase 1

    //Pivot the first time.
    double min = 1e5;
    int row = 0;
    for (int i = 0; i < this->eq_constrs_matrix.size(); i++) {
        if (this->eq_constrs_matrix[i][0] < min) {
            min = this->eq_constrs_matrix[i][0];
            row = i;
        }
    }
    this->pivot_dict(row, this->get_indep_var().size() - 1);
    std::cout << "\nTimes: " << times << std::endl;
    this->display();
    ++times;

    //Pivot from the second time, find pivot point using Bland's rule
    while(true) {
        all_positive = true;
        for (int i = 1; i < this->indep_var_obj.size(); i++) {
            if (this->indep_var_obj[i] < 0) {
                all_positive = false;
                break;
            }
        }

        if (all_positive) {
            double eps = 1e-5;
            if (std::abs(this->indep_var_obj[0]) > eps) {
                this->set_status(2);
            }
            else {
                this->set_status(1);
            }
            return;
        } //Condition to stop Phase 1

		std::vector<int> pos = this->find_pivot();
        this->pivot_dict(pos[0], pos[1]);
        std::cout << "\nTimes: " << times << std::endl;
        this->display();
        ++times;
    }
};
/*-----------------------------------------------------------------*/
Phase_2::Phase_2(Phase_1 result) {
    this->dep_var = result.get_dep_var();
    this->indep_var = result.get_indep_var();
    this->indep_var_obj = result.get_indep_var_object();
    this->eq_constrs_matrix = result.get_eq_constrs_matrix();

    this->set_status(1);
}

void Phase_2::set_status(int status) {
    this->status = status;
}
int Phase_2::get_status() {
    return this->status;
}

void Phase_2::handle_vars(Problem nor_Prob) {
    this->dep_var[0] = "z"; //Change the objective function name to "z"

    double eps = 1e-5;
    for (int i = this->indep_var_obj.size() - 1; i > 0; i--) {
        if (std::abs(this->indep_var_obj[i]) > eps) {
            this->indep_var.erase(this->indep_var.begin() + i);
            this->indep_var_obj.erase(this->indep_var_obj.begin() + i);
            for (int j = 0; j < this->eq_constrs_matrix.size(); j++) {
                this->eq_constrs_matrix[j].erase(this->eq_constrs_matrix[j].begin() + i);
            }
        }
    } //Remove variables has abs(c_j) > eps, which is not equal to zero, from the objective function in Phase 1

    std::vector<std::string> vars = nor_Prob.get_variables();
    std::vector<double> obj_idx = nor_Prob.get_object_index();

    for (int i = 0; i < vars.size(); i++) {
        for (int j = 1; j < this->indep_var.size(); j++) {
            if (vars[i] == this->indep_var[j]) {
                this->indep_var_obj[j] += obj_idx[i];
            }
        }
    } //Handle with non-basic variables in nor_Prob to get new objective function

    for (int i = 0; i < vars.size(); i++) {
        for (int j = 1; j < this->dep_var.size(); j++) {
            if (vars[i] == this->dep_var[j]) {
                for (int k = 0; k < this->eq_constrs_matrix[j - 1].size(); k++) {
                    this->indep_var_obj[k] += obj_idx[i] * this->eq_constrs_matrix[j - 1][k];
                }
            }
        }
    } //Handle with basic variables in nor_Prob to get new objective function
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

        double eps = 1e-5;

        for (int i = 1; i < this->indep_var_obj.size(); i++) {
            if (this->indep_var_obj[i] < -eps) {
                all_positive = false;
                break;
            }
        }

        if (all_positive) {
            bool inf_sol_check = false;

            std::vector<int> pos = {};

            for (int i = 1; i < this->indep_var_obj.size(); i++) {
                if (std::abs(this->indep_var_obj[i]) < eps && this->indep_var[i][0] == 'x') {
                    for (int j = 0; j < this->eq_constrs_matrix.size(); j++) {
                        if (std::abs(this->eq_constrs_matrix[j][i]) > eps) {
                            pos.push_back(i);
                        }
                    }
                }
            }
            std::string temp1, temp2;
            temp1 = {};
            temp2 = {};
            
            int amount = 0;
            for (int i = 0; i < pos.size(); i++) {
                if (!(this->indep_var[pos[i]].back() == '+' || this->indep_var[pos[i]].back() == '-')) {
                    inf_sol_check = true;
                    break;
                }

                for (int j = 0; j < this->eq_constrs_matrix.size(); j++) {
                    if (std::abs(this->eq_constrs_matrix[j][pos[i]]) > eps) {
                        amount++;
                    }
                }
                if (amount >= 2) {
                    inf_sol_check = true;
                    break;
                }
                amount = 0;
            }

            for (int i = 0; i < pos.size(); i++) {
                if (!(this->indep_var[pos[i]].back() == '+' || this->indep_var[pos[i]].back() == '-')) {
                    inf_sol_check = true;
                    break;
                }

                for (int j = 1; j < this->dep_var.size(); j++) {
                    temp1 = this->dep_var[j];
                    temp1.pop_back();
                    temp2 = this->indep_var[pos[i]];
                    temp2.pop_back();
                    if (temp1 == temp2) {
                        if (this->eq_constrs_matrix[j - 1][pos[i]] <= 1 - eps || this->eq_constrs_matrix[j - 1][pos[i]] >= 1 + eps) {
                            inf_sol_check = true;
                            break;
                        }
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
        }//Condition to stop

        std::vector<int> position = this->find_pivot();

        if (position[0] == -1) {
            this->set_status(3);
            return;
        } //Unbounded conditions check.
        this->pivot_dict(position[0], position[1]);
        std::cout << "\nTimes: " << times << std::endl;
        this->display();
        ++times;
    }
}
/*------------------------------------------------*/
S2P_Method::S2P_Method(Problem Prob) {
    this->Prob = Prob;
    this->set_status(1);
}

void S2P_Method::solve() {
    Problem nor_Prob = this->Prob.normalize_problem();
    nor_Prob.display(0);

    Phase_1 Phase1(nor_Prob);
    this->Phase1 = Phase1;
    this->Phase1.solve();
    if(this->Phase1.get_status() == 2) {
		this->set_status(2);
		return;
	}

    Phase_2 Phase2(this->Phase1);
	this->Phase2 = Phase2;
	this->Phase2.handle_vars(nor_Prob);
	this->Phase2.solve();
	if(this->Phase2.get_status() == 3) {
		this->set_status(4);
	}
	else if(this->Phase2.get_status() == 2){
		this->set_status(3);
	}
    else {
		this->set_status(1);
	}

	this->get_opt_vals();
};

void S2P_Method::set_status(int status) {
    this->status = status;
}

int S2P_Method::get_status() {
    return this->status;
}

void S2P_Method::get_opt_vals() {
    if (this->get_status() == 2 || this->get_status() == 4) {
        return;
    }

    std::vector<double> vals = this->Phase2.get_indep_var_object();
    this->opt_val = vals[0];

    if (this->get_status() == 3) {
        return;
    }

    std::vector<std::string> vars_1 = this->Phase2.get_indep_var();
    std::vector<std::string> vars_2 = this->Phase2.get_dep_var();

    vars_1.erase(vars_1.begin()); //Remove the first element, which is a null
    vars_2.erase(vars_2.begin()); //Remove the first element, which is "z"

    std::vector<std::vector<double>> eq_matrix = this->Phase2.get_eq_constrs_matrix();

    for (int i = 0; i < vars_1.size(); i++) {
        this->opt_vars_list.push_back(vars_1[i]);
        this->opt_vars_val.push_back(0);
    }

    for (int i = 0; i < vars_2.size(); i++) {
        this->opt_vars_list.push_back(vars_2[i]);
        this->opt_vars_val.push_back(eq_matrix[i][0]);
    }

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
    } //Sort the variables in ascending order(by ASCII code)

    while(this->opt_vars_list[0][0] == 'w') {
		this->opt_vars_list.erase(opt_vars_list.begin());
		this->opt_vars_val.erase(opt_vars_val.begin());
	} //Remove slack variables

    int index = 0;
    while(index < this->opt_vars_list.size()) {
		if (this->opt_vars_list[index][this->opt_vars_list[index].size() - 1] == '+') {
            this->opt_vars_list[index].pop_back(); //Remove the "+" sign
            this->opt_vars_val[index] -= this->opt_vars_val[index + 1];
            this->opt_vars_list.erase(this->opt_vars_list.begin() + index + 1);
            this->opt_vars_val.erase(this->opt_vars_val.begin() + index + 1);
		}
		index++;
	}//Get the final values of the free variables
}

void S2P_Method::display() {
    if (this->get_status() == 1) {
        std::cout << "\nOptimal problem\n";
        std::cout << "Optimal solution: ";
        for (int i = 0; i < this->opt_vars_list.size(); i++) {
            std::cout << this->opt_vars_list[i] << " = ";
            printNum(this->opt_vars_val[i], findLength(this->opt_vars_val[i]), findDecimals(this->opt_vars_val[i]));
            std::cout << " | ";
        }
        std::cout << std::endl;
        if (this->Prob.get_goal() == 1) {
            std::cout << "Optimal value: z = ";
            printNum(this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        else {
            std::cout << "Optimal value: z = ";
            printNum(-this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        return;
    }

    if (this->get_status() == 2) {
        std::cout << "\nInfeasible problem\n";
        std::cout << "Optimal solution: None\n";
        if (this->Prob.get_goal() == 1) {
            std::cout << "Optimal value: z = +inf\n";
        }
        else {
            std::cout << "Optimal value: z = -inf\n";
        }
        return;
    }

    if (this->get_status() == 3) {
        std::cout << "\nFeasible problem\n";
        std::cout << "Optimal solution: None\n";
        if (this->Prob.get_goal() == 1) {
            std::cout << "Optimal value: z = ";
            printNum(this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        else {
            std::cout << "Optimal value: z = ";
            printNum(-this->opt_val, findLength(this->opt_val), findDecimals(this->opt_val));
            std::cout << "\n";
        }
        return;
    }

    if (this->get_status() == 4) {
        std::cout << "\nUnbounded problem\n";
        std::cout << "Optimal solution: None\n";
        if (this->Prob.get_goal() == 1) {
            std::cout << "Optimal value: z = -inf\n";
        }
        else {
            std::cout << "Optimal value: z = +inf\n";
        }
        return;
    }
}