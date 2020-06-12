#ifndef TREE_HEADER                           //flag that tree was defined only once in all project
#define TREE_HEADER

#include "operations.h"
#include "dsl.h"

int default_size = 20;

struct node {
    double data;
    node* left;
    node* right;
    node* parent;
    char type;                                              //'n' - number, 'v' - variable

    explicit node (const char tp = unknown, node* left_l = nullptr, node* right_l = nullptr, node* prnt = nullptr);
    ~node ();
    void clear ();

    bool merge (node* leaf, char where);              //r - right, l - left
    node* copy ();
    void photo (const char* pict_name = "tree_graph.png", const char* pict_type = "png", int iter = 1, FILE* pFile = nullptr);
    FILE* start_latex (const char* filename = "latex.txt");
    void latex (FILE* pFile, const char* comment, bool need_der = true, bool need_beg = true, bool need_clo = true, int iter = 1);
    static bool end_latex (FILE* latex);
    bool save (const char* filename = "tree_saved.txt", FILE* pFile = nullptr, bool is_first = true, bool need_closing = true);
    bool get_tree (const char* filename = "tree_saved.txt");
    bool get_eq (const char* filename = "tree_saved.txt");
    bool rand_eq (int max_depth = 5);

    bool is_left ();
    bool is_right ();
    bool is_leaf ();

    bool is_valid();

    bool contains_x ();

    bool simplify (FILE* pFile = nullptr, node* root = nullptr, int complexity = 0);
    bool count (FILE* pFile = nullptr);
    node* diff (FILE* latex, node* root = nullptr);
};

node* recursive_rand_equation(int iter, int max_depth);
bool get_subtree (node* nd, char where, char* *cur);
node* create_node (double data, char type, node* left, node* right);
node* rec_descent (const char* filename = "tree_saved.txt");


#endif