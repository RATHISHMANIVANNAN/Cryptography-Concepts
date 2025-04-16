#include <stdio.h>
#include <string.h>
#include <ctype.h>

int mod26(int x) {
    return (x % 26 + 26) % 26;
}

void autokeyEncrypt(const char* plaintext, const char* key, char* ciphertext) {
    int keyLength = strlen(key);
    char runningKey[1024]; 
    
    strcpy(runningKey, key);
    
    for (int i = 0; plaintext[i]; ++i) {
        if (isalpha(plaintext[i])) {
            char ptChar = toupper(plaintext[i]);
            char keyChar;
            
            if (i < keyLength) {
                keyChar = toupper(key[i]);
            } else {
                keyChar = toupper(plaintext[i - keyLength]);
            }
            
            int pt = ptChar - 'A';
            int k = keyChar - 'A';
            ciphertext[i] = 'A' + mod26(pt + k);
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

void autokeyDecrypt(const char* ciphertext, const char* key, char* plaintext) {
    int keyLength = strlen(key);
    
    for (int i = 0; ciphertext[i]; ++i) {
        if (isalpha(ciphertext[i])) {
            char ctChar = toupper(ciphertext[i]);
            char keyChar;
            
            if (i < keyLength) {
                keyChar = toupper(key[i]);
            } else {
                keyChar = toupper(plaintext[i - keyLength]);
            }
            
            int ct = ctChar - 'A';
            int k = keyChar - 'A';
            char ptChar = 'A' + mod26(ct - k);
            plaintext[i] = ptChar;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
    const char* plaintext = "AUTOCLAVE";
    const char* key = "KEYAUTOCL";
    char ciphertext[1024];
    char decrypted[1024];
    
    autokeyEncrypt(plaintext, key, ciphertext);
    printf("Plaintext: %s\n", plaintext);
    printf("Key: %s\n", key);
    printf("Encrypted: %s\n", ciphertext);
    

    autokeyDecrypt(ciphertext, "KEY", decrypted);
    printf("Decrypted: %s\n", decrypted);
    
    return 0;
}