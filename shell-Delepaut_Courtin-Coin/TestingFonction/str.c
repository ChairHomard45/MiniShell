#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define maxs 101

int substenv(char* str, size_t max) {
    if (str == NULL || max <= 0) {
        printf("Paramètres invalid\n");
        return -1; // Invalid parameters
    }

    int Lec = 0;
    int dif = 0;
    char varName[64]; // To store the variable name

    while (str[Lec] != '\0' && Lec < max - 1) {
        if (str[Lec] == '$') {
            int start = Lec + 1;
            if (str[start] == '{') {
                start++;
                while (str[Lec] != '}' && str[Lec] != '\0' && Lec < max - 1) {
                    Lec++;
                }
            } else {
                while ((str[Lec] >= 'A' && str[Lec] <= 'Z') || (str[Lec] >= 'a' && str[Lec] <= 'z')) {
                    Lec++;
                }
            }

            if (Lec != start) {
                // Extract the variable name
                strncpy(varName, str + start, Lec - start);
                varName[Lec - start] = '\0';

                // Get the environment variable value
                char* envValue = getenv(varName);

                if (envValue != NULL) {
                    int envValueLen = strlen(envValue);
                    int remaining = max - dif - 1; // Leave space for the null-terminator
                    if (envValueLen <= remaining) {
                        // Replace the variable with its value
                        strcpy(str + dif, envValue);
                        dif += envValueLen;
                    } else {
                        // Handle insufficient space (you can resize the buffer or handle it as needed)
                    }
                }
            }
        }
        Lec++;
        dif++;
    }

    return strlen(str);
}

int main() {
    char TMPCMDLINE[256] = "${HOME} -He- $USER D- $UID ";
    printf("Hello: ");
    int result = substenv(TMPCMDLINE, 256);

    if (result > 0) {
        printf("Result: %s\n", TMPCMDLINE);
    } else {
        printf("Replacement failed.\n");
    }
}
