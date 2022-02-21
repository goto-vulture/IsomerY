#!/bin/bash
# Im lokalen Netzwerk (192.168.178.x) nach Rechnern suchen
# Dies soll die Anzahl an moeglichen Geraeten einschraenken, wenn sich die Adressen der anderen Rechner im lokalen Netzwerk
# geaendert haben

local_address=0
network_mask=0
start_address=0


if [[ "$#" == 0 ]];
then
    local_address="192.168.178."
    network_mask="255.255.255.0"
    start_address="192.168.178.1"
elif [[ "$#" == 1 ]];
then
    local_address=${1}
elif [[ "$#" == 2 ]];
then
    local_address=${1}
    network_mask=${2}
elif [[ "$#" == 3 ]];
then
    local_address=${1}
    network_mask=${2}
    start_address=${3}
else
    printf "Too much CLI parameter ! Got: %d; max. possible: 3 !\n" "#$"
    exit 1
fi


count_used_addresses=0

start_host_part=$(echo ${start_address} | cut -d '.' -f 4)
printf "Start address: %s\n\n" ${start_address}

for (( i=${start_host_part}; i<255; i++ ))
do
    local_address=${1}
    local_address+=${i}

    printf "Using: %s\r" ${local_address}

    #ping_out=$(ping ${local_address} -c 1 | grep --extended-regexp --only-matching "Destination Host Unreachable")

    # -c 1: Eine Abfrage
    # -W 1: Timeout nach 1000ms
    ping ${local_address} -c 1 -W 1 1> /dev/null
    ret_value_ping=${?}


    if [[ ${ret_value_ping} -eq 0 ]];
    then
        printf "Used address: %s " ${local_address}
        count_used_addresses=$(( ${count_used_addresses} + 1))

        # Ist eine SSH-Verbindung mit dem Geraet moeglich ?
        #ssh_test=$(ssh ${local_address} 2>> /dev/null | grep --extended-regexp --only-matching "Connection refused")

        #if [[ -z ${ssh_test} ]];
        #then
        #    printf " SSH No"
        #else
        #    printf " SSH Yes"
        #fi
        echo ""
    fi

    i_plus_one=$(( $i + 1 ))
    # Beim letzten Durchlauf die aktuelle Zeile leeren
    # Dient nur der besseren Lesbarkeit der Ausgaben !
    if [[ ${i_plus_one} -ge 255 ]];
    then
        for (( i2=0; i2<32; i2++ ));
        do
            printf " "
        done
    fi
done

printf "\n\nNumber of used addresses: %d\n" ${count_used_addresses}
