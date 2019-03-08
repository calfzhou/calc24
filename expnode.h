#ifndef _EXPNODE_H_
#define _EXPNODE_H_
// 定义算式结点类
#include <iostream>
#include "head.h"
#include "number.h"
#include "operator.h"

class ExpNode
{ // expression tree node class
  public:
    virtual bool isLeaf() const = 0;
    virtual Number value() const = 0;
    virtual int level() const = 0; // 运算优先级，常数最高(为0)
    virtual ~ExpNode() {}
};

class LeafNode : public ExpNode
{ // expression tree leaf node class
  private:
    Number var;

  public:
    LeafNode() { var = var.INF; }
    LeafNode(const Number &val) { var = val; }
    ~LeafNode() {}

    bool isLeaf() const { return true; }
    Number value() const { return var; } // get value
    int level() const { return 0; }

    void setValue(const Number &val) { var = val; } // set value
};

class IntlNode : public ExpNode
{ // expression tree internal node class
  private:
    Operator op;
    ExpNode *l;
    ExpNode *r;
    static IntlNode *freelist;

  public:
    IntlNode(char ch = '+', ExpNode *left = NULL, ExpNode *right = NULL)
    {
        op = Operator(ch);
        l = left;
        r = right;
    }
    ~IntlNode() {}

    bool isLeaf() const { return false; }
    Number value() const { return op.func()(l->value(), r->value()); }
    int level() const { return op.level(); }

    ExpNode *left() const { return l; }
    ExpNode *right() const { return r; }
    char oper() const { return op.name(); }
    Operator Oper() const { return op; }
    bool canjiaohuan() const
    {
        return op.func()(l->value(), r->value()) ==
               op.func()(r->value(), l->value());
    }
    void setLeft(ExpNode *left) { l = left; }
    void setRight(ExpNode *right) { r = right; }
    void setOper(char ch) { op = Operator(ch); }

    void *operator new(size_t);   // Overload new
    void operator delete(void *); // Overload delete
};

ostream &operator<<(ostream &, const ExpNode &);

#endif
