#ifndef OPERATIONS                           //flag that tree was defined only once in all project
#define OPERATIONS

struct oper {
    const char* name;
    const int code;
    const char n_args;                //* has higher priority than +
    const char complexity;
};

enum type_codes {
    unknown,
    number,
    variable,
    plus,
    minus,
    mul,
    divide,
    power,
    sinus,
    cosinus,
    tg,
    ln,
    arcsin,
    arccos,
    arctg,
    sqroot
};

const oper operations[] = {
        {"unknown", unknown,    0,      0},
        {"number",  number,     0,      1},
        {"var",     variable,   0,      4},
        {"+",       plus,       2,      10},
        {"-",       minus,      2,      10},
        {"*",       mul,        2,      25},
        {"/",       divide,     2,      40},
        {"^",       power,      2,      60},
        {"sin",     sinus,      1,      20},
        {"cos",     cosinus,    1,      20},
        {"tg",      tg,         1,      20},
        {"ln",      ln,         1,      20},
        {"arcsin",  arcsin,     1,      50},
        {"arccos",  arccos,     1,      50},
        {"arctg",   arctg,      1,      50},
        {"sqrt",    sqroot,     1,      40},
};

#endif