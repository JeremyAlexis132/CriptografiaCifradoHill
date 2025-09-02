#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convierte caracter a número (A=0,...,Z=25, '.'=26, espacio=27)
int charToNum(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c == '.') return 26;
    if (c == ' ') return 27;
    return -1;
}

// Convierte número a caracter
char numToChar(int n) {
    if (n >= 0 && n <= 25) return 'A' + n;
    if (n == 26) return '.';
    if (n == 27) return ' ';
    return '?';
}

// Calcula el determinante para matrices 2x2 o 3x3
int determinante(int **mat, int n) {
    int det = 0;
    if (n == 2) {
        det = mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
    } else if (n == 3) {
        det = mat[0][0]*(mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1])
            - mat[0][1]*(mat[1][0]*mat[2][2] - mat[1][2]*mat[2][0])
            + mat[0][2]*(mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0]);
    }
    return det;
}

// Calcula el MCD
int mcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Función de cifrado Hill Cipher
void hillCipherEncrypt(int **key, int n, int *msgNum, int len) {
    int bloques = (len + n - 1) / n; // Número de bloques
    int *cifrado = (int *)malloc(bloques * n * sizeof(int));

    for (int b = 0; b < bloques; b++) {
        int bloque[n];
        for (int i = 0; i < n; i++) {
            int idx = b*n + i;
            bloque[i] = (idx < len) ? msgNum[idx] : 27; // Relleno espacio
        }
        for (int i = 0; i < n; i++) {
            int suma = 0;
            for (int j = 0; j < n; j++)
                suma += key[i][j] * bloque[j];
            suma %= 28;
            cifrado[b*n + i] = suma;
        }
    }

    printf("\nMensaje cifrado (números): ");
    for (int i = 0; i < bloques * n; i++) {
        printf("%d ", cifrado[i]);
    }
    printf("\nMensaje cifrado (texto): ");
    for (int i = 0; i < bloques * n; i++) {
        printf("%c", numToChar(cifrado[i]));
    }
    printf("\n");
    free(cifrado);
}

int main() {
    int n, i, j;
    printf("Introduce el tamaño de la matriz clave (n): ");
    scanf("%d", &n);

    int **key = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        key[i] = (int *)malloc(n * sizeof(int));

    printf("Introduce la matriz clave (%dx%d valores del 0 al 27):\n", n, n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &key[i][j]);

    getchar(); // Limpiar buffer

    char mensaje[256];
    printf("Introduce el mensaje (máx 255 caracteres, solo mayúsculas, punto y espacio):\n");
    fgets(mensaje, 256, stdin);
    mensaje[strcspn(mensaje, "\n")] = 0;

    int len = strlen(mensaje);
    int *msgNum = (int *)malloc(len * sizeof(int));
    for (i = 0; i < len; i++) {
        msgNum[i] = charToNum(mensaje[i]);
        if (msgNum[i] < 0) {
            printf("Caracter inválido en el mensaje: %c\n", mensaje[i]);
            for (int k = 0; k < n; k++) free(key[k]);
            free(key);
            free(msgNum);
            return 1;
        }
    }

    int det = determinante(key, n) % 28;
    if (det < 0) det += 28;
    printf("Determinante de la matriz clave: %d\n", det);
    if (mcd(det, 28) != 1) {
        printf("La matriz NO tiene inversa módulo 28, elige otra clave.\n");
    } else {
        printf("La matriz SÍ tiene inversa módulo 28, puedes cifrar.\n");
        hillCipherEncrypt(key, n, msgNum, len);
    }

    for (i = 0; i < n; i++)
        free(key[i]);
    free(key);
    free(msgNum);

    return 0;
}
