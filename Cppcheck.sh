#!/bin/bash
# Testen des Quellcodes mittels Cppcheck
# Solche Testanwendungen finden bei weitem nicht alle Fehler ! Aber zumindest bei einigen Punkten koennen sie Hinweise
# ueber moegliche Fehler liefern.
# => Also ausprobieren. :)

echo "--enable=warning"
cppcheck --std=c11 --enable=warning --force ./src/ | grep -E "*\(warning\)*"
echo ""

echo "--enable=performance"
cppcheck --std=c11 --enable=performance --force ./src/ | grep -E "*\(performance\)*"
echo ""

echo "--enable=portability"
cppcheck --std=c11 --enable=portability --force ./src/ | grep -E "*\(portability\)*"
echo ""

echo "--enable=unusedFunction"
cppcheck --std=c11 --enable=unusedFunction --force ./src/ | grep -E "*\(unusedFunction\)*"
echo ""

echo "--enable=missingInclude"
cppcheck --std=c11 --enable=missingInclude --force ./src/ | grep -E "*\(missingInclude\)*"
echo ""
