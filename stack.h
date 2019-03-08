#ifndef _STACK_H_
#define _STACK_H_

template <class ELEM>
class LinkNode
{
  private:
    static LinkNode<ELEM> *freelist;

  public:
    ELEM element;   //value of this node
    LinkNode *next; //pointer to next node

    LinkNode(const ELEM &e, LinkNode *nextp = NULL)
    {
        element = e;
        next = nextp;
    }

    LinkNode(LinkNode *nextp = NULL)
    {
        next = nextp;
    }

    ~LinkNode() {}

    void *operator new(size_t);
    void operator delete(void *ptr);
};

template <class ELEM>
class Stack
{
    typedef int (*CMP)(const ELEM &, const ELEM &);

  private:
    LinkNode<ELEM> *head;
    int numnodes;
    void clearhelp();
    LinkNode<ELEM> *find(const ELEM &, CMP) const;

  public:
    Stack()
    {
        head = new LinkNode<ELEM>;
        numnodes = 0;
    }
    ~Stack() { clearhelp(); }

    int size() const { return numnodes; }
    void clear()
    {
        clearhelp();
        numnodes = 0;
    }
    bool push(const ELEM &e, CMP comp)
    {
        LinkNode<ELEM> *pos = find(e, comp);
        if (pos == NULL)
            return false;
        pos->next = new LinkNode<ELEM>(e, pos->next);
        numnodes++;
        return true;
    }
    bool pop(ELEM &e)
    {
        if (head->next == NULL)
            return false;
        LinkNode<ELEM> *temp = head->next;
        e = temp->element;
        head->next = temp->next;
        delete temp;
        numnodes--;
        return true;
    }
    bool getTop(ELEM &e) const
    {
        if (head->next == NULL)
            return false;
        e = head->next->element;
        return true;
    }
    void print() const;
};

/////////////////////////////////////////////////////

template <class ELEM>
LinkNode<ELEM> *LinkNode<ELEM>::freelist = NULL;

template <class ELEM>
void *LinkNode<ELEM>::operator new(size_t)
{
    if (freelist == NULL)
        return ::new LinkNode;
    LinkNode<ELEM> *temp = freelist;
    freelist = freelist->next;
    return temp;
}

template <class ELEM>
void LinkNode<ELEM>::operator delete(void *ptr)
{
    ((LinkNode<ELEM> *)ptr)->next = freelist;
    freelist = (LinkNode<ELEM> *)ptr;
}

////////////////////////////////////////////////////////////

template <class ELEM>
void Stack<ELEM>::clearhelp()
{
    if (head->next == NULL)
        return;
    LinkNode<ELEM> *pos = head->next;
    while (pos != NULL)
    {
        head->next = pos->next;
        delete pos;
        pos = head->next;
    }
}

template <class ELEM>
LinkNode<ELEM> *Stack<ELEM>::find(const ELEM &e, CMP comp) const
{
    LinkNode<ELEM> *pos = head;
    while (pos->next != NULL)
    {
        int res = comp(pos->next->element, e);
        if (res > 0)
            break;
        else if (res == 0)
            return NULL;
        pos = pos->next;
    }
    return pos;
}

template <class ELEM>
void Stack<ELEM>::print() const
{
    if (head->next == NULL)
    {
        cout << "Empty stack." << endl;
        return;
    }
    LinkNode<ELEM> *pos = head->next;
    while (pos != NULL)
    {
        cout << pos->element << endl;
        pos = pos->next;
    }
    cout << endl;
}

#endif
