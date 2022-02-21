#!/bin/bash
# Einfaches Messen der Ausfuehrungszeit mittels des time-Befehls
# Insgesamt wird das Programm mehrfach im Debug und Release-Modus ausgefuehrt und der Mittelwert berechnet

# Wichtig, damit das Trennzeichen bei Gleitkommazahlen immer ein Kommata ist !
LC_NUMERIC="de_DE.UTF-8"

# Keine Default-Werte mehr !
# DEFAULT_RUNS=5
# DEFAULT_NUM_C_ATOMS=10

TOTAL_RUNS=0
TOTAL_NUM_C_ATOMS=0

SEC_TOTAL_ADDED=0
AVERAGE_SEC=0


# CLI-Parameter abfragen - falls vorhanden
# Falls keine Parameter vorhanden sind, werden die Standard-Werte verwendet
#
# 1. CLI-Parameter: Anzahl an Testdurchlaeufe des Programms
# 2. CLI-Parameter: Anzahl der C-Atome, die fuer die Isomererstellung verwendet werden sollen
if [[ "$#" == 0 ]];
then
    # TOTAL_RUNS=${DEFAULT_RUNS}
    # TOTAL_NUM_C_ATOMS=${DEFAULT_NUM_C_ATOMS}
    echo "Missing parameter ! (Number of runs and c-atoms)"
    exit 1
elif [[ "$#" == 1 ]];
then
    TOTAL_RUNS=${1}
    # TOTAL_NUM_C_ATOMS=${DEFAULT_NUM_C_ATOMS}
    echo "Missing parameter ! (Number of c-atoms)"
    exit 1
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

    # Debug-Tests
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

    # Release-Tests
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
        PROG_NAME=""

        # Programm starten und Zeit-Ausgabe von time sichern
        if [[ ${test} -eq 0 ]];
        then
            # Linux oder Windows ?
            if [[ -e "./IsomerY_Debug_Win" ]];
            then
                PROG_NAME="./IsomerY_Debug_Win"
            elif [[ -e "./IsomerY_Debug_Linux" ]];
            then
                PROG_NAME="./IsomerY_Debug_Linux"
            else
                echo "No debug program file found !"
                exit 1
            fi
            PROGRAM_OUTPUT=$(time ( ${PROG_NAME} -c ${TOTAL_NUM_C_ATOMS} ) 2>&1 1> /dev/null)
            last_call=${?}
        fi
        if [[ ${test} -eq 1 ]];
        then
            # Linux oder Windows ?
            if [[ -e "./IsomerY_Release_Win" ]];
            then
                PROG_NAME="./IsomerY_Release_Win"
            elif [[ -e "./IsomerY_Release_Linux" ]];
            then
                PROG_NAME="./IsomerY_Release_Linux"
            else
                echo "No release program file found !"
                exit 1
            fi
            PROGRAM_OUTPUT=$(time ( ${PROG_NAME} -c ${TOTAL_NUM_C_ATOMS} ) 2>&1 1> /dev/null)
            last_call=${?}
        fi

        # Ist das Testprogramm normal beendet worden ?
        if [[ ${last_call} -ne 0 ]];
        then
            echo "In the last program call the program exits abnormal! (With return value: ${last_call})"
            exit 1
        fi

        # "real" Zeitinformationen aus der Programmausgabe extrahieren
        TIME_INFO=$(echo ${PROGRAM_OUTPUT} | grep --extended-regexp --only-matching "real[[:blank:]]{1}[[:digit:]]{1,}m[[:digit:]]{1,2}(\.|\,)[[:digit:]]{3}s")

        # Zeitinformationen an Leerzeichen aufspalten
        SPLITTED_TIME_INFO=($TIME_INFO)

        # Minuten und Sekunden aus den passenden Element extrahieren
        MIN=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 1)
        SEC=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 2 | cut -d 's' -f 1 | tr , .)

        # echo ${MIN}
        # echo ${SEC}

        # Sind Zeitinformationen nach den Testdurchlaeufen vorhanden ?
        # In Ausnahmefaellen kommen keine Werte zustande
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
