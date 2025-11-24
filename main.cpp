#include "Calculator.h"
#include "loader.h"
#include <iostream>
#include <cmath>
#include <string>

std::string format_number(double x) {
    if (!(x == x) || !std::isfinite(x)) {
        return "nan";
    }

    std::string s = std::to_string(x);

    std::string::size_type p = s.find('.');
    if (p != std::string::npos) {
        while (!s.empty() && s.back() == '0') {
            s.pop_back();
        }
        if (!s.empty() && s.back() == '.') {
            s.pop_back();
        }
    }
    return s;
}

int main(int argc, char* argv[]) {
    try {
        Loader loader;
        loader.load_all("plugins");

        std::string expr;
        if (argc > 1) {
            int i;
            for (i = 1; i < argc; ++i) {
                if (i > 1) expr += ' ';
                expr += argv[i];
            }
        }
        else {
            std::getline(std::cin, expr); 
        }

        Calculator calc(expr, &loader);
        double res = calc.evaluate();
        std::cout << format_number(res) << "\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "fatal: " << e.what() << "\n";
        return 2;
    }
    catch (...) {
        std::cerr << "fatal: unknown error\n";
        return 3;
    }
}
