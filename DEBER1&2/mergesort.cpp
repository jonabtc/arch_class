#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <queue>
using namespace std;

/*******************************
 *  Page size constants and
 *  limits
 *  ***************************/
#define PAGESIZE 48//65536
#define PAGES 4//32768
#define SCRATCHPAGES 1
#define MAXPAGENO 2//32

/******************************
 * file names for input
 * and scratch files
 * should be used to sort
 * temporarily the file
 * ****************************/
const string INPUTFILE = "input.dat";
const string SCRATCHFILE = ".data.tmp";


void generateInputFile () {
    srand(time(0));
    ofstream os ("input.dat", ofstream::binary);
    int arr[PAGESIZE/sizeof(int)];
    for (int j = 0 ; j < PAGES ; j++) {
        for (int i = 0 ; i < PAGESIZE/sizeof(int) ; i++) {
            arr[i] = rand() % PAGESIZE;
        }

        os.write((const char*)&arr, PAGESIZE);
    }
    os.close();

}
int mergeSort(int bloque) {
	
    ifstream is(INPUTFILE, ifstream::binary);
    ofstream os(SCRATCHFILE, ifstream::binary);
    
    int *data[MAXPAGENO], *scratch = new int[PAGESIZE/sizeof(int)], count = 0, siguienteBloque = MAXPAGENO * bloque;
   
    // temp es el número de bloques que existe en un archivo.... paginaPosicion mantiene el la pagina en la que nos encontramos con la posicion del arreglo en que nos encontramos (por pagina)
    int temp = PAGES / (bloque/PAGESIZE), paginaPosicion [MAXPAGENO][2]; 
    int bloqueActual [MAXPAGENO];
    
    if(PAGES % (bloque/PAGESIZE) > 0) {
        temp++;
    }
    int temp2 = 0;
    
    

    for(int i = 0; i < MAXPAGENO; i++) {
        data[i] = new int[PAGESIZE/sizeof(int)];
    }
    //mientras exista más grupos de paginas que ordenar
    while(temp > 0) {
        
        if( temp > MAXPAGENO) {
            temp2 = MAXPAGENO;
        } else {
            temp2 = temp;
        }
        
        for(int i = 0; i < temp2; i++) {
            paginaPosicion[i][0] = 0;
            paginaPosicion[i][1] = 0;
            bloqueActual[i] = true;

        }
        
        bool revisar = true; //revisa si ya todos los valores del nuevo bloque estásn ordenados

       
        for(int i = 0; i < temp2; i++) {
            is.seekg(count * siguienteBloque + bloque * i);
            is.read((char *)data[i],PAGESIZE);
        }
        
        int internal = 0;

       
        while(revisar) {
            int aux = 0;
            int posicion = 0;
            bool first = true; 
            
            for(int i = 0; i < temp2; i++) {
                
                if(bloqueActual[i]) {
                    
                    if(data[i][paginaPosicion[i][1]] < aux || first) {
                        posicion = i;
                        aux = data[i][paginaPosicion[i][1]];
                        first = false;
                    }
                }
            }
            
            if(first) {
                revisar = false;
            }
            
            else {
                
                scratch[internal] = data[posicion][paginaPosicion[posicion][1]];
                paginaPosicion[posicion][1]++;
                internal++;
                
                if(internal == PAGESIZE/sizeof(int)) {
                    os.write((const char*)scratch, PAGESIZE);
                    internal = 0;
                }
                
                //buscamos la siguiente página
                if(paginaPosicion[posicion][1] >= PAGESIZE/sizeof(int)) {
                    paginaPosicion[posicion][1] = 0;
                    paginaPosicion[posicion][0]++;

                    unsigned long a = (unsigned long)PAGES * (unsigned long)PAGESIZE; 
                    long b = count * siguienteBloque + bloque * posicion + paginaPosicion[posicion][0]* PAGESIZE;
                    if(paginaPosicion[posicion][0] >= bloque/PAGESIZE || a <= b) { 
                        bloqueActual[posicion] = false;
                    } else {
                        is.seekg((count * siguienteBloque + bloque * posicion + paginaPosicion[posicion][0]*PAGESIZE));
                        is.read((char *)data[posicion],PAGESIZE);
                    }
                }
            }
        }
        count++;
        temp = temp - temp2;
    }
    os.close();
    is.close();
    remove (INPUTFILE.c_str());
    rename (SCRATCHFILE.c_str(),INPUTFILE.c_str());
   
    for(int i = 0; i < MAXPAGENO; i++) 
        delete data[i];
    
    delete scratch;
    return siguienteBloque;

}
void merge(int inicio) {
    int temp = inicio;
    unsigned long a= (unsigned long)PAGES * (unsigned long)PAGESIZE;
    while(temp <  a) {
        temp = mergeSort(temp);
    }
}

int main () {
    // creamos un nuevo archivo de numeros
    //generateInputFile();

    // creamos un puntero para mantener una pagina (PAGESIZE/4 enteros en la pagina)
    int * data;
    data = new int[PAGESIZE/sizeof(int) * MAXPAGENO];

    // ordenamos cada pagina y la guardamos en el archivo de scratch
    ifstream is(INPUTFILE, ifstream::binary);
    ofstream os(SCRATCHFILE, ifstream::binary);

    for (int i = 0 ; i < PAGES/MAXPAGENO ; i++) {
        is.read((char *)data, PAGESIZE * MAXPAGENO);
        // sort, definida en algorithm le damos el primer y ultimo elemento del bloque
        sort (data, data+(PAGESIZE/sizeof(int) * MAXPAGENO));
        os.write((const char*)data, PAGESIZE * MAXPAGENO);
    }

    delete data;

    //Una forma para que recoja aquel número de páginas que son menores a MAXPAGENO

    if(PAGES % MAXPAGENO > 0) {
        data = new int[PAGESIZE/sizeof(int) * (PAGES%MAXPAGENO)];
        is.read((char *)data, PAGESIZE * (PAGES%MAXPAGENO));
        sort (data, data+(PAGESIZE/sizeof(int) * (PAGES%MAXPAGENO)));
        os.write((const char*)data, PAGESIZE * (PAGES%MAXPAGENO));
        delete data;
    }
/*******************************************************************************************************/

    os.close();
    is.close();


    // borramos el archivo original y lo reemplazamos con el de scratch
    remove (INPUTFILE.c_str());
    rename (SCRATCHFILE.c_str(),INPUTFILE.c_str());

	//Al merge se le envía el grupo de paginas que se puede operar a la vez
    merge(MAXPAGENO*PAGESIZE); 

    // leemos pagina a pagina el archivo modificado
   is.open(INPUTFILE, ifstream::binary);
    for (int i = 0 ; i < PAGES ; i++) {
        is.seekg(i*PAGESIZE);
        is.read((char *)data, PAGESIZE);
        for (int j = 0; j < PAGESIZE/sizeof(int) ; j++)
            cout << data[j] << " ";
    }

    is.close();
    cout << endl;
    system("PAUSE");
    //delete data;
}

