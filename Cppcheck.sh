#!/bin/bash
# Testen des Quellcodes mittels Cppcheck
# Solche Testanwendungen finden bei weitem nicht alle Fehler ! Aber zumindest bei einigen Punkten koennen sie Hinweise
# ueber moegliche Fehler liefern.
# => Also ausprobieren. :)

echo "--enable=warning"
cppcheck --std=c11 --enable=warning ./src/ | grep -E "*\(warning\)*"
echo ""

echo "--enable=performance"
cppcheck --std=c11 --enable=performance ./src/ | grep -E "*\(performance\)*"
echo ""

echo "--enable=portability"
cppcheck --std=c11 --enable=portability ./src/ | grep -E "*\(portability\)*"
echo ""

echo "--enable=unusedFunction"
cppcheck --std=c11 --enable=unusedFunction ./src/ | grep -E "*\(unusedFunction\)*"
echo ""

echo "--enable=missingInclude"
cppcheck --std=c11 --enable=missingInclude ./src/ | grep -E "*\(missingInclude\)*"
echo ""
