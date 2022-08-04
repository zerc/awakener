#include "../src/server.h"
#include <check.h>
#include <stdlib.h>

struct response handle_request(struct request req)
{
    struct response r;
    return r;
}

START_TEST(test_parse_request)
{
    struct request r;
    memset(&r, 0, sizeof r);

    char buff[] = "GET /foo\nContent-Encoding: gzip\n\n<html><body>Hello</body></html>\0";

    parse_request(buff, &r);

    ck_assert_str_eq(r.method, "GET");
    ck_assert_str_eq(r.path, "/foo");
}
END_TEST

Suite *server_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Server");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_parse_request);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int no_failed = 0;
    Suite *s;
    SRunner *runner;

    s = server_suite();
    runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
