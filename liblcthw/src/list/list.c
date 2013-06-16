#include "list.h"
#include "dbg.h"

List *List_create()
{
    return calloc(1, sizeof(List));
}

List *List_copy(List *list)
{
    List *result = List_create();
    result->count = list->count;

    ListNode *node = NULL;
    ListNode *last_node = NULL;
    LIST_EACH(list, cur) {
        node = calloc(1, sizeof(ListNode));
        node->value = cur->value;

        if(last_node) {
            last_node->next = node;
            node->prev = last_node;
        } else {
            result->first = node;
        }

        last_node = node;
    }
    result->last = last_node;

    return result;
}

void List_destroy(List *list)
{
    LIST_EACH(list, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List *list)
{
    LIST_EACH(list, cur) {
        free(cur->value);
    }
}

void List_clear_destroy(List *list)
{
    LIST_EACH(list, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
        free(cur->value);
    }

    free(list->last);
    free(list);
}

void List_push(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void List_push_all(List *list, List *src_list)
{
    List *copy = List_copy(src_list);

    if(list->last == NULL) {
        list->first = copy->first;
        list->last = copy->last;
    } else {
        list->last->next = copy->first;
        copy->first->prev = list->last;
        list->last = copy->last;
    }

    list->count += copy->count;
    free(copy);
}

void *List_pop(List *list)
{
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void List_unshift_all(List *list, List *src_list)
{
    List *copy = List_copy(src_list);

    if(list->last == NULL) {
        list->first = copy->first;
        list->last = copy->last;
    } else {
        copy->last->next = list->first;
        list->first->prev = copy->last;
        list->first = copy->first;
    }

    list->count += copy->count;
    free(copy);
}

void *List_shift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node)
{
    void *result = NULL;

    check(list->first && list->last, "List is empty");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    } else if(node == list->last) {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        before->next = after;
        after->prev = before;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}

void List_remove_all(List *list)
{
    LIST_EACH(list, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);

    list->first = NULL;
    list->last = NULL;
    list->count = 0;
}

List **List_split(List *list, int position)
{
    List **result = calloc(2, sizeof(List *));
    result[0] = List_create();
    result[1] = List_create();

    int lpos = 0;
    List *targetPush;
    LIST_EACH(list, cur) {
        targetPush = lpos < position ? result[0] : result[1];
        List_push(targetPush, cur->value);
        lpos++;
    }

    return result;
}

List *List_sublist(List *list, int from, int to)
{
    List *result = List_create();

    int lpos = 0;
    LIST_EACH(list, cur) {
        if(lpos >= from && lpos < to) {
            List_push(result, cur->value);
        } else if(lpos >= to) {
            break;
        }
        lpos++;
    }

    return result;
}
