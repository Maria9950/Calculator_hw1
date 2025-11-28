#pragma once
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <windows.h>

class Loader {
public: 
    Loader();
    ~Loader();

    void load_all(std::string dir);

    bool has(std::string name);

    double call(std::string name, const std::vector<double>& args);

private:
    struct Func {
        HMODULE module;
        int arity;
        double (*eval)(const double*, int);
    };

    std::map<std::string, Func> funcs_;
    std::vector<HMODULE> modules_;
};