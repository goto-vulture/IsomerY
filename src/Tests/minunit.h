/* minunit.h */
/**
 * Ein ganz simples Testframework.
 *
 * Quelle: http://www.jera.com/techinfo/jtns/jtn002.html
 */

#ifndef TESTS_MINUNIT_H
#define TESTS_MINUNIT_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#ifndef mu_assert
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#else
#error "The macro \"mu_assert\" is already defined !"
#endif /* mu_assert */

#ifndef mu_run_test
#define mu_run_test(test) do { const char* message = test(); tests_run++; if (message != NULL) return message; } while (0)
#else
#error "The macro \"mu_run_test\" is already defined !"
#endif /* mu_run_test */

extern unsigned int tests_run;



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TESTS_MINUNIT_H */
