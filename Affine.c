#include <stdio.h>
#include <string.h>

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int isValidA(int a) {
    return gcd(a, 26) == 1;
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1; 
}


int affine_encrypt(char *text, int a, int b, char *result) {
    if (!isValidA(a)) {
        return 0; 
    }
    
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z')
            result[i] = ((a * (ch - 'A') + b) % 26) + 'A';
        else if (ch >= 'a' && ch <= 'z')
            result[i] = ((a * (ch - 'a') + b) % 26) + 'a';
        else
            result[i] = ch;
    }
    result[strlen(text)] = '\0';
    return 1; 
}


int affine_decrypt(char *text, int a, int b, char *result) {
    if (!isValidA(a)) {
        return 0; 
    }
    
    int a_inv = modInverse(a, 26);
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z')
            result[i] = (a_inv * ((ch - 'A' - b + 26) % 26)) % 26 + 'A';
        else if (ch >= 'a' && ch <= 'z')
            result[i] = (a_inv * ((ch - 'a' - b + 26) % 26)) % 26 + 'a';
        else
            result[i] = ch;
    }
    result[strlen(text)] = '\0';
    return 1; 
}

int main() {
    char plaintext[100], encrypted[100], decrypted[100];
    int a, b;
    
    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    
    int len = strlen(plaintext);
    if (len > 0 && plaintext[len-1] == '\n')
        plaintext[len-1] = '\0';
    
    printf("Enter key value a: ");
    scanf("%d", &a);
    
    printf("Enter key value b: ");
    scanf("%d", &b);
    
    if (!affine_encrypt(plaintext, a, b, encrypted)) {
        printf("Error: Invalid key 'a' (must be coprime with 26)\n");
        printf("Valid values for a: 1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25\n");
        return 1;
    }
    
    printf("Encrypted: %s\n", encrypted);
    
    affine_decrypt(encrypted, a, b, decrypted);
    printf("Decrypted: %s\n", decrypted);
    
    return 0;
}
