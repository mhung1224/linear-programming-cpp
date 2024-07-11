#include "Classes.h"
#include "ExFuncs.h"


int main() {

    Problem Prob;
    Prob.add_Problem();
    Prob.display(1);
    S2P_Method S2P(Prob);
    S2P.solve();
    S2P.display();
    return 1;
}