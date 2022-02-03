#!/bin/bash
# Im lokalen Netzwerk (192.168.178.x) nach Rechnern suchen
# Dies soll die Anzahl an moeglichen Geraeten einschraenken, wenn sich die Adressen der anderen Rechner im lokalen Netzwerk
# geaendert haben



count_used_addresses=0

for (( i=1; i<255; i++ ))
do
    local_address="192.168.178."
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
done

printf "\n\nNumber of used addresses: %d\n" ${count_used_addresses}
