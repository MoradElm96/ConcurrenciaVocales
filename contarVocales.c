#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

static char* nombreFichero = "ficheroPrueba.txt"; 
 
 

int contarVocalA(FILE *f,int vocalA,int leido)
{
    f = fopen(nombreFichero, "r");
    if (!f)
        {
            printf("no se ha podido abrir el archivo");
            return 1;
        }
    do
    {
         leido = tolower(fgetc(f)); // devuelve asci por eso es int, ademas convertimos todo a minisulas
        if (leido != EOF)
        {
            if (leido == 'a')
            {
                vocalA++;
            }
        }

    } while (!feof(f));
    fclose(f);
    return vocalA;
   
}

int contarVocalE(FILE *f,int vocalE,int leido)
{
  f = fopen(nombreFichero, "r");
  if (!f)
        {
            printf("no se ha podido abrir el archivo");
            return 1;
        }
    do  {
        leido = tolower(fgetc(f));
        if (leido != EOF)
        {
            if (leido == 'e')
            {
                vocalE++;
            }
        }

    } while (!feof(f));
    fclose(f);
    return vocalE;
   
}

int contarVocalI(FILE *f, int vocalI, int leido)
{
   f = fopen(nombreFichero, "r");
   if (!f)
        {
            printf("no se ha podido abrir el archivo");
            return 1;
        }
    do
    {
        leido = tolower(fgetc(f));
        if (leido != EOF)
        {
            if (leido == 'i')
            {
                vocalI++;
            }
        }

    } while (!feof(f));
    fclose(f);
    return vocalI;
  
}

int contarVocalO(FILE *f,int vocalO,int leido)
{
    f = fopen(nombreFichero, "r");
     if (!f)
        {
            printf("no se ha podido abrir el archivo");
            return 1;
        }
    do
    {
         leido = tolower(fgetc(f));
        if (leido != EOF)
        {
            if (leido == 'o')
            {
                vocalO++;
            }
        }

    } while (!feof(f));
    fclose(f);
    return vocalO;
   
}


int contarVocalU(FILE *f, int vocalU, int leido)
{
   f = fopen(nombreFichero, "r");
   if (!f)
        {
            printf("no se ha podido abrir el archivo");
            return 1;
        }
    do
    {
        leido = tolower(fgetc(f));
        if (leido != EOF)
        {
            if (leido == 'u')
            {
                vocalU++;
            }
        }

    } while (!feof(f));
    fclose(f);
    return vocalU;
}
///funciones genericas para enviar y leer por Pipes para comunicar al padre el resultado
void enviarNumVocal(int fd[],int numVocales)
{
    // pipe fd
    close(fd[0]);                            
    write(fd[1], &numVocales, sizeof(numVocales));
    close(fd[1]);
}
// leo el numero escrito por el pipe y lo retorno, para posteriormente su comprobacion
int leerNumeroVocal(int fd[])
{

    int resultado;
    close(fd[1]); // cerramos el descriptor de escritura
    read(fd[0], &resultado, sizeof(resultado));
    close(fd[0]);

    return resultado;
}

int main()
{
    // feof es un caracter de fin o no, devuelve no cero si el cursor esta al final e archivo
    // fget c, lee caracteres
    // feof si devuelve true, fin del archivo
    // fegts lee cadenas de caracteres;
  
    FILE *fichero;
    if (!fichero)
        {
            printf("no se ha podido abrir el archivo");
            return 1;
        }
        

    int leido;
    int vocalA = 0;
    int vocalE = 0;
    int vocalI = 0;
    int vocalO = 0;
    int vocalU = 0;

    int fd1[2], fd2[2],fd3[2],fd4[2],fd5[2];
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    pipe(fd5);

    pid_t pid_hijo1, pid_hijo2, pid_hijo3, pid_hijo4, pid_hijo5;

    /// HIJO1
    pid_hijo1 = fork();
    if (pid_hijo1 == 0)
    {
        printf("Soy el hijo 1 estoy contando la letra A... Mi padre es= %d, Mi PID= %d \n", getppid(), getpid());
        enviarNumVocal(fd1,contarVocalA(fichero,vocalA,leido));
        sleep(1);
        exit(0);
    }

    /// HIJO 2
    pid_hijo2 = fork();
    if (pid_hijo2 == 0)
    {

        printf("Soy el hijo 2 estoy contando la letra E... Mi padre es= %d, Mi PID= %d \n", getppid(), getpid());
        
        enviarNumVocal(fd2, contarVocalE(fichero,vocalE,leido));
        sleep(1);
        exit(0);
    }

    /// HIJO 3
    pid_hijo3 = fork();
    if (pid_hijo3 == 0)
    {

        printf("Soy el hijo 3 estoy contando la letra I... Mi padre es= %d, Mi PID= %d \n", getppid(), getpid());
        enviarNumVocal(fd3,contarVocalI(fichero,vocalI,leido));
        sleep(1);
        exit(0);
    }

    /// HIJO 4
    pid_hijo4 = fork();
    if (pid_hijo4 == 0)
    {

        printf("Soy el hijo 4 estoy contando la letra O...  Mi padre es= %d, Mi PID= %d \n", getppid(), getpid());
        enviarNumVocal(fd4,contarVocalO(fichero,vocalO,leido));
        sleep(1);
        exit(0);
    }

    /// HIJO 5
    pid_hijo5 = fork();
    if (pid_hijo5 == 0)
    {

        printf("Soy el hijo 5 estoy contando la letra U... Mi padre es= %d, Mi PID= %d \n", getppid(), getpid());
        enviarNumVocal(fd5,contarVocalU(fichero,vocalU,leido));
        sleep(1);
        exit(0);
    }

    // PADRE

    wait(NULL);//esperamos que terminen los hijos
    printf("\nProceso PADRE = %d, mi PID: %d \n", getppid(), getpid());

    printf("\nLetra A se repite : %d veces\n",leerNumeroVocal(fd1));
    printf("\nLetra E se repite : %d veces\n",leerNumeroVocal(fd2));
    printf("\nLetra I se repite : %d veces\n",leerNumeroVocal(fd3));
    printf("\nLetra O se repite : %d veces\n",leerNumeroVocal(fd4));
    printf("\nLetra U se repite : %d veces\n",leerNumeroVocal(fd5));

    printf("\nFin del programa\n");
    
    exit(0);
}
