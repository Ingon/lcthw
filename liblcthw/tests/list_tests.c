#include "minunit.h"
#include <list/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";


char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}


char *test_destroy()
{
    List_clear_destroy(list);

    return NULL;
}


char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");

    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    // we only need to test the middle remove case since push/shift 
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}


char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value on shift.");

    val = List_shift(list);
    mu_assert(val == test1, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}


char *test_remove_all()
{
    mu_assert(List_count(list) == 0, "wrong list size");

    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    mu_assert(List_count(list) == 3, "wrong list size");

    List_remove_all(list);

    mu_assert(List_count(list) == 0, "wrong list size");

    return NULL;
}

char *test_dup()
{
    mu_assert(List_count(list) == 0, "wrong list size");

    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    List *other_list = List_copy(list);

    mu_assert(List_count(list) == List_count(other_list), "wrong list sizes");

    List_remove_all(list);
    List_destroy(other_list);

    return NULL;
}

char *test_split()
{
    mu_assert(List_count(list) == 0, "wrong list size");

    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    List **other_lists = List_split(list, 2);
    mu_assert(List_count(other_lists[0]) == 2, "wrong list sizes");
    mu_assert(List_count(other_lists[1]) == 1, "wrong list sizes");

    List_remove_all(list);
    List_destroy(other_lists[0]);
    List_destroy(other_lists[1]);

    return NULL;
}

char *test_sublist()
{
    mu_assert(List_count(list) == 0, "wrong list size");

    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    List *another_list = List_sublist(list, 0, 2);
    mu_assert(List_count(another_list) == 2, "wrong list sizes");

    List_remove_all(list);
    List_destroy(another_list);

    return NULL;
}

char *all_tests() 
{
    mu_suite_start();

    mu_run_test(test_create);
    
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_remove_all);

    mu_run_test(test_dup);
    mu_run_test(test_split);
    mu_run_test(test_sublist);

    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
