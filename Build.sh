#!/bin/bash
# Sehr einfaches Build-Skript fuer das Projekt IsomerY.
# Hier die Annahme, dass die Debug-Version fuer Linux gebaut werden soll !

# Aufraeumen
scons -c 1>> /dev/null

# 1. Build mittels der Zeitmessungen von scons
scons --debug=time

printf "\n\n\n\n"

# Wieder aufraeumen
scons -c 1>> /dev/null

# 2. Build mittels "time" fuer die Kompilierungszeit
time scons
