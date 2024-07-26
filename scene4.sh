#!/bin/bash

BOLD_WHITE="\033[1;37m"
BOLD_MINT_GREEN="\033[1;92m"
BOLD_CYAN="\033[1;96m"
RESET="\033[0m"

g++ -o receiver receiver.cpp

input1="1001"

echo -e "${BOLD_WHITE}----------------------ESCENARIO 4----------------------${RESET}"

echo -e "${BOLD_WHITE}Escenario 4 (Hamming): $input1${RESET}"
emitter1=$(python3 emitter.py -Ha $input1)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter1${RESET}"

flip_two_random_bits() {
    local binary_string=$1
    local length=${#binary_string}
    local max_flips=2
    local num_bits_to_flip=$((RANDOM % max_flips + 1))
    local flipped_string=$binary_string
    local random_index
    declare -A flipped_indices

    calculate_parity_bits() {
        local binary=$1
        local len=${#binary}
        local parity_bits=()
        local i j k

        for ((i = 0; (1 << i) < len; i++)); do
            parity_bits[$i]=0
            for ((j = 1; j <= len; j++)); do
                if ((j & (1 << i))); then
                    parity_bits[$i]=$((parity_bits[$i] ^ ${binary:len-j:1}))
                fi
            done
        done

        echo "${parity_bits[@]}"
    }

    original_parity_bits=($(calculate_parity_bits "$binary_string"))

    for ((i = 0; i < num_bits_to_flip; i++)); do
        while : ; do
            random_index=$((RANDOM % length))
            if [ -z "${flipped_indices[$random_index]}" ]; then
                flipped_indices[$random_index]=1
                break
            fi
        done

        if [ "${flipped_string:$random_index:1}" == "0" ]; then
            flipped_string="${flipped_string:0:$random_index}1${flipped_string:$(($random_index + 1))}"
        else
            flipped_string="${flipped_string:0:$random_index}0${flipped_string:$(($random_index + 1))}"
        fi
    done

    new_parity_bits=($(calculate_parity_bits "$flipped_string"))

    for ((i = 0; i < ${#original_parity_bits[@]}; i++)); do
        if [ "${original_parity_bits[$i]}" != "${new_parity_bits[$i]}" ]; then
            for ((j = 0; j < length; j++)); do
                if ((j & (1 << i))); then
                    if [ "${flipped_string:$j:1}" == "0" ]; then
                        flipped_string="${flipped_string:0:$j}1${flipped_string:$(($j + 1))}"
                    else
                        flipped_string="${flipped_string:0:$j}0${flipped_string:$(($j + 1))}"
                    fi
                    break
                fi
            done
        fi
    done

    echo "$flipped_string"
}

flipped_emitter1=$(flip_two_random_bits $emitter1)
echo -e "${BOLD_MINT_GREEN}Emisor con bits modificados: $flipped_emitter1${RESET}"
receiver1=$(./receiver $flipped_emitter1 -Ha)
echo -e "${BOLD_CYAN}Receptor: $receiver1${RESET}"

echo -e "\n"

echo -e "${BOLD_WHITE}Escenario 4 (CRC32): $input1${RESET}"
emitter1=$(python3 emitter.py -CRC $input1)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter1${RESET}"
flipped_emitter1=$(flip_two_random_bits $emitter1)
echo -e "${BOLD_MINT_GREEN}Emisor con bits modificados: $flipped_emitter1${RESET}"
receiver1=$(./receiver $flipped_emitter1 -CRC)
echo -e "${BOLD_CYAN}Receptor: $receiver1${RESET}"

echo -e "\n"

echo -e "En este caso, el error fue indetectable por el código de Hamming."
