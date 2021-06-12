#ifndef __LIST_INTERNAL_H__
#define __LIST_INTERNAL_H__

typedef struct Node Node;


struct Node
{
void *m_data;
Node* m_next;
Node* m_prev;
int magicNumber;
};

struct List
{
Node m_head;
Node m_tail;
int magicNumber;
};

# endif  /* __LIST_INTERNAL_H__ */