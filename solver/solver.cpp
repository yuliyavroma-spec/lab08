#include "formatter_ex.h"
#include "solver.h"
#include <iostream>

int main() {
    float x1, x2;
    solve(1, -3, 2, x1, x2);
    std::cout << formatter("x1 = " + std::to_string(x1) + ", x2 = " + std::to_string(x2)) << std::endl;
    return 0;
}

