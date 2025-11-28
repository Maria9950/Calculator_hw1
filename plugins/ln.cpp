#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) const char* plugin_name() {
    return "ln";
}

extern "C" __declspec(dllexport) int plugin_arity() {
    return 1;
}

extern "C" __declspec(dllexport) double plugin_eval(const double* args, int argc) {
    if (!args || argc != 1) {
        throw std::runtime_error("ln: arity mismatch"); 
    }

    double x = args[0];
    if (x <= 0.0) {
        throw std::runtime_error("ln: domain error (x <= 0)");
    }

    return std::log(x);
}
