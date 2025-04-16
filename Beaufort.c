#include <stdio.h>
#include <string.h>
#include <ctype.h>

int mod26(int x) {
    return (x % 26 + 26) % 26;
}

void beaufortEncrypt(const char* plaintext, const char* key, char* ciphertext) {
    int keyLen = strlen(key);
    for (int i = 0; plaintext[i]; ++i) {
        if (isalpha(plaintext[i])) {
            int pt = toupper(plaintext[i]) - 'A';
            int k = toupper(key[i % keyLen]) - 'A';
            ciphertext[i] = 'A' + mod26(k - pt);
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

void beaufortDecrypt(const char* ciphertext, const char* key, char* plaintext) {
    beaufortEncrypt(ciphertext, key, plaintext);
}

int main() {
    const char* plaintext = "BEAUFORT";
    const char* key = "KEY";
    char ciphertext[1024];
    char decrypted[1024];
    
    beaufortEncrypt(plaintext, key, ciphertext);
    printf("Plaintext: %s\n", plaintext);
    printf("Key: %s\n", key);
    printf("Encrypted: %s\n", ciphertext);
    
    beaufortDecrypt(ciphertext, key, decrypted);
    printf("Decrypted: %s\n", decrypted);
    
    return 0;
}