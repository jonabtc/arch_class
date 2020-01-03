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
#define PAGESIZE 45 //65536
#define PAGES 4 //32768
#define SCRATCHPAGES 1
#define MAXPAGENO 2 //32

/******************************
 * file names for input
 * and scratch files
 * should be used to sort
 * temporarily the file
 * ****************************/
const string INPUTFILE = "input.dat";
const string SCRATCHFILE = ".data.tmp";


void generateInputFile ()
{
	srand(time(0));
	ofstream os ("input.dat", ofstream::binary);
	int arr[PAGESIZE/sizeof(int)];
	for (int j = 0 ; j < PAGES ; j++){
		for (int i = 0 ; i < PAGESIZE/sizeof(int) ; i++){
			arr[i] = rand() % PAGESIZE;
			//cout << arr[i] << " ";
		}
		
		os.write((const char*)&arr, PAGESIZE);
	}
	os.close();
	//cout << endl;

}
bool finalizo(int* a, int dimension){
	bool x = false;
	for(int i=0; i<dimension;i++)
		a[i]==PAGESIZE/sizeof(int)?x=true:x=false;
	return x;
}
void mergeSort(int** arreglo, int dimension){
	int* indices = new int [dimension];
	for(int i=0; i<dimension; i++)
		indices[i]=0;
	int* temp = new int[SCRATCHPAGES*PAGESIZE/sizeof(int)];
	int contador = 0, aux = 0;
	ofstream os(SCRATCHFILE, ifstream::binary);
	if(dimension>1){
	while(!finalizo(indices, dimension)){//(dimension>0){
		if(contador <= SCRATCHPAGES*PAGESIZE/sizeof(int)-1){
			cout << "esto ya 2" << endl;
			for(int i=0; i<dimension;i++){
				for(int j=0;j<dimension;j++){
					if(indices[j]!=12){
						aux=arreglo[j][j];
						break;
					}
				}
				cout << "esto ya 3" << endl;
				if(!aux <= arreglo[i][indices[i]]){
					aux = arreglo[i][indices[i]];
					indices[i]++;
				}
				cout << "esto ya 4" << endl;
			}
			temp[contador]=aux;
			contador++;
			cout << "contador: "<<contador << endl;
		//ORDENAR LOS ELEMENTOS DEL VECTOR, COMPARAR LOS PRIMEROS EL ARREGLO INDICES POSEE LA SEÑAL DE DONDE SE ENCUENTRA CADA ELEMENTO
			
		}else{
		//	for(int k=0; k<SCRATCHPAGES*PAGESIZE/sizeof(int); k++)
		//		os << temp[k]<<endl;
			os.write((const char*)temp, PAGESIZE);
		//	os.close();
			contador=0;
			cout << "esto ya 5" << endl;
			system("PAUSE");
		}
	}
	}
	else{
		for(int i=0; i<SCRATCHPAGES*PAGESIZE/sizeof(int);i++)
			temp[i]=arreglo[1][i];
	//	for(int k=0; k<SCRATCHPAGES*PAGESIZE/sizeof(int); k++)
	//			os << temp[k]<<endl;
		os.write((const char*)temp, PAGESIZE);
		//os.close();
		cout << "esto ya 6" << endl;
	}
	os.close();
	
}

