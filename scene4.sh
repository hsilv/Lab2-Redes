#!/bin/bash

BOLD_WHITE="\033[1;37m"
BOLD_MINT_GREEN="\033[1;92m"
BOLD_CYAN="\033[1;96m"
RESET="\033[0m"

g++ -o receiver receiver.cpp

input1="10011"

echo -e "${BOLD_WHITE}----------------------ESCENARIO 4----------------------${RESET}"

echo -e "${BOLD_WHITE}Escenario 4 (Hamming): $input1${RESET}"
emitter1=$(python3 emitter.py -Ha $input1)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter1${RESET}"

flip_two_random_bits() {
    local binary_string=$1
    local length=${#binary_string}
    local flipped_string=$binary_string
    local random_index1=$((RANDOM % length))
    local random_index2=$((RANDOM % length))

    while [ $random_index1 -eq $random_index2 ]; do
        random_index2=$((RANDOM % length))
    done

    if [ "${flipped_string:$random_index1:1}" == "0" ]; then
        flipped_string="${flipped_string:0:$random_index1}1${flipped_string:$(($random_index1 + 1))}"
    else
        flipped_string="${flipped_string:0:$random_index1}0${flipped_string:$(($random_index1 + 1))}"
    fi

    if [ "${flipped_string:$random_index2:1}" == "0" ]; then
        flipped_string="${flipped_string:0:$random_index2}1${flipped_string:$(($random_index2 + 1))}"
    else
        flipped_string="${flipped_string:0:$random_index2}0${flipped_string:$(($random_index2 + 1))}"
    fi

    echo "$flipped_string"
}

flipped_emitter1=$(flip_two_random_bits $emitter1)
echo -e "${BOLD_MINT_GREEN}Emisor con bits modificados: $flipped_emitter1${RESET}"
receiver1=$(./receiver $flipped_emitter1 -Ha)
echo -e "${BOLD_CYAN}Receptor: $receiver1${RESET}"

echo -e "\n"

echo -e "En este caso, el error fue indetectable por el código de Hamming."