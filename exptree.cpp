#include "exptree.h"

ExpNode *ExpTree::copyhelp(const ExpNode *scrroot)
{
    ExpNode *dstroot;
    if (scrroot == NULL)
        dstroot = NULL;
    else if (scrroot->isLeaf())
        dstroot = new LeafNode(((LeafNode *)scrroot)->value());
    else
    { // is internal node
        ExpNode *l = copyhelp(((IntlNode *)scrroot)->left());
        ExpNode *r = copyhelp(((IntlNode *)scrroot)->right());
        dstroot = new IntlNode(((IntlNode *)scrroot)->oper(), l, r);
    }
    return dstroot;
}

void ExpTree::clearhelp(ExpNode *subroot)
{
    if (subroot == NULL)
        return;
    if (!subroot->isLeaf())
    {
        clearhelp(((IntlNode *)subroot)->left());
        clearhelp(((IntlNode *)subroot)->right());
    }
    delete subroot;
}

// simp1使同级的运算符尽可能的排成左链（这样可减少不必要的括号）
// 比如对于加减：
//   +         +      +         -      -         -      -         +
//  / \       / \    / \       / \    / \       / \    / \       / \
// a   +  => +   c  a   -  => +   c  a   +  => -   c  a   -  => -   c
//    / \   / \        / \   / \        / \   / \        / \   / \
//   b   c a   b      b   c a   b      b   c a   b      b   c a   b
ExpNode *ExpTree::simp1(ExpNode *subroot)
{
    if (subroot == NULL || subroot->isLeaf())
        return subroot;
    IntlNode *rt = (IntlNode *)subroot;

    while (!rt->right()->isLeaf() && rt->level() == rt->right()->level())
    {
        IntlNode *r = (IntlNode *)rt->right();
        if (!rt->Oper().duichen()) // -, /
            r->setOper(r->Oper().invopname());
        rt->setRight(r->left());
        r->setLeft(rt);
        rt = r;
    }

    rt->setLeft(simp1(rt->left()));
    rt->setRight(simp1(rt->right()));

    return rt;
}

// simp2使排成左链的同级的运算符中的不对称符聚集在上部
// 比如：
//     +         -        *         /
//    / \       / \      / \       / \
//   -   c  => +   b    /   c  => *   b
//  / \       / \      / \       / \
// a   b     a   c    a   b     a   c
void ExpTree::simp2(ExpNode *subroot)
{
    if (subroot == NULL || subroot->isLeaf())
        return;
    IntlNode *rt = (IntlNode *)subroot;

    if (!rt->left()->isLeaf() && rt->Oper().duichen())
    {
        IntlNode *l = (IntlNode *)rt->left();
        while (rt->level() == l->level())
        {
            if (!l->Oper().duichen())
            {
                rt->setOper(rt->Oper().invopname());
                l->setOper(l->Oper().invopname());
                ExpNode *temp = rt->right();
                rt->setRight(l->right());
                l->setRight(temp);
                break;
            }
            if (l->left()->isLeaf())
                break;
            l = (IntlNode *)l->left();
        }
    }

    simp2(rt->left());
    simp2(rt->right());
}

