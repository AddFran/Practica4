#include <iostream> // Libreria estandar
#include <fstream> // Libreria para trabajar con archivos
#include <sstream> // Libreria para manipular cadenas de texto (stringstream)
#include <string> // Libreria para trabajar con cadenas de texto
#include <conio.h> 
using namespace std;

// Clase para crear nodos (tambien podemos usar struct)
class Node{
    public:
        int id; // ID unico para cada objeto
        string nombre,correo,carrera; // Datos
        int anio; // Otro dato
        Node* next; // Nodo siguiente
        Node* prev; // Nodo anterior

        // Constructor de la clase, inicializamos next y prev como nullptr
        Node(int _id,string _nombre,string _correo,string _carrera,int _anio) : id(_id),nombre(_nombre),correo(_correo),carrera(_carrera),anio(_anio), next(nullptr),prev(nullptr) {}
};

// Clase para crear la lista de estudiante, usaremos una lista doblemente enlazada
class listaDoble{
    private:
        Node* head; // Indica la cabeza de la lista
        int idActual; // Id actual
    public:
        // Constructor por defecto
        listaDoble(){
            head=nullptr;
            idActual=1;
        }
        // Funciones principales
        void agregar(string,string,string,int);
        void mostrar();
        void buscarPorNombre(string);
        void buscarPorId(int);
        void modificar(int);
        void eliminar(int);
        void guardarEnArchivo();
        void cargarDesdeArchivo();
        void recargarDesdeArchivo();
};
// Funcion para agregar un estudiante a la lista
void listaDoble::agregar(string name, string correo, string carrera, int anio){
    Node* nuevo=new Node(idActual++,name,correo,carrera,anio); // Creamos un nuevo nodo, y le asignamos los parametros como sus atributos
    // En caso de que sea el primer nodo de la lista
    if(!head){
        head=nuevo;
    }
    // Si no es el primer elemento usamos otro metodo de insersion
    else{
        Node* current=head; // Creamos un puntero nodo current y hacemos que haga referencia a head
        // Con ayuda de un while y el nodo creado nos desplazamos por la lista, hasta que current apunte al ultimo elemento
        while(current->next){
            current=current->next;
        }
        // Cuando current apunte al ultimo nodo...
        current->next=nuevo; // current->next dejara de apuntar a nullptr, para apuntar al nuevo nodo 
        nuevo->prev=current; // Ya que es una lista doblemente enlazada, indicamos que nuevo->prev apunte a current para realizar las conexiones correctamente
    }
    guardarEnArchivo(); // Llamamos a la funcion para guardar en el archivp
}
// Funcion para mostrar la lista
void listaDoble::mostrar(){
    Node* current=head; // Creamos un puntero nodo current y hacemos que haga referencia a head
    // Hacemos que muestr la info
    cout<<"\nID\tNombre\t\tCorreo\t\t\tCarrera\t\tAnio\n"; //   \n para salto de linea, \t para espaciar como si presionaramos tab
    cout<<"--------------------------------------------------------------\n";
    // Recorremos la lista y mostramos sus elementos con current
    while(current){
        cout<<current->id<<"\t"<<current->nombre<<"\t\t"<<current->correo<<"\t"<<current->carrera<<"\t"<<current->anio<<endl;
        current=current->next;
    }
    cout<<endl;
}
// Funcion para buscar un elemento por el atributo nombre
void listaDoble::buscarPorNombre(string nombreBuscar){
    Node* current=head; // Nodo para desplazarnos por la lista
    bool encontrado=false; // Bandera para indicar si ya encontrasmos el elemento
    // Bucle para desplazarnos por la lista
    while(current){
        if(current->nombre==nombreBuscar){ // Comprobamos si el atributo nombre es el que buscamos, si lo es mostramos la info
            cout<<"\nRegistro encontrado:\n";
            cout<<" ID: "<<current->id<<endl;
            cout<<" Nombre: "<<current->nombre<<endl;
            cout<<" Correo: "<<current->correo<<endl;
            cout<<" Carrera: "<<current->carrera<<endl;
            cout<<" Anio: "<<current->anio<<endl;
            encontrado=true; // Cambiamos el estado
            break;
        }
        current=current->next; // Cambiamos el a current para movernos
    }
    // Si no lo encontramos mostramos el mensaje correspondiente
    if(!encontrado){
        cout<<"No se encontro el nombre: "<<nombreBuscar<<endl;
    }
}
// Funcio para buscar un elemento por ID, practicamente igual a buscarPorNombre(), solo que ahora con un entero id
void listaDoble::buscarPorId(int id_b){
    Node* current=head;
    while(current){
        if(current->id==id_b){
            cout<<"\nRegistro encontrado:"<<endl;
            cout<<" ID: "<<current->id<<endl;
            cout<<" Nombre: "<<current->nombre<<endl;
            cout<<" Correo: "<<current->correo<<endl;
            cout<<" Carrera: "<<current->carrera<<endl;
            cout<<" Anio: "<<current->anio<<endl;
            return; // Si los IDs coinciden, entonces salimos de la funcion
        }
        current=current->next;
    }
    // Si llegamos aqui significa que no encontro algun id igual, por lo que el id bucaado no existe
    cout<<"No se encontro algun registro con la ID "<<id_b<<endl;
}
// Funcion para modificar los datos de un ID
void listaDoble::modificar(int id_b){ // Pasamos un id
    Node* current=head; // Node para recorrer la lista
    while(current){
        if(current->id==id_b){ // Si los id coinciden
            // Pedimos al usuario que ingrese los nuevos datos
            cout<<"Registro encontrado. Actualice los datos:"<<endl;
            string nombre,correo,carrera;
            int anio;
            cout<<"Nombre completo: ";
            getline(cin,nombre); 
            cout<<"Correo: ";
            getline(cin,correo);
            cout<<"Carrera profesional: ";
            getline(cin,carrera);
            cout<<"Año de ingreso: ";
            cin>>anio; 
            cin.ignore();
            
            // Reemplazamos los datos antiguos con los nuevos
            current->nombre=nombre;
            current->correo=correo;
            current->carrera=carrera;
            current->anio=anio;

            return; 
        }
        current=current->next;
    }
    // Si llegamos aqui ningun id coincidio, por lo que no existe
    cout<<"ID no encontrado"<<endl;
}
// Funcion para eliminar un elemento por ID
void listaDoble::eliminar(int idEliminar){
    Node* current=head; // Nodo para desplazarnos
    // Con el bucle y el nodo nos movemos por la lista
    while(current){
        if(current->id==idEliminar){ // Comprobamos si el ID coincide, si es asi...
            // Manejamos casos en los que prev y next apunten a otro nodo, correjimos esto la lista ya no esta enlazada 
            if(current->prev) 
                current->prev->next=current->prev;
            else
                head=current->next;
            if(current->next)
                current->next->prev=current->prev;
            delete current; // Manejado cada caso especial, borramos el nodo
            guardarEnArchivo(); // Guardamos los cambioa
            cout<<"Registro eliminado con exito.\n";
            return; // Detenemos la funcion
        }
        current=current->next; // Nos movemos
    }
    cout<<"ID no encontrado.\n"; // Si llegamos aqui, significa que no encontramos el elemento que buscamos eliminar
}
// Funcion para guardar la lista en un archivo .txt
void listaDoble::guardarEnArchivo(){
    ofstream archivo("registros.txt"); // ofstream: Permite crear o sobreescribir un archivo, en este caso un .txt
    Node* current=head; // Nodo para despalazarnos por la lista
    while(current){ // Nos movemos por la lista
        // En el archivo escribimos cada dato obtenido del nodo current
        archivo<<current->id<<";"<<current->nombre<<";"<<current->correo<<";"<<current->carrera<<";"<<current->anio<<endl;
        current=current->next; // Cambiamos el valor de current al nodo siguiente
    }
    archivo.close(); // Cerramos el archivo
}
// Funcion para obtener datos de un archivo .txt
void listaDoble::cargarDesdeArchivo(){
    ifstream archivo("registros.txt"); // ifstream: Permite leer datos de archivos
    string linea; // Cramos un string para almacenar una linea del archivo
    while(getline(archivo,linea)){ // Iterammos linea a linea sobre el archivo que abrimos
        stringstream ss(linea); 
        /* 
            stringstream: Clase de la libreria sstream que permite manejar una cadena de texto con entrada y salida (cin y cout),
            es como un archivo temporal, en el cual podemos almacenar una cadena de texto, y extraer o insertar subcadenas de el.
            En este caso, estamos diciendo que ss almacenara la cadena de texto linea, ahora por medio de ss podemos extraer frgmentos
            de la cadena mas adelante
        */

        // Declaramos variables que almacenaran los datos extraidos del string 
        string campo; // Variable auxiliar
        int id,anio;
        string nombre,correo,carrera;

        // Usamos getline para obtener la linea
        getline(ss,campo,';'); // Obtenemos la linea ss, extraaemos un string hasta que encuentre un ";", dicho string lo guardamos en la variable auxiliar "campo"
        id=stoi(campo); // Convertimos el string "campo" e un entero, y lo guardamos en la variable "id"
        getline(ss,nombre,';'); // Obtenemos de la linea ss... funciona igual que ell primer getline, ademas, ignora el primer string obtenido (id)
        getline(ss,correo,';');
        getline(ss,carrera,';');
        getline(ss,campo,';'); 
        anio=stoi(campo); // Convertimso el string a entero
        // Luego de esto ya extraimos los datos de la linea y los guardamos en variables

        // Con los datos obtenidos, creamos una nueva lista
        Node* nuevo=new Node(id,nombre,correo,carrera,anio);
        if(!head){
            head=nuevo;
        }else{
            Node* current=head;
            while(current->next)
                current=current->next;
            current->next=nuevo;
            nuevo->prev=current;
        }
        if(id>=idActual)
            idActual=id+1;
    }
    // Cuando se acaben todas las lineas del archivo, ya habremos acabado de obtener todos los datos de la lista
    archivo.close(); // Cerramos el archivo
}
// Funcion que limpia la lista y luego obteniene los datos del archivo
void listaDoble::recargarDesdeArchivo(){
    Node* current=head;
    while(current){
        Node* temp=current;
        current=current->next;
        delete temp;
    }
    head=nullptr;
    idActual=1;
    cargarDesdeArchivo();
    cout<<"Registros recargados desde archivo correctamente.\n";
}

