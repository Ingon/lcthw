#ifndef lcthw_list_h
#define lcthw_list_h

#include <stdlib.h>

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
List *List_copy(List *list);
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void List_push(List *list, void *value);
void List_push_all(List *list, List *src_list);
void *List_pop(List *list);

void List_unshift(List *list, void *value);
void List_unshift_all(List *list, List *src_list);
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);
void List_remove_all(List *list);

List **List_split(List *list, int position);
List *List_sublist(List *list, int from, int to);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#define LIST_EACH(L, V) LIST_FOREACH(L, first, next, V)

#endif
