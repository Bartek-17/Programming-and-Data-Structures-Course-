#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

long MyStrtol(const char* nptr, char** endptr, int base) {
    const char* s = nptr;
    int negative = 0;
    unsigned long result = 0;


    // invalid base check
    if (base != 0 && (base < 2 || base > 36)) {
        if (endptr) *endptr = (char *)nptr;
        errno = EINVAL;
        return 0;
    }

    // Skip leading whitespace
    while (isspace(*s)) s++;

    // sign
    if (*s == '-') {
        negative = 1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    // invalid string
    if(isalnum(*s) == 0){
        return 0;
    }


    // Determine base if not specified or if there is a prefix
    if (base == 0 || (base == 16 && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))) {
        if (*s == '0') {
            s++;
            if (*s == 'x' || *s == 'X') {
                base = 16;
                s++;
                if((*s) == 0){
                    s--;
                    *endptr = (char*) s;
                    return 0;
                }
            } else if (base == 0) {
                base = 8;  // Leading zero - octal
            }
        } 
        else if (base == 0) {
            base = 10;  // decimal by default
        }
    }


    // Parse digits
    while (*s) {

        int digit = 0;

        if ('0' <= *s && *s <= '9') {
            digit = *s - '0';
        } else if ('a' <= *s && *s <= 'z') {
            digit = *s - 'a' + 10;
        } else if ('A' <= *s && *s <= 'Z') {
            digit = *s - 'A' + 10;
        } else {
            break;  // invalid character
        }


        if (digit >= base) {
            if(base == 16){  // prefix Ox was mislead
                s--;
            }
            break;
        }

        // Check for overflow
        if (result > (ULONG_MAX - digit) / base) {
            errno = ERANGE;
            result = negative ? LONG_MIN : LONG_MAX;
            // ------------------------
            // keep parsing to set end ptr correctly if number is longer
            while (*s) {
                int digit = 0;
                if ('0' <= *s && *s <= '9') {
                    digit = *s - '0';
                } else if ('a' <= *s && *s <= 'z') {
                    digit = *s - 'a' + 10;
                } else if ('A' <= *s && *s <= 'Z') {
                    digit = *s - 'A' + 10;
                } else {
                    break;  // invalid character
                }

                if (digit >= base) {
                    break;
                }

                s++;
            }

            // --------------------------
            break;
        }

        result = result * base + digit;
        s++;
    }

    // Set endptr
    if (endptr) {
            *endptr = (char*) s;
    }

    // sign and overflow handling
    if (negative) {
        if (result > -(unsigned long)LONG_MIN) {
            errno = ERANGE;
            return LONG_MIN;
        }
        return -result;
    }

    if (result > LONG_MAX) {
        errno = ERANGE;
        return LONG_MAX;
    }

    return result;
}


int main(){
    const char sTekst[] = "0xT";
    char * EndPtr;
    char * EndPtr2;
    long number = MyStrtol( sTekst, &EndPtr, 0 );
    long b = strtol(sTekst, &EndPtr2, 0);
    if(EndPtr2 != EndPtr) {
        printf("!=\n");
    }
    printf( "Number = %ld\n", number );
    printf( "End pointer: %s\n", EndPtr );
    return 0;
}