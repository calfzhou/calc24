#ifndef _OPERATOR_H_
#define _OPERATOR_H_
// 定义操作符类
#include <iostream>
#include "head.h"
#include "number.h"

class Operator
{ // 运算符class
    typedef Number (*ACT)(const Number &, const Number &);

  private:
    char m_name;      // 名称，符号
    char m_invopname; // 逆运算名称
    ACT m_func;       // 动作
    int m_level;      // 优先级
    bool m_duichen;   // 交换对称性

    Operator(char op, char invop, ACT act, int l, bool dc)
    {
        m_name = op;
        m_invopname = invop;
        m_func = act;
        m_level = l;
        m_duichen = dc;
    }

  public:
    static Operator ADD;
    static Operator SUB;
    static Operator MUL;
    static Operator DIV;

    Operator(char op = '+');
    ~Operator() {}

    char name() const { return m_name; }
    char invopname() const { return m_invopname; }
    ACT func() const { return m_func; }
    int level() const { return m_level; }
    bool duichen() const { return m_duichen; }
};

ostream &operator<<(ostream &, const Operator &);
Operator inv(const Operator &);

#endif
