#!/bin/bash

BOLD_WHITE="\033[1;37m"
BOLD_MINT_GREEN="\033[1;92m"
BOLD_CYAN="\033[1;96m"
RESET="\033[0m"

g++ -o receiver receiver.cpp

input1="10011"
input2="1011011010001"
input3="1101"

flip_random_bit() {
    local binary_string=$1
    local length=${#binary_string}
    local random_index=$((RANDOM % length))
    local flipped_bit

    if [ "${binary_string:$random_index:1}" == "0" ]; then
        flipped_bit="1"
    else
        flipped_bit="0"
    fi

    echo "${binary_string:0:$random_index}${flipped_bit}${binary_string:$(($random_index + 1))}"
}


echo -e "${BOLD_WHITE}----------------------ESCENARIO 2----------------------${RESET}"

echo -e "${BOLD_WHITE}Escenario 2 (Hamming): $input1${RESET}"
emitter1=$(python3 emitter.py -Ha $input1)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter1${RESET}"
flipped_emitter1=$(flip_random_bit $emitter1)
echo -e "${BOLD_MINT_GREEN}Emisor con bit modificado: $flipped_emitter1${RESET}"
receiver1=$(./receiver $flipped_emitter1 -Ha)
echo -e "${BOLD_CYAN}Receptor: $receiver1${RESET}"

echo -e "\n"

echo -e "${BOLD_WHITE}Escenario 2 (Hamming): $input2${RESET}"
emitter2=$(python3 emitter.py -Ha $input2)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter2${RESET}"
flipped_emitter2=$(flip_random_bit $emitter2)
echo -e "${BOLD_MINT_GREEN}Emisor con bit modificado: $flipped_emitter2${RESET}"
receiver2=$(./receiver $flipped_emitter2 -Ha)
echo -e "${BOLD_CYAN}Receptor: $receiver2${RESET}"

echo -e "\n"

echo -e "${BOLD_WHITE}Escenario 2 (Hamming): $input3${RESET}"
emitter3=$(python3 emitter.py -Ha $input3)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter3${RESET}"
flipped_emitter3=$(flip_random_bit $emitter3)
echo -e "${BOLD_MINT_GREEN}Emisor con bit modificado: $flipped_emitter3${RESET}"
receiver3=$(./receiver $flipped_emitter3 -Ha)
echo -e "${BOLD_CYAN}Receptor: $receiver3${RESET}"

echo -e "\n"

echo -e "${BOLD_WHITE}Escenario 2 (CRC32): $input1${RESET}"
emitter1=$(python3 emitter.py -CRC $input1)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter1${RESET}"
flipped_emitter1=$(flip_random_bit $emitter1)
echo -e "${BOLD_MINT_GREEN}Emisor con bit modificado: $flipped_emitter1${RESET}"
receiver1=$(./receiver $flipped_emitter1 -CRC)
echo -e "${BOLD_CYAN}Receptor: $receiver1${RESET}"

echo -e "\n"

echo -e "${BOLD_WHITE}Escenario 2 (CRC32): $input2${RESET}"
emitter2=$(python3 emitter.py -CRC $input2)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter2${RESET}"
flipped_emitter2=$(flip_random_bit $emitter2)
echo -e "${BOLD_MINT_GREEN}Emisor con bit modificado: $flipped_emitter2${RESET}"
receiver2=$(./receiver $flipped_emitter2 -CRC)
echo -e "${BOLD_CYAN}Receptor: $receiver2${RESET}"

echo -e "\n"

echo -e "${BOLD_WHITE}Escenario 2 (CRC32): $input3${RESET}"
emitter3=$(python3 emitter.py -CRC $input3)
echo -e "${BOLD_MINT_GREEN}Emisor: $emitter3${RESET}"
flipped_emitter3=$(flip_random_bit $emitter3)
echo -e "${BOLD_MINT_GREEN}Emisor con bit modificado: $flipped_emitter3${RESET}"
receiver3=$(./receiver $flipped_emitter3 -CRC)
echo -e "${BOLD_CYAN}Receptor: $receiver3${RESET}"

echo -e "\n"
