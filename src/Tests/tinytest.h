/* TinyTest: A really really really tiny and simple no-hassle C unit-testing framework.
 *
 * Features:
 *   - No library dependencies. Not even itself. Just a header file.
 *   - Simple ANSI C. Should work with virtually every C or C++ compiler on
 *     virtually any platform.
 *   - Reports assertion failures, including expressions and line numbers.
 *   - Stops test on first failed assertion.
 *   - ANSI color output for maximum visibility.
 *   - Easy to embed in apps for runtime tests (e.g. environment tests).
 *
 * Example Usage:
 *
 *    #include "tinytest.h"
 *    #include "mylib.h"
 *
 *    void test_sheep()
 *    {
 *      ASSERT("Sheep are cool", are_sheep_cool());
 *      ASSERT_EQUALS(4, sheep.legs);
 *    }
 *
 *    void test_cheese()
 *    {
 *      ASSERT("Cheese is tangy", cheese.tanginess > 0);
 *      ASSERT_STRING_EQUALS("Wensleydale", cheese.name);
 *    }
 *
 *    int main()
 *    {
 *      RUN(test_sheep);
 *      RUN(test_cheese);
 *      return TEST_REPORT();
 *    }
 *
 * To run the tests, compile the tests as a binary and run it.
 *
 * Project home page: http://github.com/joewalnes/tinytest
 *
 * 2010, -Joe Walnes <joe@walnes.com> http://joewalnes.com
 */

#ifndef _TINYTEST_INCLUDED
#define _TINYTEST_INCLUDED

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>
#include <stdlib.h>



/* Main assertion method */
#ifndef ASSERT
#define ASSERT(msg, expression) if (!tt_assert(__FILE__, __LINE__, (msg), (#expression), (expression) ? 1 : 0)) return
#else
#error "The macro \"ASSERT\" is already defined !"
#endif /* ASSERT */

/* Convenient assertion methods */
/* TODO: Generate readable error messages for assert_equals or assert_str_equals */
#ifndef ASSERT_EQUALS
#define ASSERT_EQUALS(expected, actual) ASSERT((#actual), (expected) == (actual))
#else
#error "The macro \"ASSERT_EQUALS\" is already defined !"
#endif /* ASSERT_EQUALS */
#ifndef ASSERT_STRING_EQUALS
#define ASSERT_STRING_EQUALS(expected, actual) ASSERT((#actual), strcmp((expected),(actual)) == 0)
#else
#error "The macro \"ASSERT_STRING_EQUALS\" is already defined !"
#endif /* ASSERT_STRING_EQUALS */

/* Run a test() function */
#ifndef RUN
#define RUN(test_function) tt_execute((#test_function), (test_function))
#else
#error "The macro \"RUN\" is already defined !"
#endif /* RUN */
#ifndef TEST_REPORT
#define TEST_REPORT() tt_report()
#else
#error "The macro \"TEST_REPORT\" is already defined !"
#endif /* TEST_REPORT */

#ifndef TT_COLOR_CODE
#define TT_COLOR_CODE 0x1B
#else
#error "The macro \"TT_COLOR_CODE\" is already defined !"
#endif /* TT_COLOR_CODE */
#ifndef TT_COLOR_RED
#define TT_COLOR_RED "[1;31m"
#else
#error "The macro \"TT_COLOR_RED\" is already defined !"
#endif /* TT_COLOR_RED */
#ifndef TT_COLOR_GREEN
#define TT_COLOR_GREEN "[1;32m"
#else
#error "The macro \"TT_COLOR_GREEN\" is already defined !"
#endif /* TT_COLOR_GREEN */
#ifndef TT_COLOR_RESET
#define TT_COLOR_RESET "[0m"
#else
#error "The macro \"TT_COLOR_RESET\" is already defined !"
#endif /* TT_COLOR_RESET */



extern int tt_passes;
extern int tt_fails;
extern int tt_current_test_failed;
extern const char* tt_current_msg;
extern const char* tt_current_expression;
extern const char* tt_current_file;
extern int tt_current_line;

extern char tt_failed_function_names [32][128];
extern int tt_current_failed_function_name;


extern void tt_execute (const char *name, void (*test_function) (void));

extern int tt_assert (const char *file, int line, const char *msg, const char *expression, int pass);

extern int tt_report (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TINYTEST_INCLUDED */