int main(){
    listaDoble lista;
    int opcion;

    while(true){
        system("cls");
        cout<<"\n--- SISTEMA DE REGISTRO ACADEMICO ---\n";
        cout<<"1. Agregar nuevo estudiante\n";
        cout<<"2. Mostrar registro\n";
        cout<<"3. Buscar por nombre\n";
        cout<<"4. Buscar por ID\n";
        cout<<"5. Modificar\n";
        cout<<"6. Eliminar por ID\n";
        cout<<"7. Salir\n";
        cout<<"8. Cargar registros desde archivo (manual)\n";
        cout<<"Seleccione una opcion\n";
        cin>>opcion;
        cin.ignore(); // cin.ignore(): Tecnicamente, descarta el siguiente caracter en el buffer de entrada
        /*
            Para este caso, al usar cin>>, el programa solo lee el numero, pero el salto de linea queda en la memoria
            Luego, al usar getline(), el programa toma como valido el salto de linea, ignorando la entrada de datos despues
            Es como si ni bien ingresado opcion, presionaramos enter para no ingresar ningun valor, solo que es culpa del programa
            Con cin.ignore() hacemos que ese salto de linea residual sea borrado, permitiendo el correcto funcionamiento del programa
        */ 

        if (opcion==1){
            system("cls");
            string nombre,correo,carrera;
            int anio;
            cout<<"Nombre completo: ";
            getline(cin,nombre); 
                // getline(cin,var): Es una alternativa a cin>>, solo que con getline obtenemos toda una linea de texto con espacios incluidos.
                // Con cin>> no capturamos los espacios
            cout<<"Correo: ";
            getline(cin,correo);
            cout<<"Carrera profesional: ";
            getline(cin,carrera);
            cout<<"Año de ingreso: ";
            cin>>anio; 
            cin.ignore();
            lista.agregar(nombre,correo,carrera,anio);
        }
        else if(opcion==2){
            system("cls");
            lista.mostrar();
            getch();
        }
        else if(opcion==3){
            system("cls");
            string nombreBuscar;
            cout<<"Nombre a buscar: ";
            getline(cin,nombreBuscar);
            lista.buscarPorNombre(nombreBuscar);
            getch();
        }
        else if(opcion==4){
            system("cls");
            int idBuscar;
            cout<<"ID a buscar: ";
            cin>>idBuscar;
            cin.ignore();
            lista.buscarPorId(idBuscar);
            getch();
        }
        else if(opcion==5){
            system("cls");
            int idBuscar;
            cout<<"ID a modificar: ";
            cin>>idBuscar;
            cin.ignore();
            lista.modificar(idBuscar);
            getch();
        }
        else if(opcion==6){
            system("cls");
            int id;
            cout<<"ID a eliminar: ";
            cin>>id; 
            cin.ignore();
            lista.eliminar(id);
            getch();
        }
        else if(opcion==7){
            system("cls");
            cout<<"Gracias por usar el sistema.\n";
            break;
        }
        else if(opcion==8){
            system("cls");
            lista.recargarDesdeArchivo();
            getch();
        }
        else{
            system("cls");
            cout<<"Opción no válida. Intente nuevamente.\n";
            getch();
        }

    }
    return 0;
}