#define d(node)\
node->diff(latex, root)

#define dL left->diff(latex, root)

#define dR right->diff(latex, root)

#define cL left->copy()

#define cR right->copy()

#define c(node)\
node->copy()

#define n(num)\
create_node (num, number, nullptr, nullptr)

#define PLUS(left_l, right_l)\
create_node (0, plus, left_l, right_l)

#define MINUS(left_l, right_l)\
create_node (0, minus, left_l, right_l)

#define MULT(left_l, right_l)\
create_node (0, mul, left_l, right_l)

#define DIVIDE(left_l, right_l)\
create_node (0, divide, left_l, right_l)

#define POW(left_l, right_l)\
create_node (0, power, left_l, right_l)

#define SIN(lf)\
create_node (0, sinus, lf, nullptr)

#define COS(lf)\
create_node (0, cosinus, lf, nullptr)

#define TG(lf)\
create_node (0, tg, lf, nullptr)

#define LN(lf)\
create_node (0, ln, lf, nullptr)

#define ARCSIN(lf)\
create_node (0, arcsin, lf, nullptr)

#define ARCCOS(lf)\
create_node (0, arccos, lf, nullptr)

#define ARCTG(lf)\
create_node (0, arctg, lf, nullptr)

#define SQRT(lf)\
create_node (0, sqroot, lf, nullptr)