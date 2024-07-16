#include "Classes.h"
#include "ExFuncs.h"

int main() {
    Problem Prob;
    Prob.add_Problem();
    Prob.display(1);
    S2P_Method S2P;
    S2P.solve(Prob);
    S2P.display();
    std::cout << "Press Enter to close the program.";
    std::cin.ignore();
    std::cin.get();
    return 1;
}