// simp3使排成左链的同级的有相同对称性的运算符的子树按复杂度排序
// 比如：
//     +         +        -         -
//    / \       / \      / \       / \
//   +   1  => +   3    -   1  => -   3
//  / \       / \      / \       / \
// 2   3     1   2    2   3     2   1
void ExpTree::simp3(ExpNode *subroot)
{
    if (subroot == NULL || subroot->isLeaf())
        return;
    IntlNode *rt = (IntlNode *)subroot;

    simp3(rt->left());
    simp3(rt->right());

    if (rt->left()->level() != rt->level())
    {
        if (rt->canjiaohuan() && compnode(rt->left(), rt->right()) > 0)
        {
            ExpNode *temp = rt->right();
            rt->setRight(rt->left());
            rt->setLeft(temp);
        }
        return;
    }
    IntlNode *l = (IntlNode *)rt->left();
    IntlNode *p = rt; // l's parent
    while (compnode(l->right(), p->right()) > 0)
    {
        if (l->oper() == rt->oper())
        {
            ExpNode *temp = p->right();
            p->setRight(l->right());
            l->setRight(temp);
            if (l->left()->isLeaf())
            {
                if (l->canjiaohuan() && compnode(l->left(), l->right()) > 0)
                {
                    temp = l->right();
                    l->setRight(l->left());
                    l->setLeft(temp);
                }
                break;
            }
            p = l;
            l = (IntlNode *)p->left();
        }
        else
        {
            if (p->canjiaohuan() && compnode(l, p->right()) > 0)
            {
                ExpNode *temp = p->right();
                p->setRight(l);
                p->setLeft(temp);
            }
            break;
        }
    }
}
// simp系列函数尚未实现：
// 排成左链的同级的不同对称性的运算符的子树之间的特殊处理
// 比如（因为2*3 = 6）：
//     -           -
//    / \         / \
//   +   6       /   \
//  / \     =>  +     *
// 3   *       / \   / \
//    / \     3   6 2   3
//   2   3

ExpTree &ExpTree::simplify()
{
    if (root == NULL)
        return *this;
    root = simp1(root);
    simp2(root);
    simp3(root);
    return *this;
}

int ExpTree::compnode(const ExpNode *node1, const ExpNode *node2) const
{
    // compare two nodes, return -1 if node1<node2, 0 if =, 1 if >
    // 这里的 < = > 是从结构上讲的
    if (node1 == NULL && node2 != NULL)
        return -1;
    if (node1 != NULL && node2 == NULL)
        return 1;
    if (node1 == NULL && node2 == NULL)
        return 0;

    if (node1->isLeaf() && !node2->isLeaf())
        return -1;
    if (!node1->isLeaf() && node2->isLeaf())
        return 1;

    if (node1->isLeaf() && node2->isLeaf())
    {
        if (node1->value() < node2->value())
            return -1;
        if (node1->value() > node2->value())
            return 1;
        return 0;
    }

    //if (!node1->isLeaf () && !node2->isLeaf ()) {
    Operator op1 = ((IntlNode *)node1)->Oper();
    Operator op2 = ((IntlNode *)node2)->Oper();
    if (op1.level() < op2.level())
        return -1;
    if (op1.level() > op2.level())
        return 1;
    if (op1.duichen() && !op2.duichen())
        return -1;
    if (!op1.duichen() && op2.duichen())
        return 1;
    if (node1->value() < node2->value())
        return -1;
    if (node1->value() > node2->value())
        return 1;
    int res = compnode(((IntlNode *)node1)->left(), ((IntlNode *)node2)->left());
    if (res != 0)
        return res;
    return compnode(((IntlNode *)node1)->right(), ((IntlNode *)node2)->right());
    //}
}

/////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, const ExpTree &tree)
{
    return tree.print(s) << " = " << tree.getroot()->value(); //.value ();
}

bool operator==(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.value() == tree2.value();
}

bool operator!=(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.value() != tree2.value();
}

bool operator<(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.value() < tree2.value();
}

bool operator>(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.value() > tree2.value();
}

bool operator<=(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.value() <= tree2.value();
}

bool operator>=(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.value() >= tree2.value();
}

//======================================================

ExpTree operator+(const ExpTree &tree1, const ExpTree &tree2)
{
    ExpTree tree = tree1;
    return tree += tree2;
}

ExpTree operator-(const ExpTree &tree1, const ExpTree &tree2)
{
    ExpTree tree = tree1;
    return tree -= tree2;
}

ExpTree operator*(const ExpTree &tree1, const ExpTree &tree2)
{
    ExpTree tree = tree1;
    return tree *= tree2;
}

ExpTree operator/(const ExpTree &tree1, const ExpTree &tree2)
{
    ExpTree tree = tree1;
    return tree /= tree2;
}

//======================================================

int comptree(const ExpTree &tree1, const ExpTree &tree2)
{
    return tree1.comptree(tree2);
}
