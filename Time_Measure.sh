#!/bin/bash
# Einfaches Messen der Ausfuehrungszeit mittels des time-Befehls
# Insgesamt wird das Programm 5 Mal ausgefuehrt und der Mittelwert berechnet

# Programm vor Zeitmessung neu uebersetzen
make clean
make Debug=1

RUNS=3
SEC_TOTAL_ADDED=0
AVERAGE_SEC=0



printf "\n\n\n"

# Programm mehrmals ausfuehren und die Ausfuehrungszeit sichern
for (( i=0; i<${RUNS}; i++ ))
do
    printf "===== Program run %2d / %2d =====\n" $(( ${i} + 1 )) ${RUNS}

    # Programm starten und Ausgabe sichern
    PROGRAM_OUTPUT="$(time ( ./IsomerY_Debug_Linux ) 2>&1 1> /dev/null)"

    # "real" Zeitinformationen aus der Programmausgabe extrahieren
    TIME_INFO=$(echo ${PROGRAM_OUTPUT} | grep --extended-regexp --only-matching "real[[:blank:]]{1}[[:digit:]]{1,}m[[:digit:]]{1,2}\,[[:digit:]]{3}s")

    # Zeitinformationen an Leerzeichen aufspalten
    SPLITTED_TIME_INFO=($TIME_INFO)

    # Minuten und Sekunden aus den passenden Element extrahieren
    MIN=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 1)
    SEC=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 2 | cut -d 's' -f 1 | tr , .)

    # echo ${MIN}
    # echo ${SEC}

    # Gesamte Anzahl an Sekunden, die das Programm in diesem Durchlauf gebraucht hat, ermitteln
    SEC_TOTAL=$(awk "BEGIN {print ${SEC} + 60 * ${MIN}}")

    # Aktuelle Ausfuehrungszeit mit den vorherigen Ausfuehrungszeiten addieren
    SEC_TOTAL_ADDED=$(awk "BEGIN {print ${SEC_TOTAL} + ${SEC_TOTAL_ADDED}}")

    # Bei der Ausgabe einer Gleitkommazahl mittels printf muss der Punkt durch ein Komma ersetzt werden !
    printf "Runtime: %8.3f s\n\n" $(echo ${SEC_TOTAL} | tr . ,)
done



# Mittelwert aus allen Ausfuehrungen bestimmen
AVERAGE_SEC=$(awk "BEGIN {print ${SEC_TOTAL_ADDED} / ${RUNS}}")

printf "\n=> Average runtime: %8.3f s\n" $(echo ${AVERAGE_SEC} | tr . ,)
