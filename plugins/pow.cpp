#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) const char* plugin_name() {
    return "pow";
}

extern "C" __declspec(dllexport) int plugin_arity() {
    return 2;
}

extern "C" __declspec(dllexport) double plugin_eval(const double* args, int argc) {
    if (!args || argc != 2) {
        throw std::runtime_error("pow: arity mismatch");
    } 

    double base = args[0];
    double exponent = args[1];
    return std::pow(base, exponent);
}
