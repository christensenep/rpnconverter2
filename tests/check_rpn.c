#include <check.h>
#include <stdlib.h>
#include "../src/rpn.h"

/* Infix to Prefix Tests */

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

START_TEST(i2p_should_recognize_operator_precedence)
{
    char* desiredPostfixString = "ab-cd^e/f*+";
    char* actualPostfixString = rpn_infix_to_postfix("a-b+c^d/e*f");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_recognize_a_single_parenthetical_expression)
{
    char* desiredPostfixString = "abcd^+e/f*-";
    char* actualPostfixString = rpn_infix_to_postfix("a-(b+c^d)/e*f");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_recognize_multiple_non_nested_parenthetical_groupings)
{
    char* desiredPostfixString = "abcd^+ef*/-";
    char* actualPostfixString = rpn_infix_to_postfix("a-(b+c^d)/(e*f)");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

START_TEST(i2p_should_recognize_nested_parenthetical_groupings)
{
    char* desiredPostfixString = "abc+de/^f*-";
    char* actualPostfixString = rpn_infix_to_postfix("a-((b+c)^(d/e))*f");
    ck_assert_str_eq(actualPostfixString, desiredPostfixString);
    free(actualPostfixString);
}
END_TEST

/* Infix to Prefix Error Tests */

START_TEST(i2p_should_error_on_empty_string)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
    free(actualPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_invalid_character)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("A");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_mismatched_closing_parentheses)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+b)-c");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_mismatched_opening_parentheses)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+(b-c");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_adjacent_operators)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+b+-c");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_adjacent_operands)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+bc+d");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_operator_at_start_of_expression)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("-a+c+d");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_operator_at_end_of_expression)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+c+d-");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_operator_at_start_of_parenthetical_expression)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("e+f*(/a+c+d)");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_operator_at_end_of_parenthetical_expression)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("(a+c+d-)+g");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_operand_preceding_opening_parentheses)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+c(a+d)+g");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

START_TEST(i2p_error_on_operand_following_closing_parentheses)
{
    char* desiredPostfixPointer = NULL;
    char* actualPostfixPointer = rpn_infix_to_postfix("a+(a+d)e+g");
    ck_assert_ptr_eq(actualPostfixPointer, desiredPostfixPointer);
}
END_TEST

/* Postfix to Infix Tests */

START_TEST(p2i_should_convert_single_operand)
{
    char* desiredInfixString = "a";
    char* actualInfixString = rpn_postfix_to_infix("a");
    ck_assert_str_eq(actualInfixString, desiredInfixString);
    free(actualInfixString);
}
END_TEST

START_TEST(i2p_should_handle_provided_examples)
{
    char* desiredPostfixString1 = "abc-+";
    char* desiredPostfixString2 = "ab+c-";
    char* desiredPostfixString3 = "lmn^/o*p-";
    char* desiredPostfixString4 = "lmn^/o*p-";
    char* desiredPostfixString5 = "vw/x^yz-*";

    char* actualPostfixString1 = rpn_infix_to_postfix("a+b-c");
    char* actualPostfixString2 = rpn_infix_to_postfix("(a+b)-c");
    char* actualPostfixString3 = rpn_infix_to_postfix("l/m^n*o-p");
    char* actualPostfixString4 = rpn_infix_to_postfix("((l/(m^n))*o)-p");
    char* actualPostfixString5 = rpn_infix_to_postfix("((v/w)^x)*(y-z)");

    ck_assert_str_eq(actualPostfixString1, desiredPostfixString1);
    ck_assert_str_eq(actualPostfixString2, desiredPostfixString2);
    ck_assert_str_eq(actualPostfixString3, desiredPostfixString3);
    ck_assert_str_eq(actualPostfixString4, desiredPostfixString4);
    ck_assert_str_eq(actualPostfixString5, desiredPostfixString5);

    free(actualPostfixString1);
    free(actualPostfixString2);
    free(actualPostfixString3);
    free(actualPostfixString4);
    free(actualPostfixString5);
}
END_TEST

Suite* rpn_test_suite(void)
{
    Suite* rpnSuite = suite_create("RPN");
    TCase* tcase_infix_to_postfix = tcase_create("Infix To Postfix");
    TCase* tcase_infix_to_postfix_errors = tcase_create("Infix to Postfix Error Handling");
    TCase* tcase_postfix_to_infix = tcase_create("Postfix To Infix");
    TCase* tcase_postfix_to_infix_errors = tcase_create("Postfix to Infix Error Handling");

    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_single_operand);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_single_operator);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_chain_of_equal_precedence_operators);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_convert_very_long_chain_of_equal_precedence_operators);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_recognize_all_operators);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_recognize_operator_precedence);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_recognize_a_single_parenthetical_expression);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_recognize_multiple_non_nested_parenthetical_groupings);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_recognize_nested_parenthetical_groupings);
    tcase_add_test(tcase_infix_to_postfix, i2p_should_handle_provided_examples);
    suite_add_tcase(rpnSuite, tcase_infix_to_postfix);

    tcase_add_test(tcase_infix_to_postfix_errors, i2p_should_error_on_empty_string);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_invalid_character);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_mismatched_closing_parentheses);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_mismatched_opening_parentheses);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_adjacent_operators);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_adjacent_operands);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_operator_at_start_of_expression);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_operator_at_end_of_expression);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_operator_at_start_of_parenthetical_expression);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_operator_at_end_of_parenthetical_expression);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_operand_preceding_opening_parentheses);
    tcase_add_test(tcase_infix_to_postfix_errors, i2p_error_on_operand_following_closing_parentheses);
    suite_add_tcase(rpnSuite, tcase_infix_to_postfix_errors);

    tcase_add_test(tcase_postfix_to_infix, p2i_should_convert_single_operand);
    suite_add_tcase(rpnSuite, tcase_postfix_to_infix);

    suite_add_tcase(rpnSuite, tcase_postfix_to_infix_errors);

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
