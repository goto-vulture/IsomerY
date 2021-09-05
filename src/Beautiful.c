/**
 * Beautiful.c
 *
 * Alle liebevollen Voegel wurden hier gefunden: https://textart.sh/topic/vulture
 *
 *  Created on: 04.09.2021
 *      Author: am1
 */


#include "Beautiful.h"
#include <stdio.h>



//---------------------------------------------------------------------------------------------------------------------

/**
 * Show the first beautiful bird.
 *
 * Ein wunderschoener Gaensegeier ! Hoffentlich kann ich bald einen besuchen !
 */
void Beautiful_1 (void)
{
    puts("");
    puts("                                  ░░░░░░                                                                        ");
    puts("                              ░░░░░░░░░░░░░░░░                                                                  ");
    puts("                          ░░░░░░░░░░░░░░░░░░░░░░░░                                                              ");
    puts("                ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░                                                            ");
    puts("        ░░░░▒▒▓▓▒▒▒▒░░▒▒░░▒▒▒▒▒▒▓▓▓▓░░░░░░░░░░░░░░░░                                                            ");
    puts("    ░░░░  ░░░░░░▓▓▒▒░░▒▒▒▒▒▒▒▒░░▓▓▓▓░░▒▒░░░░░░░░░░░░░░                                                          ");
    puts("  ░░    ░░░░░░░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒░░░░░░░░░░░░░░░░                                                        ");
    puts("        ░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░░░░░░░░░░░░░░░                                                        ");
    puts("    ░░░░░░░░▒▒▒▒▒▒░░▒▒▒▒░░▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░▒▒░░░░░░                                                      ");
    puts("  ░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒░░░░▒▒░░▒▒░░▒▒░░░░░░░░░░░░░░░░░░░░░░                                                      ");
    puts("  ▒▒░░░░░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░▒▒▒▒▒▒░░░░                                                ");
    puts("  ░░                    ░░▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒██▓▓▓▓▓▓▓▓                                            ");
    puts("                              ░░▒▒▒▒░░░░▒▒▒▒▒▒░░▒▒░░▒▒▒▒░░██████████▓▓▒▒░░        ░░▒▒▓▓▓▓                      ");
    puts("                  ▓▓██▓▓██▒▒    ▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒░░▒▒▒▒████████▓▓▓▓██▓▓▒▒    ░░▓▓▓▓▓▓▓▓▓▓▒▒                  ");
    puts("              ░░████████▓▓▓▓██▒▒████▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒██████████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒                ");
    puts("            ░░▓▓██████████▓▓████████░░░░░░▒▒░░░░▒▒░░░░▒▒▓▓████▓▓██████████████▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓░░              ");
    puts("            ▓▓▓▓████████▓▓▓▓▓▓██▓▓▓▓░░░░░░░░░░▒▒▒▒░░▒▒▒▒▓▓▒▒░░▓▓▓▓▓▓██████████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓              ");
    puts("          ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▓▓░░  ░░▒▒▓▓▓▓██▓▓██████▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓░░            ");
    puts("        ░░▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░░░  ░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░    ░░▒▒██████▓▓██████▓▓▓▓▓▓████▓▓▓▓▓▓▓▓▓▓██            ");
    puts("        ▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░    ░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒      ░░░░▓▓████████████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░          ");
    puts("        ▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░    ░░░░░░░░░░░░░░░░▒▒▒▒░░      ░░░░▒▒██████▓▓████▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓          ");
    puts("        ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░    ░░░░░░░░░░░░░░░░▒▒▒▒            ░░██████████▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓          ");
    puts("      ░░▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓░░      ░░░░░░░░░░░░▒▒▒▒▒▒░░          ░░▓▓████████▓▓██▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▒▒▒▒▓▓░░        ");
    puts("      ░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░    ░░░░░░░░░░░░▒▒▒▒░░              ▒▒▓▓▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒        ");
    puts("      ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░  ░░░░░░░░░░░░░░▒▒▒▒░░          ░░░░▓▓████████▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒        ");
    puts("      ▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░  ░░    ░░░░░░░░░░        ░░░░░░░░░░▓▓████▓▓██▓▓▓▓██▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓██▒▒▓▓        ");
    puts("      ▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒  ░░░░    ░░░░░░░░    ░░░░  ░░░░▒▒▒▒▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓██▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░      ");
    puts("      ▒▒▒▒▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓░░░░    ░░░░░░░░░░░░  ░░░░░░░░░░▒▒▓▓▓▓▓▓▓▓░░▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒      ");
    puts("    ░░░░░░▒▒▒▒▓▓░░▓▓▓▓▓▓▓▓██▒▒░░░░░░░░░░░░░░░░░░▒▒▒▒░░░░░░░░░░▓▓▓▓▒▒▒▒▒▒░░▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒▓▓▓▓▓▓▓▓▓▓      ");
    puts("      ▓▓░░▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓░░░░░░▒▒▒▒░░▒▒▓▓▓▓▓▓▓▓░░  ░░░░░░░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓████▒▒░░▒▒▓▓      ");
    puts("      ▓▓██▓▓▓▓░░██▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓░░░░░░░░░░▒▒▒▒░░▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓████▓▓▓▓░░      ");
    puts("      ▓▓██▓▓▓▓██▓▓▓▓██▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░▒▒▒▒▓▓▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓██▒▒      ");
    puts("      ████▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒      ");
    puts("      ▓▓▓▓██▓▓██▓▓░░▓▓▓▓▓▓▓▓░░▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓████▓▓▓▓▒▒▓▓▓▓      ");
    puts("      ▓▓▓▓██▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓░░░░░░▓▓▓▓▓▓▓▓▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▒▒▓▓▒▒▓▓██      ");
    puts("      ██▓▓██▓▓▓▓░░▓▓▓▓▓▓▓▓██▒▒▒▒▓▓▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓██▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒██      ");
    puts("      ██▓▓██▒▒██▓▓▓▓▓▓▓▓▓▓██▒▒░░▓▓▓▓░░  ░░░░░░  ░░▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓      ");
    puts("      ▒▒▓▓██▒▒██▓▓▓▓▓▓▓▓▓▓██▒▒▒▒▒▒▓▓▒▒░░░░░░    ░░▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓██████▓▓██▓▓▒▒▓▓      ");
    puts("      ░░▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓██▒▒▓▓▒▒▒▒▓▓░░▒▒░░  ░░░░░░▒▒▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓██▓▓▓▓▓▓████▓▓██▓▓██▓▓▒▒░░      ");
    puts("        ▒▒▓▓██▓▓▓▓▓▓▓▓██▓▓██▒▒▒▒▒▒▓▓▒▒▒▒▓▓▒▒░░░░░░▒▒▒▒▓▓▓▓▒▒▓▓░░▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓██▓▓▓▓▓▓████▓▓██▓▓██▓▓▒▒        ");
    puts("        ░░▓▓████▓▓▓▓▓▓▓▓▓▓██▒▒▒▒▓▓▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓░░▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓██▓▓██▓▓▓▓▓▓████▓▓██▓▓██▓▓░░        ");
    puts("          ▓▓████▓▓██▓▓▓▓▓▓▓▓▒▒░░▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓░░░░▒▒▓▓▓▓██▓▓▓▓██▓▓██████▓▓██▓▓▒▒        ");
    puts("          ▒▒████▓▓████▓▓▓▓▓▓▒▒░░▓▓░░▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒░░▒▒▒▒░░▒▒▒▒▒▒▓▓▒▒▒▒▓▓██▓▓██▓▓▓▓██████▓▓██▓▓░░        ");
    puts("          ▒▒████▓▓▓▓██████▒▒▓▓░░▓▓░░▓▓▓▓▓▓▒▒▒▒▓▓▓▓██▒▒▒▒▒▒▓▓▓▓░░░░▒▒▒▒▓▓▓▓▒▒░░▒▒██▓▓██▓▓████▓▓████████          ");
    puts("            ▓▓██▓▓▓▓██████▓▓▒▒▒▒░░▒▒▒▒▓▓██▓▓▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▒▒▓▓░░▒▒██▓▓▓▓▓▓██▓▓██▓▓██▓▓▓▓          ");
    puts("            ██▒▒██▓▓██████▓▓░░░░░░░░░░▒▒██▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒░░██▓▓▓▓████▓▓██▒▒██▓▓▒▒          ");
    puts("            ▓▓▓▓██▓▓██▓▓██▒▒▒▒▒▒▒▒▓▓░░░░██▓▓▓▓░░▓▓▓▓▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒░░▓▓▓▓▒▒▓▓▓▓██▓▓██▓▓▓▓██▓▓▓▓▓▓▒▒          ");
    puts("            ▒▒██▒▒▓▓▓▓▓▓██▓▓░░  ░░░░▒▒▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒░░░░▒▒▒▒██▓▓▓▓████████▓▓▓▓░░          ");
    puts("            ░░▓▓██████▓▓██▒▒▓▓░░▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓░░▒▒▒▒▒▒▒▒░░▒▒▒▒▓▓▓▓░░░░░░░░▓▓██▓▓▓▓██▓▓▓▓████▒▒            ");
    puts("              ██▓▓████▓▓██▒▒▒▒░░░░▒▒▓▓▓▓▓▓▓▓▒▒▒▒░░██▓▓▓▓▒▒░░░░░░░░▓▓▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓████▓▓██▓▓▓▓▒▒░░          ");
    puts("              ▓▓▓▓██▓▓▒▒▓▓▒▒▒▒░░▒▒▒▒▓▓▓▓▓▓▓▓▒▒░░▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒░░░░░░░░▓▓▓▓██████▓▓████▓▓▒▒            ");
    puts("              ▓▓▓▓▓▓██▓▓▓▓░░░░░░░░░░██▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒░░░░▒▒▒▒▓▓▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒██▓▓▓▓▓▓████▒▒▒▒            ");
    puts("                ██▓▓██▓▓▓▓▒▒░░░░░░▓▓▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒░░░░░░██████▓▓▓▓▓▓▓▓▓▓▒▒            ");
    puts("                ░░▓▓██▓▓▓▓▓▓  ▓▓▒▒▒▒░░▓▓▒▒▓▓▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒░░▒▒▓▓░░▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓            ");
    puts("                  ▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒░░▓▓▓▓▒▒▓▓▓▓░░▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒░░▒▒▓▓▓▓██▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓██▓▓██▓▓▒▒            ");
    puts("                  ██▓▓▓▓▒▒▒▒▓▓▓▓▒▒░░▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒▒▒░░▒▒▓▓▒▒▒▒▒▒▓▓▓▓▒▒▒▒░░▓▓▓▓▓▓▓▓▓▓██████▓▓░░            ");
    puts("                  ▒▒▓▓▓▓▓▓▓▓░░▒▒░░▒▒░░▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▒▒░░▒▒▒▒▒▒▓▓▒▒▓▓▒▒██▓▓▓▓██████▓▓              ");
    puts("                  ▒▒▓▓▓▓▒▒▓▓░░░░░░░░▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▓▓▒▒▒▒██▒▒██▓▓▓▓▓▓████▓▓              ");
    puts("                  ░░▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓░░░░▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓████▓▓██████▓▓              ");
    puts("                    ▒▒▓▓▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒██▒▒░░▒▒▓▓▓▓▒▒▒▒▒▒▒▒██▓▓████▓▓██████▒▒              ");
    puts("                    ▒▒▓▓▓▓▒▒▓▓▓▓▓▓▒▒░░░░▒▒▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒████▓▓████████████░░              ");
    puts("                    ▒▒▓▓▓▓▓▓▓▓▒▒▓▓░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▓▓▓▓▓▓▒▒▒▒▓▓▒▒██▓▓▓▓▓▓▓▓██▓▓██████                ");
    puts("                      ▒▒▓▓▒▒▒▒▓▓▓▓░░░░▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒▓▓▓▓██▓▓██████████████                ");
    puts("                      ▒▒▒▒▓▓▒▒▒▒▒▒░░░░░░▒▒▒▒▓▓▓▓░░░░▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓████████▓▓                ");
    puts("                      ▒▒▓▓▒▒▓▓▓▓▓▓░░░░░░░░▒▒▓▓▒▒▓▓▓▓██▓▓▒▒▓▓▓▓▒▒░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓████████▒▒                ");
    puts("                      ░░▓▓▓▓▒▒▒▒▒▒░░░░░░▒▒░░░░▓▓▓▓▓▓████▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓██████████                  ");
    puts("                      ░░▒▒▒▒▓▓▓▓▒▒░░  ░░░░▓▓▓▓▓▓▓▓▒▒██▓▓▒▒▒▒▓▓▒▒░░░░▒▒▒▒░░▒▒▓▓▒▒▓▓▓▓██████████                  ");
    puts("                      ░░▒▒▓▓▓▓▒▒▒▒░░░░  ░░▓▓▒▒▓▓▓▓░░▒▒▒▒▒▒▓▓▒▒▓▓▓▓▒▒░░░░░░▒▒▓▓▓▓▒▒▒▒██████████                  ");
    puts("                      ▒▒▓▓▒▒░░▒▒▒▒░░░░░░░░▒▒▒▒░░░░▒▒▓▓▓▓▓▓░░░░▒▒▒▒░░░░░░░░▒▒▒▒▒▒▓▓▓▓████████▓▓                  ");
    puts("                      ▒▒▓▓▒▒▓▓▒▒░░░░░░  ░░▒▒▓▓░░▒▒▓▓▓▓▒▒▓▓██▒▒▒▒░░      ░░▓▓▓▓▓▓▓▓▓▓████████░░                  ");
    puts("                      ▒▒▓▓▓▓▒▒▒▒░░░░░░░░░░░░██▓▓▓▓▒▒▓▓▒▒▒▒░░▒▒██        ░░▓▓▓▓▓▓▓▓▓▓████████                    ");
    puts("                      ▓▓▓▓▓▓▒▒▒▒░░░░░░░░  ░░▓▓████▓▓▒▒▓▓██▓▓▓▓▓▓░░    ░░░░▓▓▓▓▓▓▓▓██████████                    ");
    puts("                      ▒▒▓▓▒▒░░▒▒░░░░░░░░░░▓▓▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░░░  ░░▒▒▓▓▓▓▓▓▓▓████████▒▒                    ");
    puts("                      ▒▒▒▒▒▒  ▒▒░░░░░░░░▒▒██▓▓▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▒▒░░░░░░░░▓▓▓▓▓▓▓▓████████                      ");
    puts("                          ▒▒░░  ▓▓░░░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▒▒░░░░░░▒▒▒▒▓▓▓▓▒▒████████                      ");
    puts("                          ▒▒      ░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▒▒▒▒▒▒░░░░▓▓▒▒▒▒▒▒▓▓████████░░                      ");
    puts("                          ▒▒        ░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓░░░░▓▓▓▓██▓▓▓▓██████▓▓                        ");
    puts("                            ░░      ░░▒▒▓▓▒▒▓▓██▓▓▓▓▓▓▒▒████▓▓▓▓▓▓░░░░▓▓▓▓▓▓▓▓████████░░                        ");
    puts("                            ░░    ░░░░▒▒▓▓▒▒▓▓▓▓▓▓██▒▒████▓▓██▒▒▓▓░░▒▒██████████████▒▒                          ");
    puts("                                  ░░░░░░▓▓▓▓▒▒▓▓██████████▓▓████▒▒▒▒▒▒████████▓▓░░                              ");
    puts("                                  ░░░░░░▓▓██████████▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒▒▒████████                                  ");
    puts("                                  ░░░░▒▒▓▓██████████████▓▓▒▒▒▒██▒▒▒▒▒▒██████░░                                  ");
    puts("                                  ░░░░░░░░██████████████████████░░░░░░████                                      ");
    puts("                                  ░░░░▒▒  ████████████████████▓▓░░░░░░▓▓██                                      ");
    puts("                                ░░▒▒▒▒▒▒░░▒▒██▓▓████████████▓▓▒▒▒▒▒▒▒▒▒▒░░                                      ");
    puts("                              ░░▒▒▒▒▒▒▒▒▒▒░░▒▒▓▓░░░░░░▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░                                  ");
    puts("                            ░░░░░░▒▒▒▒░░▒▒▓▓▓▓░░░░░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░░░░░░░░░░░                      ");
    puts("                ░░░░░░░░░░▒▒░░▒▒▒▒▓▓▒▒░░▒▒▒▒▓▓▓▓▒▒░░▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░                ");
    puts("            ░░░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▓▓▒▒▓▓▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░                ");
    puts("              ░░▒▒▓▓▓▓▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒░░░░▒▒▒▒▒▒░░▒▒░░▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░░░░░░░                    ");
    puts("                            ▒▒▒▒▓▓░░              ░░░░                  ▒▒▒▒░░▒▒                                ");
    puts("                            ▒▒▒▒░░                                          ░░░░                                ");
    puts("                            ░░░░                                              ░░                                ");
    puts("");

    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Show the second beautiful bird.
 *
 * Ein faszinierender Ohrengeier ! Wie sehr wuerde ich mich freuen, wenn er mich als Freund oder zumindest als Bekannter
 * sehen wuerde. Und gleichzeitig mich in seiner Naehe lassen wuerde.
 */
void Beautiful_2 (void)
{
    puts("");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒        ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒    ░░░░      ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▓▓▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░▒▒░░░░▒▒░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒░░▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓░░░░░░▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓██░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓██▓▓░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▓▓▓▓▓▓██░░░░▒▒▒▒▒▒░░▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓██▓▓▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▓▓██▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒██▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░░░▒▒░░░░▒▒▓▓██▓▓▒▒▒▒░░▓▓██▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒▓▓░░▒▒▒▒▓▓▒▒▓▓▓▓▓▓▒▒▒▒░░▓▓██▓▓▓▓▒▒▒▒▒▒▓▓██▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▓▓▓▓▓▓██▓▓▒▒▒▒░░▒▒▒▒▓▓▓▓░░░░▒▒████▓▓▓▓▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒░░▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▓▓▓▓████▓▓██▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▒▒▒▒░░████▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒██████▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▒▒▓▓▒▒▓▓▒▒▒▒▓▓██▒▒▓▓████▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▓▓▓▓▒▒██▓▓░░▓▓████▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓░░▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒░░▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▓▓░░▒▒░░░░░░░░▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒");
    puts("▒▒▒▒▒▒░░░░░░░░▒▒▒▒░░██▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▓▓▒▒░░░░░░░░▒▒██▓▓▒▒██▓▓▓▓▓▓▓▓▓▓▒▒");
    puts("▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▒▒░░░░░░░░▒▒▓▓▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒░░░░░░░░░░░░▒▒░░▒▒░░▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓░░░░░░▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("░░░░░░░░░░░░░░░░▒▒██░░▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓██▓▓▒▒░░▓▓▒▒▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("░░░░░░░░░░░░░░▒▒▓▓██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓▓▓▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▒▒");
    puts("░░░░░░░░░░░░▒▒▓▓██▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓████▓▓▓▓▓▓▓▓▓▓░░");
    puts("░░░░░░░░░░▒▒▓▓▓▓██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒▓▓▒▒▒▒▓▓████▓▓▓▓▓▓▓▓▓▓░░");
    puts("░░░░░░░░░░▓▓▓▓████░░▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒██▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒██▓▓██▓▓▓▓▓▓████░░");
    puts("░░░░░░░░▓▓▓▓▓▓▒▒▒▒░░▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓██▓▓▓▓██▓▓▓▓██░░░░");
    puts("░░░░░░░░██▓▓▒▒░░░░▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓██▓▓██▓▓▓▓▓▓██░░░░");
    puts("░░░░░░░░▓▓██▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓██▓▓▓▓▓▓██▓▓░░░░░░");
    puts("░░░░░░░░▒▒▓▓▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓████▓▓▓▓▓▓▓▓▓▓░░░░░░");
    puts("░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▓▓████▓▓▓▓▓▓██▒▒░░░░░░");
    puts("░░░░░░░░░░░░░░░░▒▒▓▓▒▒▓▓▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓██████▓▓██▓▓░░░░░░░░");
    puts("░░░░░░░░░░░░░░░░▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓██▓▓██░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░░░▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓██▓▓░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░▒▒░░██▒▒▓▓▒▒▒▒▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓██▓▓░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▒▒▓▓▓▓██▓▓██░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░▒▒░░▓▓▒▒▓▓▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓██▓▓░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░▒▒▒▒▓▓▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒░░▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓██▒▒░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░▒▒▒▒▓▓▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒▓▓▓▓▓▓▒▒██▓▓▓▓▓▓▒▒▓▓▒▒▓▓██▒▒▓▓░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▒▒▓▓▒▒▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░██▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▓▓░░▒▒▒▒▓▓▓▓▓▓██▓▓▓▓▓▓▒▒▒▒▒▒▓▓██▒▒▓▓  ░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░▒▒▓▓██▒▒▓▓▒▒▒▒▒▒▓▓▒▒██▓▓▓▓▓▓▓▓▓▓▒▒░░░░▓▓░░▓▓░░▓▓████▓▓▓▓▓▓▒▒░░░░▒▒▒▒▓▓░░░░  ░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░██▓▓██▒▒▓▓▒▒▒▒▒▒▓▓▒▒██▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░▒▒░░░░▓▓██▓▓▓▓▓▓▓▓░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░████▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░▒▒░░░░▒▒████▓▓▓▓░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░▒▒▒▒████▓▓▒▒██▒▒▒▒▒▒▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▒▒▒▒▒▒░░▒▒████▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░    ▓▓▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓██▓▓██▓▓▓▓▓▓▒▒░░░░░░▒▒░░░░▒▒██▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░  ▓▓▓▓▒▒▒▒▓▓▒▒▓▓▓▓▓▓██▓▓██▓▓▓▓▓▓▒▒░░░░░░▒▒░░░░▒▒██▓▓▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░  ██▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓██████▓▓▓▓▒▒▒▒░░░░░░░░░░░░▒▒██▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓██▓▓████▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▒▒▓▓██▓▓▓▓▓▓▓▓██▓▓████▓▓▓▓░░░░░░░░░░░░░░░░▒▒▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░▒▒██▓▓▓▓▓▓▓▓▓▓██▓▓▓▓████░░░░░░░░░░░░░░░░▒▒░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██████░░░░░░░░░░░░░░░░▒▒░░░░░░░░▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓  ░░░░░░░░░░░░░░░░░░      ░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░      ");
    puts("░░░░░░░░░░░░▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓██░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░  ░░░░░░░░▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓██▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░        ░░░░░░░░░░░░░░░░░░░░");
    puts("  ░░    ░░░░▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▒▒░░  ░░░░  ░░░░░░░░░░░░░░░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▒▒▓▓██▓▓▓▓▓▓██▒▒  ░░    ░░░░░░░░  ░░  ░░░░░░  ░░░░  ░░░░░░      ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░▓▓▓▓▓▓▓▓░░▒▒▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░▓▓▒▒  ░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒░░▒▒░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░");
    puts("");

    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Show the third beautiful bird.
 *
 * Klein aber suess. Ein liebenswerter Schmutzgeier. Wirf die Eier mit Steinen kaputt; ich werde dir dabei helfen ! :)
 */
void Beautiful_3 (void)
{
    puts("");
    puts("██████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████");
    puts("██████████████████████████████████████████████████▓▓██████████████████████████████████████████████████████████████████████████");
    puts("██████████████████████████████████████████████▓▓▓▓▓▓▓▓████████████████████████████████████████████████████████████████████████");
    puts("██████████████████████████████████████████████▒▒▒▒░░▓▓░░▒▒██░░████████████████████████████████████████████████████████████████");
    puts("██████████████████████████████████████░░▒▒██████▒▒░░  ▒▒░░░░▒▒  ▒▒▓▓▒▒████████████████████████████████████████████████████████");
    puts("████████████████████████████████████▓▓▒▒░░░░░░░░▒▒░░░░                ░░░░████████████████████████████████████████████████████");
    puts("████████████████████████████▓▓▓▓▒▒▒▒▓▓████▓▓▒▒░░░░                          ░░▒▒██████████████████████████████████████████████");
    puts("████████████████████████████████▓▓▒▒▒▒░░░░  ░░░░                                  ░░▓▓████████████████████████████████████████");
    puts("████████████████████████████████████████▓▓▒▒░░                        ░░░░▒▒░░  ░░  ░░░░▒▒████████████████████████████████████");
    puts("████████████████████████████████▓▓▒▒░░            ░░░░              ░░░░▒▒▓▓░░▒▒░░░░░░░░░░        ░░▒▒████████████████████████");
    puts("██████████████████████████████▒▒▒▒██▒▒░░░░                        ░░░░▒▒▒▒▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ░░  ▒▒██████");
    puts("██████████████████████████████▓▓░░              ░░░░░░░░░░      ░░░░░░▒▒▓▓██▓▓▓▓░░▒▒▒▒▒▒▒▒░░░░░░▒▒████▓▓▒▒░░░░░░░░▒▒▓▓▒▒░░▒▒██");
    puts("██████████████████████████▒▒▓▓▒▒░░░░░░░░      ░░░░    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▓▓▓▓▓▓▒▒░░▒▒");
    puts("██████████████████████████████████▒▒        ░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓░░░░");
    puts("████████████████████████████▓▓░░        ░░░░░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░▒▒░░░░░░░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▒▒");
    puts("████████████████████████▓▓██▒▒░░░░    ░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒░░░░▒▒░░▒▒▒▒░░░░░░▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓██████████████▓▓");
    puts("██████████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░░░▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓████▓▓████████████████████");
    puts("████████████████████████▒▒▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒░░░░░░░░▒▒░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██████████████████████████");
    puts("██████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░▒▒░░  ░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██████████████████████████");
    puts("██████████████████████████▒▒░░  ░░░░░░░░░░░░░░░░▒▒░░░░░░  ░░░░░░░░░░░░░░▒▒░░▒▒▒▒▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓████████████████████████████");
    puts("████████████████████████▒▒░░▒▒  ░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░  ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██████████████████████████████");
    puts("██████████████████████▓▓▓▓░░░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓████████████████████████████████");
    puts("██████████████████████▓▓▓▓██░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓████████████████████████████████");
    puts("██████████████████████████░░░░░░░░░░░░▒▒▒▒░░░░▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓██████████████████████████████████");
    puts("████████████████████████▒▒▒▒░░░░░░░░░░▒▒▒▒░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓██████████████████████████████████");
    puts("██████████████████████████░░░░░░░░░░▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓██████████████████████████████████");
    puts("██████████████████████▓▓██░░░░░░░░▒▒▒▒▒▒░░▒▒░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▓▓▒▒▒▒▓▓████████████████████████████████████");
    puts("████████████████████████▒▒░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ░░▒▒▒▒▒▒░░░░▒▒████████████████████████████████████");
    puts("██████████████████████▓▓░░░░░░░░▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒░░▒▒░░░░░░░░░░░░░░    ░░░░░░▒▒░░░░░░▒▒██████████████████████████████████");
    puts("████████████████████▓▓░░░░░░░░▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒░░▒▒░░▒▒░░░░░░░░░░  ░░░░    ░░░░░░░░░░░░░░▒▒▓▓██████████████████████████████");
    puts("██████████████████░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░▒▒▒▒░░░░░░░░░░░░░░░░  ░░░░░░░░░░  ░░░░░░░░░░░░░░▓▓██████████████████████████████");
    puts("████████████████▒▒░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░▒▒░░▒▒▒▒░░░░░░░░░░░░░░░░  ░░  ░░  ░░    ░░░░░░░░░░░░░░▓▓████████████████████████████");
    puts("████████████▒▒░░░░░░  ░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░▒▒▒▒░░  ░░  ░░    ░░░░░░░░░░░░░░░░░░▒▒████████████████████████████");
    puts("██████████▓▓░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒░░▒▒░░░░░░░░▒▒▒▒░░  ░░░░░░░░  ░░░░  ░░░░░░░░░░░░░░▒▒██████████████████████████");
    puts("████████▓▓░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░░░▒▒▒▒░░▒▒░░▒▒░░▒▒▒▒░░░░░░░░░░    ░░  ░░░░░░░░░░░░░░░░░░░░██████████████████████████");
    puts("██████▓▓░░░░░░░░░░░░▒▒▒▒░░░░░░▒▒░░░░▒▒░░▒▒▒▒▒▒▒▒░░▒▒░░▒▒▒▒▒▒▒▒░░░░░░░░░░    ░░░░░░░░░░░░░░░░░░░░░░░░░░████████████████████████");
    puts("████▒▒░░░░░░░░░░░░▒▒░░░░░░░░░░▒▒░░░░░░░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░  ░░░░░░▒▒░░░░░░░░░░░░░░░░▒▒██████████████████████");
    puts("██░░░░░░░░░░▒▒░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░▒▒░░▒▒██████████████████████");
    puts("▒▒▒▒░░░░░░░░░░░░░░░░▒▒░░░░▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░██▒▒██████████████████████");
    puts("▒▒▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░░░░░░░░░░░░░▒▒▒▒░░░░░░░░░░░░░░▒▒░░▒▒░░░░▒▒████████████████████████");
    puts("░░░░░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░░░░░░░▒▒░░░░▒▒▒▒░░░░░░░░░░░░░░▒▒░░░░▒▒░░░░████████████████████████");
    puts("░░░░░░░░░░░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒░░▒▒░░▒▒▒▒░░░░░░░░░░░░▒▒▒▒░░▒▒▒▒▒▒▒▒██████████████████████");
    puts("░░░░░░░░░░░░░░░░░░▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒░░░░▒▒▒▒░░░░░░▒▒░░▒▒▒▒▒▒▒▒▒▒░░████████████████████████");
    puts("░░░░░░░░░░░░▒▒░░░░░░▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒████████████████████████");
    puts("░░░░▒▒░░░░▒▒▒▒░░░░░░▒▒▒▒▒▒▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒░░▒▒▒▒░░▒▒▒▒▒▒▒▒▓▓██████████████████████");
    puts("░░░░░░▒▒░░░░░░░░░░▒▒░░░░▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒░░▒▒▒▒▒▒██▒▒██████████████████████");
    puts("");

    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Show the fourth beautiful vulture.
 */
void Beautiful_4 (void)
{
    puts("");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░          ░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░    ░░░░░░░░░░░░  ░░░░        ░░▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░              ░░░░░░    ░░  ░░▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░  ░░    ░░░░░░              ░░░░░░        ░░▒▒▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░  ░░░░                              ░░▒▒▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░            ░░░░    ░░    ░░░░░░░░░░▓▓▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒░░░░░░░░░░    ░░    ░░░░░░  ░░░░░░░░            ░░░░▓▓▒▒▒▒▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒  ░░░░░░░░  ░░    ░░░░    ░░░░  ░░░░            ░░░░▒▒▓▓▓▓▒▒░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░  ░░░░░░░░░░    ░░      ░░░░        ░░░░        ░░▒▒▒▒░░░░▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░  ░░░░░░░░░░░░          ░░    ░░  ░░▒▒░░░░  ░░░░░░▒▒  ░░░░▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░            ░░░░░░░░▒▒░░    ░░░░░░░░░░░░░░▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░                          ░░░░  ▒▒░░░░░░  ░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░                    ░░░░░░░░░░░░░░░░░░░░░░    ");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░                        ░░▒▒░░░░░░░░░░  ░░    ");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░              ░░░░▒▒░░░░░░░░      ");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ░░░░░░░░░░▒▒▒▒░░░░░░░░░░░░░░  ");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒░░░░░░░░▒▒  ░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓▒▒▒▒░░▒▒▒▒░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░▒▒░░▒▒░░░░░░▒▒▒▒▒▒▒▒░░▒▒▒▒░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░▒▒░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░▒▒▒▒▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░            ▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▓▓▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░          ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░  ░░░░░░░░░░░░░░      ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒░░░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    ░░▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓░░░░░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░    ░░▓▓▓▓▓▓▒▒▒▒▒▒▒▒░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▒▒▒▒░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓████▓▓▓▓▓▓▒▒▒▒░░▒▒░░      ░░▓▓▓▓▓▓▒▒▒▒▒▒░░▒▒░░░░░░░░░░▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░");
    puts("▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓██████▓▓▒▒▒▒▓▓▒▒▒▒▒▒░░░░      ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒░░░░░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░░░░░");
    puts("▒▒▒▒▒▒▓▓  ░░▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░        ▒▒▓▓▓▓▒▒▓▓▒▒▓▓░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░░░");
    puts("▒▒▒▒▓▓▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░░░░░        ▒▒██▓▓▓▓▒▒██▓▓░░▒▒▒▒░░░░░░░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░");
    puts("▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░        ██████▓▓▓▓██▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒░░");
    puts("▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░      ▒▒▓▓▓▓██▓▓████▓▓▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒");
    puts("▒▒██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░    ░░▓▓▓▓▓▓██████▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒▒▒░░");
    puts("▓▓██▓▓▓▓▓▓████▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░      ░░░░▓▓██████████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒██▓▓▒▒▒▒▒▒░░");
    puts("▒▒██▓▓████████████████▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒░░▒▒░░░░░░░░  ░░░░░░▓▓██▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▒▒▓▓▒▒");
    puts("▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░  ░░  ░░░░▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▒▒▓▓▒▒");
    puts("▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░  ░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓▓▓▒▒");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▒▒████▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓░░▒▒░░░░░░░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▓▓░░▒▒░░▒▒▒▒▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓██▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓████▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒░░░░▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓████");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████▓▓██");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓████");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓");
    puts("");

    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
