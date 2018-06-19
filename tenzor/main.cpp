#include <cassert>

struct node
{
    node * next;
};

struct list
{
    node * first;
};

node * reverse1 (node * head)
{
    node * p    = head;
    node * prev = nullptr;
    node * next = p->next;
    
    while (next) {
        p->next = prev;
        prev = p;
        p = next;
        next = p->next;
    }
    
    p->next = prev;
    return p;
}

node * reverse2 (node * head)
{
    node * parent = nullptr;
    
    while (head != nullptr) {
        node * tmp = head->next;
        head->next = parent;
        parent = head;
        head = tmp;
    }
    
    return parent;
}

// Segmentation fault
node * reverse3 (node * cur, node * parent = nullptr)
{
    if (cur->next == nullptr) {
        cur->next = parent;
        return cur;
    }
    
    node * head = reverse3(cur, cur->next);
    cur->next = parent;
    return head;
}

int main ()
{
    list l;
    node n1;
    node n2;
    node n3;
    node n4;
    
    l.first = & n1;
    n1.next = & n2;
    n2.next = & n3;
    n3.next = & n4;
    n4.next = nullptr;
    
    l.first = reverse1(l.first);
    
    assert(l.first == & n4);
    assert(n4.next == & n3);
    assert(n3.next == & n2);
    assert(n2.next == & n1);
    assert(n1.next == nullptr);
    
    return 0;
}