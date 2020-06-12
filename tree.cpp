//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of wolphram alpha derivatives
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef TREE_WAS_DEFINED                           //flag that tree was defined only once in all project
#define TREE_WAS_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "tree.h"
#include "my_lyb.h"
#include "operations.h"
#include "dsl.h"
#include "comments.h"
#include <new>


#define DEBUG
#ifdef DEBUG
#define ASSERT(cond)\
    if (!(cond)) {\
        printf ("Assert failure: < %s > in file %s, line %d, function %s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
        abort();\
}
#else
#define ASSERT(cond) ;
#endif

#define el_array(name)\
sizeof (name) / sizeof (name[0])

#define get_rand_elem(name)\
name[rand() % (sizeof (name) / sizeof (name[0]))]

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Kind of analog of node construct, but it is more convinient for recursive diff
//!
//! @param [in] data - value
//! @param [in] type - type of node
//! @param [in] left - left leaf
//! @param [in] right - right leaf
//!
//! @return pointer to newly created node
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node* create_node (double data, char type, node* left, node* right) {
    node* nd = new node;
    ASSERT (nd != nullptr)

    nd->data = data;
    nd->type = type;
    nd->left = left;
    nd->right = right;

    if (nd->is_left())
        nd->left->parent = nd;
    if (nd->is_right())
        nd->right->parent = nd;

    ASSERT (nd != nullptr)

    return nd;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Node construct
//!
//! @param [in] type - type of node
//! @param [in] left - left leaf
//! @param [in] right - right leaf
//! @param [in] prnt - parent node
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node::node(const char tp, node* left_l, node* right_l, node* prnt) {
    data = 0;
    left = left_l;
    right = right_l;
    parent = prnt;
    type = tp;                                         //unknown
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! Destructor of a tree
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node::~node() {                         //make it!!!

}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! Safe clearing of a tree
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void node::clear() {
    if (is_left()) {
        left->clear();
        delete left;
    }
    if (is_right()) {
        right->clear();
        delete right;
    }

    left = nullptr;
    right = nullptr;
    parent = nullptr;
    type = unknown;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Merging leaf to a given node from a given side
//!
//! @param [in] where - l for left, r for right
//! @param [in] leaf - leaf to merge
//! @return - if it was OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::merge(node* leaf, char where) {
    if (leaf == nullptr) {
        if (where == 'l') {
            left = nullptr;
        }
        else if (where == 'r') {
            right = nullptr;
        }
        else {
            err_info ("Wrong format of merge, where = ");
            err_info (&where);
            err_info ("\n");
            return false;
        }
    }
    else {
        //printf ("son in merge: \t%p\n", leaf);
        if (where == 'l') {
            left = leaf;
            leaf->parent = this;
        }
        else if (where == 'r') {
            right = leaf;
            leaf->parent = this;
        }
        else {
            err_info ("Wrong format of merge, where = ");
            err_info (&where);
            err_info ("\n");
            return false;
        }
    }

    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Returns pointer to a complete copy of a given node
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node* node::copy() {
    node* cpy = new node;
    cpy->type = type;
    cpy->data = data;
    if (is_left())
        cpy->left = left->copy();
    if (is_right())
        cpy->right = right->copy();
    //printf ("%s\n", operations[type].name);
    return cpy;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes picture of a tree
//!
//! @param [in] pict_name - name of photo
//! @param [in] pict_type - type (ex: png)
//! @param [in] iter - NOT FOR USERS
//! @param [in] pFile - NOT FOR USERS
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void node::photo(const char* pict_name, const char* pict_type, int iter, FILE* pFile) {
    ASSERT (pict_name != nullptr)
    ASSERT (pict_type != nullptr)

    if (iter == 1) {
        pFile = fopen ("tree_graph.dot", "w");
        ASSERT (pFile != nullptr)
        fprintf (pFile, "digraph G{\n\tedge[color=\"chartreuse4\",fontcolor=\"blue\",fontsize=12];\n\tnode[shape=\"rectangle\",fontsize=15];\n");
    }
    ASSERT (pFile != nullptr)

    //vertices
    if (type == 0) {
        fprintf (pFile, "\t%d [shape=record,label=\"  <f0> %p| {unknown | %g} | <f1> %p\" ];\n", iter, left, data, right);
    }
    else if (type == 1) {
        fprintf (pFile, "\t%d [shape=record,label=\"  <f0> %p| {%g | num} | <f1> %p\" ];\n", iter, left, data, right);
    }
    else if (type == 2) {
        fprintf (pFile, "\t%d [shape=record,label=\"  <f0> %p| {var | %g} | <f1> %p\" ];\n", iter, left, data, right);
    }
    else {
        fprintf (pFile, "\t%d [shape=record,label=\"  <f0> %p| {%s | op} | <f1> %p\" ];\n", iter, left, operations[type].name, right);
    }                                                                                                                     //3 - look in operations.h

    //edges
    if (left != nullptr) {
        (*left).photo (pict_name, pict_type, iter * 2, pFile);
        fprintf (pFile, "\t\t%d:<f0> -> %d\n", iter, iter * 2);
    }
    if (right != nullptr) {
        (*right).photo (pict_name, pict_type, iter * 2 + 1, pFile);
        fprintf (pFile, "\t\t%d:<f1> -> %d[color=\"red\"]\n", iter, iter * 2 + 1);
    }

    if (iter == 1) {
        fprintf (pFile, "}");
        fclose (pFile);

        //executing command in terminal
        const int max_cmd_size = 50;
        char command[max_cmd_size] = "dot tree_graph.dot -T ";
        strcat (command, pict_type);
        strcat (command, " -o ");
        strcat (command, pict_name);

        system (command);
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Opens latex file and writes intro information
//!
//! @param [in] filename - name of file to open
//! @return pointer to a file which was opened
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
FILE* node::start_latex(const char *filename) {
    FILE* latex = fopen (filename, "w");
    if (latex == nullptr) {
        err_info ("nullptr while start_latex\n");
        return nullptr;
    }
    else {
        fprintf (latex, "\\documentclass[a4paper,12pt]{article}\n\n"
                        "\\begin{document}\n\n"
                        "\\title{New approaches to derivative analysis}\n"
                        "\\author{Alexey Kudrinsky}\n"
                        "\\date{\\today}\n"
                        "\\maketitle\n\n"
                        "\\section{Introduction}\n"
                        "In this paper we discuss several new approaches to finding function derivatives. Many people, and even mathematicians, would say that is a trick, because it gives you a huge boost in productivity.\nHonor programming!\n"
                        "\\section{Example}\n"
                        );
    }

    this->latex (latex, "Let's consider an example of finding a derivative in an expression that normal universities would call brutal", true, true, true);

    return latex;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes latex info about current node
//!
//! @param [in] pFile - pointer to a file which was opened with start_latex
//! @param [in] comment - str to add before formula
//! @param [in] need_beg, need_der, need_clo - think about it
//! @param [in] iter - NOT FOR USERS
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void node::latex(FILE* pFile, const char* comment, bool need_der, bool need_beg, bool need_clo, int iter) {
    //this->photo("problem.png");

    if (iter == 1) {
        if (need_beg) {
            fprintf (pFile, "\\par %s \\begin{math}", comment);
        }
        if (need_der) {
            fprintf (pFile, "(");
        }
        else {
            fprintf (pFile, " \\to ");
        }
    }

    ++iter;

    switch (type) {
        case number: {
            fprintf (pFile, "%g", data);
            break;
        }
        case variable: {
            fprintf (pFile, "x");
            break;
        }
        case plus: {
            fprintf (pFile, "(");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "+");
            right->latex (pFile, comment, need_der, need_beg, need_clo,iter);
            fprintf (pFile, ")");
            break;
        }
        case minus: {
            fprintf (pFile, "(");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "-");
            right->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, ")");
            break;
        }
        case mul: {
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "*");
            right->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            break;
        }
        case divide: {
            fprintf (pFile, "\\frac{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}{");
            right->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case power: {
            fprintf (pFile, "{(");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, ")}^{");
            right->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case sinus: {
            fprintf (pFile, "\\sin{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case cosinus: {
            fprintf (pFile, "\\cos{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case tg: {
            fprintf (pFile, "\\tan{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case ln: {
            fprintf (pFile, "\\ln{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case arcsin: {
            fprintf (pFile, "\\arcsin{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case arccos: {
            fprintf (pFile, "\\arccos{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case arctg: {
            fprintf (pFile, "\\arctan{");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
        case sqroot: {
            fprintf (pFile, "\\sqrt {");
            left->latex (pFile, comment, need_der, need_beg, need_clo, iter);
            fprintf (pFile, "}");
            break;
        }
    }

    if (iter == 2) {
        if (need_der) {
            fprintf (pFile, ")'");
        }
        if (need_clo) {
            fprintf (pFile, "\\end{math}");
        }
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Convertes latex file to a pdf
//!
//! @param [in] latex - pointer to a file which was opened with start_latex
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::end_latex(FILE* latex) {
    if (latex == nullptr) {
        err_info ("nullptr while start_latex\n");
        return false;
    }
    else {
        fprintf (latex, "\\section{Conclusion}\n"
                        "In previous sections we modestly discussed different techniques, which make differentiation quite easy and even intuitively understandable. Of course, this brief leaflet should be considered only as an introduction to new methods of derivative analysis."
                        "\\section{References}\n"
                        "[1] Kudrinsky, Alexey M. New approaches to derivative analysis. ACM Press, Perm, 2019."
                        "\\end{document}\n"
        );
    }
    fclose (latex);

    system ("pdflatex latex.txt");
    return true;
}

#include "rec.h"

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Saves tree to a txt file format
//!
//! @param [in] filename - name of file to save tree to
//! @param [in] pFile - NOT FOR USERS
//! @param [in] is_first - NOT FOR USERS
//! @param [in] need_closing - NOT FOR USERS
//! @return - if it was OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::save(const char* filename, FILE* pFile, bool is_first, bool need_closing) {
    ASSERT (filename != nullptr)

    bool status = true;
    if (is_first) {
        pFile = fopen (filename, "w");
        ASSERT (pFile != nullptr)
    }

    fprintf (pFile, "(");

    if (left == nullptr && right == nullptr) {
        if (type == number)
            fprintf (pFile, "%lf", data);
        else if (type == variable)
            fprintf (pFile, "x");
        else
            fprintf (pFile, "%s", operations[type]);
    }
    if (left == nullptr && right != nullptr) {
        fprintf (pFile, "@");

        if (type == number)
            fprintf (pFile, "%lf", data);
        else if (type == variable)
            fprintf (pFile, "x");
        else
            fprintf (pFile, "%s", operations[type]);

        if (!(*right).save(filename, pFile, false, false))
            status = false;
    }
    if (left != nullptr && right == nullptr) {
        if (!(*left).save (filename, pFile, false, false))
            status = false;

        if (type == number)
            fprintf (pFile, "%lf", data);
        else if (type == variable)
            fprintf (pFile, "x");
        else
            fprintf (pFile, "%s", operations[type]);

        fprintf (pFile, "@");
    }
    if (left != nullptr && right != nullptr) {
        if (!(*left).save (filename, pFile, false, false))
            status = false;

        if (type == number)
            fprintf (pFile, "%lf", data);
        else if (type == variable)
            fprintf (pFile, "x");
        else
            fprintf (pFile, "%s", operations[type]);

        if (!(*right).save (filename, pFile, false, true))
            status = false;
    }

    fprintf (pFile, ")");

    if (is_first) {
        fclose (pFile);
    }

    return status;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Users function for making tree from txt file
//!
//! @param [in] filename - name of file from which we need to make tree
//!
//! @return if it was good
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::get_tree (const char* filename) {
    ASSERT (filename != nullptr)
    FILE* pFile = fopen (filename, "r");
    ASSERT (pFile != nullptr)

    char* file = read_text (pFile);
    char* cur = file;
    int got_c = 0;

    if (*file == '(') {
        ++cur;
        char* arg = new char[default_size]();
        sscanf (cur, "%[^()]%n", arg, &got_c);
        //printf ("arg: %s\n", arg);
        if (strncmp (arg, "x", 3) == 0) {
            type = variable;
        }
        else if (is_num (arg)) {
            type = number;
            data = atof (arg);
        }
        else {
            for (int i = 0; i < el_array(operations); ++i) {
                if (strncmp (operations[i].name, arg, 5) == 0) {
                    type = operations[i].code;
                }
            }
        }
        cur += got_c + 1;                                       //+ 1 because we need symbol after the last
        delete[] arg;

        //printf ("%d\n", got_c);
        //printf ("node: %s\n", data);
        //printf ("remaining: %s\n", cur);

        if (*cur != ')') {
            get_subtree (this, 'l', &cur);
            get_subtree (this, 'r', &cur);
        }
    }
    else {
        err_info ("Wrong format of saved tree (get_tree)\n");
        free (file);
        fclose (pFile);
        return false;
    }

    free (file);
    fclose (pFile);

    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Recursive function for making tree from txt file
//!
//! @param [in] nd - pointer to node - parent
//! @param [in] where - l for left, r for right
//! @param [in] cur - pointer to text
//!
//! @return if it was good
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool get_subtree (node* nd, char where, char* *cur) {
    ASSERT (cur != nullptr)
    ASSERT (nd != nullptr)

    while (**cur == '(' || **cur == ')') {
        (*cur)++;
    }

    char* leaf_data = new(std::nothrow) char[default_size]();
    if (leaf_data == nullptr) {
        err_info ("Unable to allocate memory for leaf_data in get_subtree\n");
        return false;
    }
    int got_c = 0;

    sscanf (*cur, "%[^()]%n", leaf_data, &got_c);
    *cur += got_c;                                       //+ 1 because we need symbol after the last
    /*
    printf ("arg: %s\n", leaf_data);
    printf ("node: %s\n", leaf_data);
    printf ("remaining: %s\n", *cur);
     */

    if (strcmp (leaf_data, "@") == 0) {
        if (where == 'l') {
            nd->left = nullptr;
        }
        else if (where == 'r') {
            nd->right = nullptr;
        }
        else {
            err_info ("Wrong format in get_subtree\n");
            return false;
        }
    }
    else {
        node* new_nd = new node;
        nd->merge (new_nd, where);
        /*
        printf ("nd (father): %p\n", nd);
        printf ("parent of child: %p\n", new_nd->parent);
        printf ("nd (child): \t%p\n", new_nd);
        printf ("son of parent: \t%p\n\n", nd->left);
         */

        if (strncmp (leaf_data, "x", 3) == 0) {
            new_nd->type = variable;
        }
        else if (is_num (leaf_data)) {
            new_nd->type = number;
            new_nd->data = atof (leaf_data);
        }
        else {
            for (int i = 0; i < sizeof (operations) / sizeof (operations[0]); ++i) {
                if (strncmp (operations[i].name, leaf_data, 5) == 0) {
                    new_nd->type = operations[i].code;                                                             //maybe will have problems here
                }
            }
        }

        if (**cur == '(') {
            //printf ("1\n");
            get_subtree (new_nd, 'l', cur);
            get_subtree (new_nd, 'r', cur);
        }
        else {
            new_nd->left = nullptr;
            new_nd->right = nullptr;
        }
    }

    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Recursive func for getting equation from txt file
//!
//! @param [in] nd - pointer to nd, that we need to make
//! @param [in] cur - pointer to line, where text equation is stored
//!
//! @return if everything was OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool get_subeq (node* nd, char* *cur) {
    if (nd == nullptr) {
        return true;
    }

    int got_c = 0;

    if (*(*cur) == '(') {
        ++(*cur);

        node* new_nd = new node();

        if (*(*cur) == '(') {
            get_subeq (new_nd, &(*cur));
        }

        char* arg = new char[default_size]();
        sscanf (*cur, "%[^()]%n", arg, &got_c);
        //printf ("%s\n", arg);

        if (is_num (arg)) {
            new_nd->type = number;
            new_nd->data = atof (arg);
        }
        else if (strcmp ("x", arg) == 0) {
            new_nd->type = variable;
            new_nd->data = 0.0;
        }
        else {
            new_nd->type = unknown;
            for (int i = 0; i < el_array (operations); ++i) {
                if (strcmp (operations[i].name, arg) == 0) {
                    new_nd->type = operations[i].code;                                                             //maybe will have problems here
                }
            }
            new_nd->data = 0.0;
        }
        (*cur) += got_c;

        if (strcmp ("@", arg) != 0) {
            if (!nd->is_left()) {
                nd->merge (new_nd, 'l');
            }
            else {
                nd->merge (new_nd, 'r');
            }
        }
        else {
            delete new_nd;
        }

        delete[] arg;

        if (*(*cur) == '(') {
            get_subeq (new_nd, &(*cur));
        }

        ++(*cur);       //the last ')'
    }
    else {
        err_info ("Wrong format in get_eq\n");
    }
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Node construct
//!
//! @param [in] filename - name of file from wiwth we need to get an equation
//!
//! @return pointer to newly created node
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::get_eq(const char *filename) {
    ASSERT (filename != nullptr)
    FILE* pFile = fopen (filename, "r");
    ASSERT (pFile != nullptr)

    char* file = read_text (pFile);
    char* cur = file;
    int got_c = 0;

    if (*cur == '(') {
        ++cur;

        if (*cur == '(') {
            get_subeq (this, &cur);
        }

        char* arg = new char[default_size]();
        sscanf (cur, "%[^()]%n", arg, &got_c);

        if (is_num (arg)) {
            type = number;
            data = atof (arg);
        }
        else {
            type = unknown;
            for (int i = 0; i < el_array (operations); ++i) {
                if (strcmp (operations[i].name, arg) == 0) {
                    type = operations[i].code;                                                             //maybe will have problems here
                }
            }
            data = 0.0;
        }
        cur += got_c;

        delete[] arg;

        if (*cur == '(') {
            get_subeq (this, &cur);
        }
    }
    else {
        err_info ("Wrong format in get_eq\n");
    }

    free (file);
    return true;
}

bool node::rand_eq(int max_depth) {
    type = get_rand_elem(operations).code;

    if (type == unknown) {                                      //we don't need any unknown nodes here
        type = variable;
    }

    if (type == number) {
        data = (double)(rand() % 1000) / 100;                                     //100 - we don't need too big numbers
    }

    if (operations[type].n_args == 2) {
        left = recursive_rand_equation (1, max_depth);
        right = recursive_rand_equation (1, max_depth);
    }
    else if (operations[type].n_args == 1) {
        left = recursive_rand_equation (1, max_depth);
    }
}

node* recursive_rand_equation(int iter, int max_depth) {
    if (iter == max_depth) {
        node* nd = new(std::nothrow) node;
        if (nd == nullptr) {
            err_info ("Problems with allocating memory for recursive_rand_equation\n");
            return nullptr;
        }
        if (number != 1 || variable != 2) {
            err_info ("You have changed order of operations in operations.h -> you should do the same in recursive_rand_equation\n");
            return nullptr;
        }
        nd->type = 1 + (rand() % 2);

        if (nd->type == number) {
            nd->data = (double)(rand() % 1000) / 100;
        }

        return nd;
    }
    else {
        printf ("flag\n");
        node* nd = new(std::nothrow) node;
        nd->type = get_rand_elem(operations).code;

        if (nd->type == unknown) {                                      //we don't need any unknown nodes here
            nd->type = variable;
        }

        if (nd->type == number) {
            nd->data = (double)(rand() % 1000) / 100;                                     //100 - we don't need too big numbers
        }

        if (operations[nd->type].n_args == 2) {
            nd->left = recursive_rand_equation (iter + 1, max_depth);
            nd->right = recursive_rand_equation (iter + 1, max_depth);
        }
        else if (operations[nd->type].n_args == 1) {
            nd->left = recursive_rand_equation (iter + 1, max_depth);
        }

        return nd;
    }
}

node* rec_descent(const char *filename) {
    FILE* input = fopen (filename, "r");

    const char* str = read_text (input);
    //printf ("%s", str);

    return get_g (str);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! If a node has left child
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_left() {
    return left != nullptr;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! If a node has right child
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_right() {
    return right != nullptr;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! If a node is a leaf or not
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_leaf() {
    return (right == nullptr && left == nullptr);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! If a tree is valid
//!
//! @return if it was OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_valid() {
    //printf ("valiator launched\n");
    if (is_left()) {
        if (this != this->left->parent) {
            err_info ("Lost connection between node and his left child\n");
        }
        if (!this->left->is_valid()) {
            return false;
        }
    }

    if (is_right()) {
        if (this != this->right->parent) {
            err_info ("Lost connection between node and his right child\n");
            return false;
        }
        if (!this->right->is_valid()) {
            return false;
        }
    }

    return true;
}

bool node::contains_x() {
    bool is_x = false;

    if (is_left() && left->contains_x())
        is_x = true;
    if (is_right() && right->contains_x())
        is_x = true;

    if (type == variable)
        is_x = true;

    return is_x;
}

bool node::simplify(FILE* pFile, node* root, int complexity) {
    printf ("simpl:\t%s,\tval: %lf\n", operations[type].name, data);
    if (root == nullptr) {
        root = this;
    }

    complexity += operations[type].complexity;

    switch (type) {
        case plus: {
            if (is_left())
                left->simplify (pFile, root);

            if (is_right())
                right->simplify (pFile, root);

            if (is_left() && left->type == number && left->data == 0.0) {           // 0 + x
                data = right->data;
                type = right->type;

                delete left;
                left = right->left;
                right = right->right;
            }

            if (is_right() && right->type == number && right->data == 0.0) {        // x + 0
                data = left->data;
                type = left->type;

                delete right;
                right = left->right;
                left = left->left;
            }
            break;
        }
        case minus: {
            if (is_left())
                left->simplify (pFile, root);

            if (is_right())
                right->simplify (pFile, root);

            /*
            if (is_left() && left->type == number && left->data == 0.0) {           // 0 - x
                data = - right->data;
                type = right->type;

                delete left;
                left = right->left;
                right = right->right;
            }
             */

            if (is_right() && right->type == number && right->data == 0.0) {        // x - 0
                data = left->data;
                type = left->type;

                delete right;
                right = left->right;
                left = left->left;
            }
            break;
        }
        case mul: {
            if (is_left()) {
                left->simplify (pFile, root);
            }

            if (is_right()) {
                right->simplify (pFile, root);
            }

            if (is_left() && left->type == number && left->data == 0.0) {           //0 * x
                node* prnt = parent;
                clear ();

                parent = prnt;
                type = number;
                data = 0.0;
            }

            if (is_right() && right->type == number && right->data == 0.0) {        //x * 0
                node* prnt = parent;
                clear ();

                parent = prnt;
                type = number;
                data = 0.0;
            }

            if (is_left() && left->type == number && left->data == 1.0) {           // 1 * x
                data = right->data;
                type = right->type;

                left->clear ();

                if (right->is_left())
                    left = right->left;
                if (right->is_right())
                    right = right->right;

                left->parent = this;
                right->parent = this;
            }

            if (is_right() && right->type == number && right->data == 1.0) {        // x * 1
                data = left->data;
                type = left->type;

                right->clear ();

                if (left->is_right())
                    right = left->right;
                if (left->is_left())
                    left = left->left;
            }

            break;
        }
        case divide: {
            if (is_left())
                left->simplify (pFile, root);

            if (is_right())
                right->simplify (pFile, root);

            if (is_left() && left->type == number && left->data == 0.0) {           //0 / x
                node* prnt = parent;
                clear ();

                parent = prnt;
                type = number;
                data = 0.0;
            }

            if (is_right() && right->type == number && right->data == 1.0) {        // x / 1
                data = left->data;
                type = left->type;

                right->clear ();

                if (left->is_right())
                    right = left->right;
                if (left->is_left())
                    left = left->left;
            }

            break;
        }
        case power: {
            if (is_left())
                left->simplify (pFile, root);

            if (is_right())
                right->simplify (pFile, root);

            if (is_right() && right->type == number && right->data == 1.0) {        // x ^ 1
                data = left->data;
                type = left->type;

                right->clear ();
                if (left->is_right())
                    right = left->right;
                if (left->is_left())
                    left = left->left;
            }

            if (is_right() && right->type == number && right->data == 0.0) {        // x ^ 0
                node* prnt = parent;
                clear ();

                parent = prnt;
                type = number;
                data = 1.0;
            }

            if (is_left() && left->type == number && left->data == 1.0) {           //1 ^ x
                node* prnt = parent;
                clear ();

                parent = prnt;
                type = number;
                data = 1.0;
            }

            if (is_right() && left->type == number && left->data == 0.0) {        // 0 ^ x
                node* prnt = parent;
                clear ();

                parent = prnt;
                type = number;
                data = 0.0;
            }

            break;
        }
        case unknown: {
            return false;
        }
    }

    if (root == this) {
        root->latex (pFile, comments[rand() % el_array (comments)], false, true, true);
    }

    return true;
}
//(+(*(/(sin(x)@)(x))(ln(x)@)))(*(1)(tg(*(x)(ln(x)@))@)))

bool node::count(FILE* pFile) {
    printf ("count:\t%s,\tval: %lf\n", operations[type].name, data);
    switch (type) {
        case number: {
            return true;
        }
        case variable: {
            return true;
        }
        case plus: {
            if (!left->count() || !right->count()) {
                return false;
            }

            if (left->type == number && right->type == number) {
                double res = left->data + right->data;
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case minus: {
            if (!left->count() || !right->count()) {
                return false;
            }

            if (left->type == number && right->type == number) {
                double res = left->data - right->data;
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case mul: {
            if (!left->count() || !right->count()) {
                return false;
            }

            if (left->type == number && right->type == number) {
                double res = left->data * right->data;
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case divide: {
            if (!left->count() || !right->count()) {
                return false;
            }

            if (left->type == number && right->type == number) {
                if (right->data == 0.0) {
                    err_info ("Dividing by zero (in count)\n");
                    return false;
                }
                double res = left->data / right->data;
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case power: {
            if (!left->count() || !right->count()) {
                return false;
            }

            if (left->type == number && right->type == number) {
                double res = pow(left->data, right->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case sinus: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = sin (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case cosinus: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = cos (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case tg: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = tan (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case ln: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = log (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case arcsin: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = asin (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case arccos: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = acos (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case arctg: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = atan (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
        case sqroot: {
            if (!left->count()) {
                return false;
            }

            if (left->type == number) {
                double res = sqrt (left->data);
                this->clear();
                data = res;
                type = number;
            }
            return true;
        }
    }

    err_info ("Something unknown in count function\n");
    return false;
}

node* node::diff(FILE* latex, node* root) {
    node* after_diff = nullptr;

    //err_info ("1\n");
    printf ("diff:\t%s,\tval: %lf\n", operations[type].name, data);
    switch (type) {
        case number: {
            after_diff = n(0);
            break;
        }
        case variable: {
            after_diff = n(1);
            break;
        }
        case plus: {
            after_diff = PLUS (dL, dR);
            break;
        }
        case minus: {
            after_diff = MINUS (dL, dR);
            break;
        }
        case mul: {
            after_diff = PLUS (MULT(dL, cR), MULT(cL, dR));
            break;
        }
        case divide: {
            after_diff = DIVIDE (MINUS (MULT(dL, cR), MULT(dR, cL)), MULT(cR, cR));
            break;
        }
        case power: {                                                                           //new feature
            if (!right->contains_x()) {
                after_diff = MULT (n(right->data), MULT (POW (cL, n(right->data - 1)), dL));
            }
            else if (!left->contains_x()) {
                after_diff = MULT (POW (cL, cR), MULT (LN (cL), dR));
            }
            else {
                after_diff = MULT (POW (cL, cR),
                                   PLUS (MULT (DIVIDE (dL, cL), cR),
                                         MULT (LN (cL), dR)));
            }
            break;
        }
        case sinus: {
            after_diff = MULT (COS (cL), dL);
            break;
        }
        case cosinus: {
            after_diff = MINUS (n(0), MULT (SIN(cL), dL));
            break;
        }
        case tg: {
            after_diff = MULT (DIVIDE (n(1), POW (COS(cL), n(2))), dL);
            break;
        }
        case ln: {
            //after_diff = MULT (DIVIDE (n(1), cL), dL);
            after_diff = DIVIDE (dL, cL);
            break;
        }
        case arcsin: {
            after_diff = MULT (DIVIDE (n(1), SQRT (MINUS (n(1), POW(cL, n(2))))), dL);
            break;
        }
        case arccos: {
            after_diff = MINUS (n(0), MULT (DIVIDE (n(1), SQRT (MINUS (n(1), POW(cL, n(2))))), dL));
            break;
        }
        case arctg: {
            after_diff = MULT (DIVIDE (n(1), PLUS (n(1), POW(cL, n(2)))), dL);
            break;
        }
        case sqroot: {
            after_diff = MULT (DIVIDE (n(1), MULT (n(2), cL)), dL);
            break;
        }
    }

    if (type == unknown) {
        err_info ("Something unknown in diff function\n");
        return nullptr;
    }

    //printf ("gone to latex old %p\n", this);
    this->latex (latex, comments[rand() % el_array (comments)], true, true, false);
    //printf ("gone to latex new %p\n", after_diff);
    after_diff->latex(latex, "", false, false, true);

    return after_diff;
}

//(+(*(/(sin(x)@)(x))(ln(x)@))(*(x)(tg(*(1)(ln(x)@))@)))
//((x)+((x)*(x)))
//((x)+((x)*(((x)ln(@))^((1)tg(@)))))
bool diff_test () {
    srand (time(nullptr));

    node* nd = rec_descent ();
    //node* nd = new node;
    //nd->rand_eq ();
    //nd->is_valid();

    FILE* latex = nd->start_latex ();

    nd->photo ();
    nd->photo ("counted.png");
    nd->save ("new_feature.txt");

    nd->count (latex);
    node* counted = nd->diff (latex);

    counted->photo ("counted.png");
    counted->count (latex);
    counted->simplify (latex);
    counted->photo ("simplified.png");

    counted->save ("new_feature.txt");

    nd->latex (latex, "\\par All in all, the derivative of this crocodile: ", true);
    counted->latex (latex, "", false, true, true);

    nd->end_latex (latex);

    counted->clear ();
}

#endif