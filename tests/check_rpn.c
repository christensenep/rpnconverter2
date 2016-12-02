#include <check.h>
#include <stdlib.h>
#include "../src/rpn.h"

START_TEST(i2p_should_convert_empty_string_to_empty_string)
{
    const char* infixString = "";
    char* postfixString = infix_to_postfix(infixString);
    ck_assert_str_eq(postfixString, "");
    free(postfixString);
}
END_TEST

Suite* rpn_test_suite(void)
{
    Suite* rpnSuite = suite_create("RPN");
    TCase* tcase_infix_to_postfix = tcase_create("Infix To Postfix");

    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_empty_string_to_empty_string);
    suite_add_tcase(rpnSuite, tcase_infix_to_postfix);

    return rpnSuite;
}

int main(void)
{
    Suite* rpnSuite = rpn_test_suite();
    SRunner* rpnSuiteRunner = srunner_create(rpnSuite);

    srunner_run_all(rpnSuiteRunner, CK_VERBOSE);
    srunner_free(rpnSuiteRunner);
    return 0;
}
