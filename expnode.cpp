#include "expnode.h"

IntlNode *IntlNode::freelist = NULL;

void *IntlNode::operator new(size_t)
{ // Overload new
    if (freelist == NULL)
        return ::new IntlNode; // Create new space
    IntlNode *temp = freelist; // Otherwise, get from freelist
    freelist = (IntlNode *)freelist->l;
    return temp; // Return the link node
}

void IntlNode::operator delete(void *ptr)
{                                    // Overload delete
    ((IntlNode *)ptr)->l = freelist; // Put on freelist
    freelist = (IntlNode *)ptr;
}

//////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, const ExpNode &node)
{
    if (node.isLeaf())
        return s << node.value();
    int level = node.level();
    bool duichen = ((IntlNode &)node).Oper().duichen();
    ExpNode *temp = ((IntlNode &)node).left();

    if (Simp_Exp)
    {
        if (!temp->isLeaf() && temp->level() < level)
            s << "(" << *temp << ")";
        else
            s << *temp;
        s << " " << ((IntlNode &)node).oper() << " ";
        temp = ((IntlNode &)node).right();
        if (!temp->isLeaf() && (temp->level() < level || (temp->level() == level && !duichen)))
            s << "(" << *temp << ")";
        else
            s << *temp;
    }
    else
    {
        if (!temp->isLeaf())
            s << "(" << *temp << ")";
        else
            s << *temp;
        s << " " << ((IntlNode &)node).oper() << " ";
        temp = ((IntlNode &)node).right();
        if (!temp->isLeaf())
            s << "(" << *temp << ")";
        else
            s << *temp;
    }

    return s;
}
