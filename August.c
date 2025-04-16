#include <stdio.h>
#include <string.h>

void caesar_cipher(char *text, char *result) {
    int shift = 1; 
    
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (ch >= 'a' && ch <= 'z')
            result[i] = ((ch - 'a' + shift) % 26) + 'a';
        else if (ch >= 'A' && ch <= 'Z')
            result[i] = ((ch - 'A' + shift) % 26) + 'A';
        else
            result[i] = ch; 
    }
    result[strlen(text)] = '\0'; 
}

int main() {
    char text[100], result[100];
    
    printf("Enter text to encrypt: ");
    fgets(text, sizeof(text), stdin);
    
    // Remove newline if present
    if (text[strlen(text) - 1] == '\n')
        text[strlen(text) - 1] = '\0';
    
    caesar_cipher(text, result);
    
    printf("Original text: %s\n", text);
    printf("Encrypted text: %s\n", result);
    
    return 0;
}
