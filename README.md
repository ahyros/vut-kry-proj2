# KRY - SHA256 IMPLEMENATION

Description: This project is implementation of the second assignement for course
KRY (Cryptography) at Brno University of Technology, Faculty of Information Technologies

Author: Andrej Hýroš <xhyros00@stud.fit.vutbr.cz>  
Date: 6th of April, 2024


## Installation
To compile and run this project, follow these steps:
1. Run `make` in project's directory to compile the code
2. Run `./kry [options]` to run the program, where options are described in section "Usage"

## Usage
`kry` can be run in 4 different modes:
1. `-c` to simply generate sha256 hash of a message
2. `-s` to generate message authentication code (MAC) for message and key
3. `-v` to validate MAC for certain message and key
4. `-e` to perform length extension attack.

### Detailed usage
This section describes exact options and optargs needed to run different modes of the program.
Option like keys, MACs, secret key length for hash extension attacks are typically fed to the program as options, while
message itself in inputted using standart input.

#### SHA256 generation
To generate SHA256 hash for message M, run:  
`./kry -c`  
and then type in your message into the console  (standart input).  
Generated hash will be printed to standart output.

#### MAC generation
To generate MAC for you message M and key K, run:  
`./kry -s -k K`  
and then type in your message into the console  (standart input).
Generated MAC will be printed to standart ouput.

#### MAC validation
To validate MAC C for you message M and key K, run:  
`./kry -s -k K -m C`  
and then type in your message into the console (standart input).
Program does not print out anything in this mode, it just ends 
with `0` if MAC C is valid,  or `1` if it is not. This exit code
can be read by running `echo $?` right after running the program.

#### Length Extension Attack
To perform length extension attack (LEA) for orignal message M, secret
key length L, message extension E and orignal MAC C, run:  
`-./kry -e -n L -a E -m C`  
and then type orignal message into the console (standart input).  
Generated MAC for "fake" message will be printed to standart oouput  
followed by "fake" message itself, in which non-character bytes  
are displayed in hex format, such as `\x80`.

### Examples
Examples are taken from assignment.

`echo -ne "zprava" | ./kry -c`  
d8305a064cd0f827df85ae5a7732bf25d578b746b8434871704e98cde3208ddf

`echo -ne "zprava" | ./kry -s -k heslo`  
23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e

`echo -ne "zprava" | ./kry -v -k heslo -m 23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e`  
`echo $?`  
0

`echo -ne "message" | ./kry -v -k password -m 23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e`  
`echo $?`  
1

`echo -ne "zprava" | ./kry -e -n 5 -a ==message -m 23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e`
a3b205a7ebb070c26910e1028322e99b35e846d5db399aae295082ddecf3edd3  
zprava\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x58==message




## Files
Project contains these files:
- `main.cpp` - Program's entry point
- `sha256.cpp/h` - Hashing logic
- `argParser.cpp/h` - Parsing command line arguments and loading input from STDIN
- `utils.cpp/h` - Various helper/utility functions and declarations
- `error.cpp/h` - Error handling and codes
- `README.md` - This document
- `makefile` - Standart makefile for compilation







