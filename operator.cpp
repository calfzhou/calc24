#include "operator.h"

Operator Operator::ADD =
    Operator('+', '-', operator+, 1, true);
Operator Operator::SUB =
    Operator('-', '+', operator-, 1, false);
Operator Operator::MUL =
    Operator('*', '/', operator*, 2, true);
Operator Operator::DIV =
    Operator('/', '*', operator/, 2, false);

Operator::Operator(char op)
{
    switch (op)
    {
    case '+':
        *this = ADD;
        break;
    case '-':
        *this = SUB;
        break;
    case '*':
        *this = MUL;
        break;
    case '/':
        *this = DIV;
        break;
    default:
        *this = ADD;
    }
}

////////////////////////////////////////////////////

ostream &operator<<(ostream &s, const Operator &op)
{
    return s << op.name();
}

Operator inv(const Operator &op)
{
    return Operator(op.invopname());
}
