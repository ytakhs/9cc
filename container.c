#include "9cc.h"

void expect_test(int line, int expected, int actual) {
    if (expected == actual)
        return;

    fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
    exit(1);
}

void runtest() {
    Vector *vec = new_vector();
    expect_test(__LINE__, 0, vec->len);

    for (intptr_t i = 0; i < 100; i++)
        vec_push(vec, (void *)i);

    expect_test(__LINE__, 100, vec->len);
    expect_test(__LINE__, 0, (intptr_t)vec->data[0]);
    expect_test(__LINE__, 50, (intptr_t)vec->data[50]);
    expect_test(__LINE__, 99, (intptr_t)vec->data[99]);
}

Vector *new_vector() {
    Vector *vec = malloc(sizeof(Vector));

    vec->data = malloc(sizeof(void *) * 16);
    vec->capacity = 16;
    vec->len = 0;

    return vec;
}

void vec_push(Vector *vec, void *elem) {
    if (vec->capacity == vec->len) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }

    vec->data[vec->len++] = elem;
}
