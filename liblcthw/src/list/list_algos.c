#include "list_algos.h"
#include <stdbool.h>

inline void ListNode_swap(ListNode *a, ListNode *b)
{
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
    bool swaped = false;
    do {
        swaped = false;

        LIST_EACH(list, cur) {
            if(cur->next) {
                if(cmp(cur->value, cur->next->value) > 0) {
                    ListNode_swap(cur, cur->next);
                    swaped = true;
                }
            }
        }
    } while(swaped);

    return 0;
}

static List *List_merge(List *left, List *right, List_compare cmp)
{
    List *result = List_create();

    void *lfirst;
    void *rfirst;
    while(List_count(left) > 0 || List_count(right) > 0) {
        if(List_count(left) > 0 && List_count(right) > 0) {
            lfirst = List_first(left);
            rfirst = List_first(right);

            if(cmp(lfirst, rfirst) <= 0) {
                List_push(result, List_shift(left));
            } else {
                List_push(result, List_shift(right));
            }
        } else if(List_count(left) > 0) {
            List_push(result, List_shift(left));
        } else if(List_count(right) > 0) {
            List_push(result, List_shift(right));
        }
    }

    return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if(List_count(list) <= 1) {
        return list;
    }

    int mid = List_count(list) / 2;
    List **split = List_split(list, mid);

    List *left = List_merge_sort(split[0], cmp);
    List *right = List_merge_sort(split[1], cmp);

    if(left != split[0]) List_destroy(split[0]);
    if(right != split[1]) List_destroy(split[1]);

    List *result = List_merge(left, right, cmp);

    List_destroy(left);
    List_destroy(right);

    return result;
}
