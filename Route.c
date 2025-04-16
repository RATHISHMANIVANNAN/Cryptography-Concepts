#include <stdio.h>
#include <string.h>

void routeEncrypt(const char* plaintext, int n, char* ciphertext) {
    char mat[n][n];
    int idx = 0;
    int len = strlen(plaintext);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (idx < len)
                mat[i][j] = plaintext[idx++];
            else
                mat[i][j] = 'X';
        }
    }

    idx = 0;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            ciphertext[idx++] = mat[i][j];
        }
    }
    ciphertext[idx] = '\0';
}

void routeDecrypt(const char* ciphertext, int n, char* plaintext) {
    char mat[n][n];
    int idx = 0;

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            mat[i][j] = ciphertext[idx++];
        }
    }

    idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            plaintext[idx++] = mat[i][j];
        }
    }
    plaintext[idx] = '\0';
}

int main() {
    const char* message = "ROSESAREGREENANDLEAVESARERED";
    int n = 6;  

    char encrypted[100];
    char decrypted[100];

    routeEncrypt(message, n, encrypted);
    printf("Encrypted: %s\n", encrypted);

    routeDecrypt(encrypted, n, decrypted);
    printf("Decrypted: %s\n", decrypted); 

    return 0;
}
