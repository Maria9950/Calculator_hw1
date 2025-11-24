#include <string>
#include <vector>

class Loader;

class Calculator {
public:
    Calculator(std::string expression, Loader* loader);
    double evaluate(); 

private:
    bool eof();
    void skip_ws();
    bool match(char c);

    double parse_number();
    double parse_expr();
    double parse_term();
    double parse_unary();
    double parse_primary();

    std::string parse_ident();
    std::vector<double> parse_args();

private:
    std::string s_;
    std::size_t pos_;
    Loader* loader_;
};
