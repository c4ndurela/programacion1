/*funcion que en un array cargado manualmente con numeros positivos, donde la suma de estos sea un n primo*/
#include <stdio.h>
#include <stdlib.h>

#define SIZEARRAY 4
void cargaArray(int array[]);
void muestraArray(int array2[]);
int sumaPrima(int primos[]); //retorna 1 si la suma es numero primo
int num[SIZEARRAY];


int main(){
    int prim;
    cargaArray(num);
    muestraArray(num);
    prim = sumaPrima(num);
    if (prim==1){
        printf("La suma es un n primo");
    }
    else{
        printf("La suma no es un n primo");
    }
    return 0;
}

void cargaArray(int array[]){//toma los numeros ingresados por el usuario para almacenarlos
    int i;
    printf("Ingrese %d números positivos.\n", SIZEARRAY);
    for(i=0; i<=SIZEARRAY; i++){
        scanf("%d", &array[i]);
        if (array[i]<0){
            printf("Ingrese un numero que sea positivo.\n");
            i--;
        }
    }
}
void muestraArray(int array2[]){//imprime en pantalla un arreglo
    int i;
    printf("\n [");
    for(i=0; i<=SIZEARRAY; i++){
        printf("%d ", array2[i]);
    }
    printf("]\n");
}
int sumaPrima(int primos[]){
    int i;
    int suma = 0;
    int divisores = 0;
    for(i=0;i<=SIZEARRAY; i++){
        suma = primos[i]+ suma;
    }
    for(i = 2; i < suma; i++) {
        if(suma % i == 0) {
            return 0; // No es primo, retorna 0
        }
    }
    return 1; // Es primo, retorna 1
}
