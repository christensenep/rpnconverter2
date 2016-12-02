#include <check.h>
#include <stdlib.h>
#include "../src/rpn.h"

START_TEST(i2p_should_convert_empty_string_to_empty_string)
{
    char* desiredPostfixString = "";
    char* actualPostfixString = rpn_infix_to_postfix("");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_convert_single_operand)
{
    char* desiredPostfixString = "a";
    char* actualPostfixString = rpn_infix_to_postfix("a");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_convert_single_operator)
{
    char* desiredPostfixString = "ab+";
    char* actualPostfixString = rpn_infix_to_postfix("a+b");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_convert_chain_of_equal_precedence_operators)
{
    char* desiredPostfixString = "ab+c+";
    char* actualPostfixString = rpn_infix_to_postfix("a+b+c");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_convert_very_long_chain_of_equal_precedence_operators)
{
    char* desiredPostfixString = "ab+c+d+e+f+g+h+i+j+k+m+n+o+p+q+r+s+t+u+v+w+x+y+z+";
    char* actualPostfixString = rpn_infix_to_postfix("a+b+c+d+e+f+g+h+i+j+k+m+n+o+p+q+r+s+t+u+v+w+x+y+z");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_recognize_all_operators)
{
    char* desiredPostfixString = "abcdef^/*-+";
    char* actualPostfixString = rpn_infix_to_postfix("a+b-c*d/e^f");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

Suite* rpn_test_suite(void)
{
    Suite* rpnSuite = suite_create("RPN");
    TCase* tcase_infix_to_postfix = tcase_create("Infix To Postfix");

    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_empty_string_to_empty_string);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_single_operand);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_single_operator);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_chain_of_equal_precedence_operators);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_very_long_chain_of_equal_precedence_operators);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_recognize_all_operators);
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
