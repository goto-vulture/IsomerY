#!/bin/bash
# Einfaches Messen der Ausfuehrungszeit mittels des time-Befehls
# Insgesamt wird das Programm mehrfach im Debug und Release-Modus ausgefuehrt und der Mittelwert berechnet

DEFAULT_RUNS=5
TOTAL_RUNS=0
SEC_TOTAL_ADDED=0
AVERAGE_SEC=0
DEFAULT_NUM_C_ATOMS=10
TOTAL_NUM_C_ATOMS=1



# CLI-Parameter abfragen - falls vorhanden
# Falls keine Parameter vorhanden sind, wird der DEFAULT_RUNS Wert verwendet
if [[ "$#" == 0 ]];
then
    TOTAL_RUNS=${DEFAULT_RUNS}
    TOTAL_NUM_C_ATOMS=${DEFAULT_NUM_C_ATOMS}
elif [[ "$#" == 1 ]];
then
    TOTAL_RUNS=${1}
    TOTAL_NUM_C_ATOMS=${DEFAULT_NUM_C_ATOMS}
elif [[ "$#" == 2 ]];
then
    TOTAL_RUNS=${1}
    TOTAL_NUM_C_ATOMS=${2}
else
    printf "Too much CLI parameter ! 0 - 2 parameter (for the number of runs) are expected ! Got %d !\n" "$#"
    exit 1
fi

printf "Using %4d for the number of runs.\n" ${TOTAL_RUNS}
printf "Using %4d for the number of c-atoms.\n\n" ${TOTAL_NUM_C_ATOMS}


# test=0: Debug-Test
# test=1: Release-Test
for (( test=0; test<2; test++ ))
do
    if [[ ${test} -ne 0 ]];
    then
        printf "\n\n\n"
    fi

    SEC_TOTAL_ADDED=0
    AVERAGE_SEC=0

    # Programm vor Zeitmessung ohne Dokumentation neu uebersetzen
    make clean 1>> /dev/null

    if [[ ${test} -eq 0 ]];
    then
        echo -n "Compile Debug version (-j 1) ..."
        time make -j 1 Debug=1 NO_DOCUMENTATION=1 1> /dev/null
        echo "Done !"

        make clean 1>> /dev/null

        echo -n "Compile Debug version (-j) ..."
        time make -j Debug=1 NO_DOCUMENTATION=1 1> /dev/null
        printf "Done !\n\n"
    fi
    if [[ ${test} -eq 1 ]];
    then
        echo -n "Compile Release version (-j 1) ..."
        time make -j 1 Release=1 NO_DOCUMENTATION=1 1> /dev/null
        echo "Done !"

        make clean 1>> /dev/null

        echo -n "Compile Release version (-j) ..."
        time make -j Release=1 NO_DOCUMENTATION=1 1> /dev/null
        printf "Done !\n\n"
    fi

    # Programm mehrmals ausfuehren und die Ausfuehrungszeit sichern
    for (( i=0; i<${TOTAL_RUNS}; i++ ))
    do
        printf "===== Program run %4d / %4d =====\n" $(( ${i} + 1 )) ${TOTAL_RUNS}

        # Programm starten und Zeit-Ausgabe von time sichern
        if [[ ${test} -eq 0 ]];
        then
            PROGRAM_OUTPUT="$(time ( ./IsomerY_Debug_Linux -c ${TOTAL_NUM_C_ATOMS} ) 2>&1 1> /dev/null)"
        fi
        if [[ ${test} -eq 1 ]];
        then
            PROGRAM_OUTPUT="$(time ( ./IsomerY_Release_Linux -c ${TOTAL_NUM_C_ATOMS} ) 2>&1 1> /dev/null)"
        fi

        # "real" Zeitinformationen aus der Programmausgabe extrahieren
        TIME_INFO=$(echo ${PROGRAM_OUTPUT} | grep --extended-regexp --only-matching "real[[:blank:]]{1}[[:digit:]]{1,}m[[:digit:]]{1,2}\,[[:digit:]]{3}s")

        # Zeitinformationen an Leerzeichen aufspalten
        SPLITTED_TIME_INFO=($TIME_INFO)

        # Minuten und Sekunden aus den passenden Element extrahieren
        MIN=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 1)
        SEC=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 2 | cut -d 's' -f 1 | tr , .)

        # echo ${MIN}
        # echo ${SEC}

        if [[ -z ${MIN} ]];
        then
            echo "No minute output found! Maybe the programm exited abnormal (e.g. Segfault)."
            exit 1
        fi
        if [[ -z ${SEC} ]];
        then
            echo "No second output found! Maybe the programm exited abnormal (e.g. Segfault)."
            exit 1
        fi

        # Gesamte Anzahl an Sekunden, die das Programm in diesem Durchlauf gebraucht hat, ermitteln
        SEC_TOTAL=$(awk "BEGIN {print ${SEC} + 60 * ${MIN}}")

        # Aktuelle Ausfuehrungszeit mit den vorherigen Ausfuehrungszeiten addieren
        SEC_TOTAL_ADDED=$(awk "BEGIN {print ${SEC_TOTAL} + ${SEC_TOTAL_ADDED}}")

        # Bei der Ausgabe einer Gleitkommazahl mittels printf muss der Punkt durch ein Komma ersetzt werden !
        printf "Runtime: %8.3f s\n\n" $(echo ${SEC_TOTAL} | tr . ,)
    done

    # Mittelwert aus allen Ausfuehrungen bestimmen
    AVERAGE_SEC=$(awk "BEGIN {print ${SEC_TOTAL_ADDED} / ${TOTAL_RUNS}}")

    printf "=> Average runtime ("
    if [[ ${test} -eq 0 ]];
    then
        echo -n "Debug"
    fi
    if [[ ${test} -eq 1 ]];
    then
        echo -n "Release"
    fi

    printf "): %8.3f s\n" $(echo ${AVERAGE_SEC} | tr . ,)
done

make clean 1> /dev/null