int main ()
{
	// creamos un nuevo archivo de numeros
	generateInputFile();

	// creamos un puntero para mantener una pagina (PAGESIZE/4 enteros en la pagina)
	int * data;
	data = new int[PAGESIZE/sizeof(int)];

	// ordenamos cada pagina y la guardamos en el archivo de scratch
	ifstream is(INPUTFILE, ifstream::binary); 
	ofstream os(SCRATCHFILE, ifstream::binary);
	for (int i = 0 ; i < PAGES ; i++)
	{
		is.read((char *)data, PAGESIZE);
		// sort, definida en algorithm le damos el primer y ultimo elemento del bloque
		sort (data, data+PAGESIZE/sizeof(int));
		os.write((const char*)data, PAGESIZE);
	}


	os.close();
	is.close();

	// borramos el archivo original y lo reemplazamos con el de scratch
	remove (INPUTFILE.c_str());
	rename (SCRATCHFILE.c_str(),INPUTFILE.c_str());

	
	// leemos pagina a pagina el archivo modificado
	is.open(INPUTFILE, ifstream::binary); 
	for (int i = 0 ; i < PAGES ; i++)
	{
		is.seekg(i*PAGESIZE); 
		is.read((char *)data, PAGESIZE);
		for (int j = 0; j < PAGESIZE/sizeof(int) ; j++)
			cout << data[j] << " ";
	}

	is.close();
	cout << endl;
	system("PAUSE");
	/***********************************************************************************************************************************************/
	/*				falta mandar el bloque.erase cada vez que se añaden la cantidad de MAXPAGENO*/
	
	
//	vector <vector<int>> bloque;
	int** bloque;
	bloque = new int *[MAXPAGENO];
	for(int k=0; k<MAXPAGENO; k++)
		bloque[k]=new int[ PAGESIZE/sizeof(int)];
		
	is.open(INPUTFILE, ifstream::binary); 
	int a = PAGES;
	while(a>0){
		//vector<int>temp;
		if(a>MAXPAGENO){
			for(int i = 0; i<MAXPAGENO; i++){
				is.seekg(i*PAGESIZE); 
				is.read((char *)data, PAGESIZE);
				for(int j=0;j<PAGESIZE/sizeof(int);j++)
				bloque[i][j]=data[j];
				//temp.push_back(data[j]);
			}
		//	bloque.push_back(temp);
		//	temp.erase(temp.begin(), temp.end());
		cout << "esto ya" << endl;
			mergeSort(bloque, MAXPAGENO);
			a = a - MAXPAGENO;
		}else{
			for(int i = 0; i<a; i++){
				is.seekg(i*PAGESIZE); 
				is.read((char *)data, PAGESIZE);
				for(int j=0;j<PAGESIZE/sizeof(int);j++)
				bloque[i][j]=data[j];
				//temp.push_back(data[j]);
			}
		//	bloque.push_back(temp);
		//	temp.erase(temp.begin(), temp.end());
			mergeSort(bloque, a);
			a = 0;
		}
		// cout <<bloque.size()<<endl;
		// bloque.erase(bloque.begin(), bloque.end());
	}
	is.close();
	// borramos el archivo original y lo reemplazamos con el de scratch
	remove (INPUTFILE.c_str());
	rename (SCRATCHFILE.c_str(),INPUTFILE.c_str());
	
	for(int i=0; i < MAXPAGENO; i++){
		for(int j=0;j < PAGESIZE/sizeof(int) ; j++)
		cout<<"B["<<i<<"]"<<"["<<j<<"]:"<<bloque[i][j] <<endl;
	}
	
	
	/*queue<int*> bloque;
	is.open(INPUTFILE, ifstream::binary); 
	int a = PAGES;
	while(a>0){
		if(a>MAXPAGENO){
			for(int i = 0; i<MAXPAGENO; i++){
				is.seekg(i*PAGESIZE); 
				is.read((char *)data, PAGESIZE);
					bloque.push(data);
			}
			a = a - MAXPAGENO;
		}else{
			for(int i = 0; i<a; i++){
				is.seekg(i*PAGESIZE); 
				is.read((char *)data, PAGESIZE);
					bloque.push(data);
			}
			a = 0;
		}
	}
	is.close();
	cout << "elementos: " << bloque.size() << endl;*/
		
	
	
	
	/***********************************************************************************************************************************************/
	
		is.open(INPUTFILE, ifstream::binary); 
	for (int i = 0 ; i < PAGES ; i++)
	{
		is.seekg(i*PAGESIZE); 
		is.read((char *)data, PAGESIZE);
		for (int j = 0; j < PAGESIZE/sizeof(int) ; j++)
			cout << data[j] << " ";
	}

	is.close();
	cout << endl;
	delete data;
}

