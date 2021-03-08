/**
 * Assert_Msg.h
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ERROR_HANDLING_ASSERT_MSG_H
#define ERROR_HANDLING_ASSERT_MSG_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <assert.h>



/**
 * Ein Assert mit benutzerdefinierter Fehlermeldung.
 */
#ifndef ASSERT_MSG
    #define ASSERT_MSG(expr, message) assert (((void) (message), expr));
#else
    #error "The macro \"ASSERT_MSG\" is already defined !"
#endif /* ASSERT_MSG */



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ERROR_HANDLING_ASSERT_MSG_H */
