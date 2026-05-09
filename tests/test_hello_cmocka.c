#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "hello.h"

static void test_say_hello_not_null(void **state) {
    (void) state;
    assert_non_null(say_hello());
}

static void test_say_hello_value(void **state) {
    (void) state;
    assert_string_equal(say_hello(), "Hello, World!");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_say_hello_not_null),
        cmocka_unit_test(test_say_hello_value),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
