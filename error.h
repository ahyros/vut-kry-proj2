/**
 * @file error.h
 * @author Andrej Hýroš
 * @date 03/04/2024
 */

#ifndef PROJ2_ERROR_H
#define PROJ2_ERROR_H

#include <iostream>

// error 1 = nespravny pocet argument
// error 2 = prepinace a ich argumenty nie su v spravnom poradi
// error 3 = chybajuci option
// error 4 = ziadny spravny základny (prvy) option

#define ERR_INVALID_ARG_CNT 1
#define ERR_INCORECT_OPT_ARG_ORDER 2
#define ERR_MISSING_OPTION 3
#define ERR_INVALID_PRIMARY_OPTION 4

#define ERR_INVALID_HASH_ENTERED 5

#define ERR_MEM_ALLOCATION_FAILED 10
#define ERR_ENDIANNESS_NOT_DEFINED 11


/**
 * @brief Used to end program with error message. Prints error message and exits with some error code.
 * @param errorMsg Error message. Will be printed to STDERR.
 * @param errorCode Error code. Program exits with this code.
 */
void throwError(const std::string& errorMsg, int errorCode);


#endif //PROJ2_ERROR_H
