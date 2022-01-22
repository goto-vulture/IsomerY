/**
 * @file CLI_Parameter.c
 *
 * @brief CLI-Parameter des Programms.
 *
 * @date 22.01.2022
 * @author: am1
 */

#include "CLI_Parameter.h"
#include <stddef.h>



const char* const GLOBAL_USAGES [] =
{
     "IsomerY [options] [[--] args]",
     "IsomerY [options]",
     NULL,
};

const char* const GLOBAL_PROGRAM_DESCRIPTION            = "Programmbeschreibung";
const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION = "Weitere Programmbeschreibung";



// Variablen fuer die geparsten CLI-Parameter
int GLOBAL_MAX_C_ATOMS_FOR_TESTS = 0;
