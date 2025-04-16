#include <stdio.h>
#include <string.h>
#include <ctype.h>

void gronsfeld_encrypt(char *text, char *key, char *result) {
    int key_len = strlen(key);
    
    for (int i = 0, j = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        
        if (isalpha(ch)) {
            int shift = key[j % key_len] - '0'; 
            
            if (shift < 0 || shift > 9) {
                printf("Error: Key should contain only digits 0-9\n");
                return;
            }
            
            char base = isupper(ch) ? 'A' : 'a';
            result[i] = ((ch - base + shift) % 26) + base;
            j++;  
        } else {
            result[i] = ch;
        }
    }
    result[strlen(text)] = '\0';
}

void gronsfeld_decrypt(char *text, char *key, char *result) {
    int key_len = strlen(key);
    
    for (int i = 0, j = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        
        if (isalpha(ch)) {
            int shift = key[j % key_len] - '0';  
            
            if (shift < 0 || shift > 9) {
                printf("Error: Key should contain only digits 0-9\n");
                return;
            }
            
            char base = isupper(ch) ? 'A' : 'a';
            result[i] = ((ch - base - shift + 26) % 26) + base;
            j++;  // Only increment key index for alphabetic characters
        } else {
            result[i] = ch;
        }
    }
    result[strlen(text)] = '\0';
}

int main() {
    char plaintext[100];
    char key[100];
    char encrypted[100];
    char decrypted[100];
    
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    
    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len-1] == '\n') {
        plaintext[len-1] = '\0';
    }
    
    printf("Enter the numeric key (digits 0-9 only): ");
    fgets(key, sizeof(key), stdin);
    
    len = strlen(key);
    if (len > 0 && key[len-1] == '\n') {
        key[len-1] = '\0';
    }
    
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isdigit(key[i])) {
            printf("Error: Key should contain only digits 0-9\n");
            return 1;
        }
    }
    
    gronsfeld_encrypt(plaintext, key, encrypted);
    printf("Encrypted text: %s\n", encrypted);
    
    gronsfeld_decrypt(encrypted, key, decrypted);
    printf("Decrypted text: %s\n", decrypted);
    
    return 0;
}