#ifndef _EXPTREE_H_
#define _EXPTREE_H_
// 定义算式类
#include <iostream>
#include "expnode.h"

class ExpTree
{
  private:
    ExpNode *root;
    Number result;
    ExpNode *copyhelp(const ExpNode *);
    void clearhelp(ExpNode *);
    int compnode(const ExpNode *, const ExpNode *) const;
    ExpNode *simp1(ExpNode *);
    void simp2(ExpNode *);
    void simp3(ExpNode *);

  public:
    ExpTree()
    {
        root = NULL;
        result = Number::INF;
    }
    ExpTree(const Number &num)
    {
        root = new LeafNode(num);
        result = num;
    }
    ExpTree(const ExpTree &tree)
    {
        root = copyhelp(tree.root);
        result = tree.result;
    }
    ~ExpTree() { clearhelp(root); }
    ExpTree &operator=(const Number &num)
    {
        clearhelp(root);
        root = new LeafNode(num);
        result = num;
        return *this;
    }
    ExpTree &operator=(const ExpTree &tree)
    {
        if (root == tree.root)
            return *this;
        clearhelp(root);
        root = copyhelp(tree.root);
        result = tree.result;
        return *this;
    }

    Number value() const { return result; } //root->value (); }
    ExpNode *getroot() const { return root; }
    ostream &print(ostream &s) const { return s << *root; }
    ExpTree &simplify();
    bool eq(const ExpTree &tree) const { return compnode(root, tree.root) == 0; }
    bool lt(const ExpTree &tree) const { return compnode(root, tree.root) < 0; }
    bool gt(const ExpTree &tree) const { return compnode(root, tree.root) > 0; }
    int comptree(const ExpTree &tree) const { return compnode(root, tree.root); }

    ExpTree &operator+=(const ExpTree &tree);
    ExpTree &operator-=(const ExpTree &tree);
    ExpTree &operator*=(const ExpTree &tree);
    ExpTree &operator/=(const ExpTree &tree);
};

inline ExpTree &ExpTree::operator+=(const ExpTree &tree)
{
    ExpNode *temp = copyhelp(tree.root);
    root = new IntlNode('+', root, temp);
    result += tree.result;
    return *this;
}

inline ExpTree &ExpTree::operator-=(const ExpTree &tree)
{
    ExpNode *temp = copyhelp(tree.root);
    root = new IntlNode('-', root, temp);
    result -= tree.result;
    return *this;
}

inline ExpTree &ExpTree::operator*=(const ExpTree &tree)
{
    ExpNode *temp = copyhelp(tree.root);
    root = new IntlNode('*', root, temp);
    result *= tree.result;
    return *this;
}

inline ExpTree &ExpTree::operator/=(const ExpTree &tree)
{
    ExpNode *temp = copyhelp(tree.root);
    root = new IntlNode('/', root, temp);
    result /= tree.result;
    return *this;
}

/////////////////////////////////////////////////////

ostream &operator<<(ostream &, const ExpTree &);
bool operator==(const ExpTree &, const ExpTree &);
bool operator!=(const ExpTree &, const ExpTree &);
bool operator<(const ExpTree &, const ExpTree &);
bool operator>(const ExpTree &, const ExpTree &);
bool operator<=(const ExpTree &, const ExpTree &);
bool operator>=(const ExpTree &, const ExpTree &);

ExpTree operator+(const ExpTree &, const ExpTree &);
ExpTree operator-(const ExpTree &, const ExpTree &);
ExpTree operator*(const ExpTree &, const ExpTree &);
ExpTree operator/(const ExpTree &, const ExpTree &);

int comptree(const ExpTree &, const ExpTree &);

#endif
