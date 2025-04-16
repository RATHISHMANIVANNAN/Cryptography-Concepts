#include <stdio.h>
#include <string.h>

void vigenere_encrypt(const char *text, const char *key, char *result) {
    int key_len = strlen(key);
    int i, j = 0;

    for (i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        char k = key[j % key_len];

        if (ch >= 'A' && ch <= 'Z') {
            result[i] = ((ch - 'A' + (k - 'A')) % 26) + 'A';
            j++;
        } else if (ch >= 'a' && ch <= 'z') {
            result[i] = ((ch - 'a' + (k - 'a')) % 26) + 'a';
            j++;
        } else {
            result[i] = ch; 
        }
    }
    result[i] = '\0';  
}

void vigenere_decrypt(const char *text, const char *key, char *result) {
    int key_len = strlen(key);
    int i, j = 0;

    for (i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        char k = key[j % key_len];

        if (ch >= 'A' && ch <= 'Z') {
            result[i] = ((ch - 'A' - (k - 'A') + 26) % 26) + 'A';
            j++;
        } else if (ch >= 'a' && ch <= 'z') {
            result[i] = ((ch - 'a' - (k - 'a') + 26) % 26) + 'a';
            j++;
        } else {
            result[i] = ch;  
        }
    }
    result[i] = '\0';
}

int main() {
    char plaintext[] = "CRYPTOISSHORTFORCRYPTOGRAPHY ";
    char key[] = "ABCDABCDABCDABCDABCDABCDABCD";
    char encrypted[100];
    char decrypted[100];

    vigenere_encrypt(plaintext, key, encrypted);
    printf("Encrypted: %s\n", encrypted);

    vigenere_decrypt(encrypted, key, decrypted);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}
