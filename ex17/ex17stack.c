#include <stdio.h>
#include <stdlib.h>

struct Node {
    struct Node *parent;
    void *data;
};

struct Stack {
    struct Node *top;
};

struct Stack *Stack_new()
{
    struct Stack *stack = malloc(sizeof(struct Stack));
    stack->top = NULL;
    return stack;
}

void Stack_push(struct Stack *stack, void *data)
{
    struct Node *new_top = malloc(sizeof(struct Node));
    new_top->parent = stack->top;
    new_top->data = data;

    stack->top = new_top;
}

void *Stack_pop(struct Stack *stack) 
{
    if(stack->top == NULL) {
        return NULL;
    }

    struct Node *old_top = stack->top;
    stack->top = old_top->parent;
    void *data = old_top->data;
    free(old_top);
    return data;
}

int main(int argc, char const *argv[])
{
    struct Stack *stack = Stack_new();

    Stack_push(stack, "123");
    Stack_push(stack, "234");
    Stack_push(stack, "345");

    printf(": %s\n", Stack_pop(stack));
    printf(": %s\n", Stack_pop(stack));
    printf(": %s\n", Stack_pop(stack));

    Stack_push(stack, "234");
    Stack_push(stack, "345");

    printf(": %s\n", Stack_pop(stack));

    Stack_push(stack, "456");

    printf(": %s\n", Stack_pop(stack));
    printf(": %s\n", Stack_pop(stack));
    printf(": %s\n", Stack_pop(stack));

    return 0;
}