#include "Calculator.h"
#include "loader.h"
#include <cctype>
#include <stdexcept>
#include <cmath>

Calculator::Calculator(std::string expression, Loader* loader)
    : s_(expression), pos_(0), loader_(loader) {
    builtin_ops_["+"] = [](double a, double b) { return a + b; };
    builtin_ops_["-"] = [](double a, double b) { return a - b; };
    builtin_ops_["*"] = [](double a, double b) { return a * b; };
    builtin_ops_["/"] = [](double a, double b) {
        if (b == 0.0) throw std::runtime_error("Division by zero");
        return a / b;
        };
    skip_ws();
}

bool Calculator::eof() {
    return pos_ >= s_.size();
}

void Calculator::skip_ws() {
    while (!eof() && std::isspace(static_cast<unsigned char>(s_[pos_]))) {
        ++pos_;
    }
}

bool Calculator::match(char c) {
    skip_ws();
    if (!eof() && s_[pos_] == c) {
        ++pos_;
        return true;
    }
    return false;
}

double Calculator::parse_number() {
    skip_ws();
    std::size_t start = pos_;
    bool any = false;

    while (!eof() && std::isdigit(static_cast<unsigned char>(s_[pos_]))) {
        ++pos_;
        any = true;
    }
    if (!eof() && s_[pos_] == '.') {
        ++pos_;
        while (!eof() && std::isdigit(static_cast<unsigned char>(s_[pos_]))) {
            ++pos_;
            any = true;
        }
    }
    if (!any) {
        throw std::runtime_error("Expected number");
    }
    return std::stod(s_.substr(start, pos_ - start));
}

std::string Calculator::parse_ident() {
    skip_ws();
    std::size_t start = pos_;
    if (eof()) return std::string();
    unsigned char ch = static_cast<unsigned char>(s_[pos_]);
    if (!(std::isalpha(ch) || s_[pos_] == '_')) {
        return std::string();
    }
    ++pos_;
    while (!eof()) {
        ch = static_cast<unsigned char>(s_[pos_]);
        if (std::isalnum(ch) || s_[pos_] == '_') {
            ++pos_;
        }
        else {
            break;
        }
    }
    return s_.substr(start, pos_ - start);
}

std::vector<double> Calculator::parse_args() {
    std::vector<double> args;
    skip_ws();
    if (match(')')) {
        return args;
    }
    args.push_back(parse_expr());
    skip_ws();
    while (match(',')) {
        args.push_back(parse_expr());
        skip_ws();
    }
    if (!match(')')) {
        throw std::runtime_error("Expected ')'");
    }
    return args;
}

double Calculator::parse_primary() {
    skip_ws();
    if (match('(')) {
        double v = parse_expr();
        if (!match(')')) {
            throw std::runtime_error("Expected ')'");
        }
        return v;
    }
    std::size_t save = pos_;
    std::string id = parse_ident();
    if (!id.empty() && match('(')) {
        std::vector<double> args = parse_args();
        if (!loader_) {
            throw std::runtime_error("No plugin loader available");
        }
        return loader_->call(id, args);
    }

    pos_ = save;
    return parse_number();
}

double Calculator::parse_unary() {
    skip_ws();
    if (match('+')) return parse_unary();
    if (match('-')) return -parse_unary();
    return parse_primary();
}

double Calculator::apply_binary(const std::string& op, double a, double b) {

    std::map<std::string, std::function<double(double, double)> >::iterator it =
        builtin_ops_.find(op);
    if (it == builtin_ops_.end()) {
        throw std::runtime_error("Unknown builtin op '" + op + "'");
    }
    return it->second(a, b);
}

double Calculator::parse_term() {
    double v = parse_unary();
    while (true) {
        skip_ws();
        if (match('*')) {
            double rhs = parse_unary();
            v = apply_binary("*", v, rhs);
        }
        else if (match('/')) {
            double rhs = parse_unary();
            v = apply_binary("/", v, rhs);
        }
        else {
            break;
        }
    }
    return v;
}

double Calculator::parse_expr() {
    double v = parse_term();
    while (true) {
        skip_ws();
        if (match('+')) {
            double rhs = parse_term();
            v = apply_binary("+", v, rhs);
        }
        else if (match('-')) {
            double rhs = parse_term();
            v = apply_binary("-", v, rhs);
        }
        else {
            break;
        }
    }
    return v;
}

double Calculator::evaluate() {
    double value = parse_expr();
    skip_ws();
    if (!eof()) {
        throw std::runtime_error("Unexpected characters at end");
    }
    return value;
}
