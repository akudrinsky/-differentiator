#ifndef RECURSIVE_DESCENT                           //flag that rec.h was defined only once in all project
#define RECURSIVE_DESCENT

#include "tree.h"

const int max_str = 1000;
const char* s = "";

node* get_g (const char* str);                  //general
node* get_e ();                                 //equation
node* get_t ();                                 //multiplication
node* get_k ();                                 //power
node* get_p ();                                 //brackets
node* get_o ();                                 //one argument functions (ex: ln(x))
node* get_n ();                                 //number

node* get_g (const char* str) {
    s = str;
    printf ("rule g: %s\n", s);

    node* nd = get_e ();

    if (*s != '\0') {
        err_info ("(rec desc): Violation of rule G\n");
    }

    return nd;
}

node* get_e () {
    printf ("rule e: %c\n", *s);
    node* nd = get_t ();

    while (*s == '+' || *s == '-') {
        printf ("rule e: %c\n", *s);
        char op = *s;
        s++;

        if (op == '+') {
            nd = PLUS (nd, get_t ());
        }
        else if (op == '-') {
            nd = MINUS (nd, get_t ());
        }
        else {
            err_info ("(rec desc): strange things in get_e\n");
        }
    }

    return nd;
}

node* get_t () {
    printf ("rule t: %c\n", *s);
    node* nd = get_k ();

    while (*s == '*' || *s == '/') {
        printf ("rule t: %c\n", *s);
        char op = *s;
        s++;

        if (op == '*') {
            nd = MULT (nd, get_k ());
        }
        else if (op == '/') {
            nd = DIVIDE (nd, get_k ());
        }
        else {
            err_info ("(rec desc): strange things in get_t\n");
        }
    }

    return nd;
}

node* get_k () {
    printf ("rule k: %c\n", *s);
    node* nd = get_p ();

    while (*s == '^') {
        printf ("rule k: %c\n", *s);
        s++;

        nd = POW (nd, get_p ());
    }

    return nd;
}

node* get_p () {
    printf ("rule p: %c\n", *s);

    if (*s == '(') {
        s++;
        node* nd = get_e ();

        if (*s != ')') {
            err_info ("(rec desc): Violation of rule P - no closing bracket\n");
        }
        s++;

        return nd;
    }
    else {
        return get_o ();
    }
}

node* get_o () {
    printf ("rule o: %c\n", *s);
    node* nd = new node;

    char* arg = new(std::nothrow) char[default_size];
    if (arg == nullptr) {
        err_info ("problem allocating memory (get_o)\n");
    }

    int scanfed = 0;
    sscanf (s, "%[^()+-*/^]%n", arg, &scanfed);

    printf ("scanfed(%d): %s\n", scanfed, arg);

    if (scanfed == 0) {
        err_info ("(rec desc): Violation of rule N - no digits\n");
    }
    else {
        s += scanfed;

        if (is_num (arg)) {
            nd->type = number;
            nd->data = atof (arg);
        }
        else if (strcmp ("x", arg) == 0) {
            nd->type = variable;
            nd->data = 0.0;
        }
        else {
            nd->type = unknown;
            for (int i = 0; i < el_array (operations); ++i) {
                if (strcmp (operations[i].name, arg) == 0) {
                    nd->type = operations[i].code;                                                             //maybe will have problems here
                }
            }
            nd->data = 0.0;

            nd->left = get_p ();
        }
    }

    if (nd->type == unknown) {
        err_info ("(rec desc): Violation of rule O - unknown operation\n");
    }

    delete[] arg;

    return nd;
}

node* get_n () {
    printf ("rule n: %c\n", *s);
    node* nd = new node;

    int scanfed = 0;
    sscanf (s, "%lg%n", &nd->data, &scanfed);

    if (scanfed == 0) {
        err_info ("(rec desc): Violation of rule N - no digits\n");
    }
    else {
        s += scanfed;
        nd->type = number;
    }

    return nd;
}

#endif