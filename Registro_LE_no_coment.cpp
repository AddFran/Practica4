#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include <conio.h> 
using namespace std;

class Node{
    public:
        int id; 
        string nombre,correo,carrera;
        int anio;
        Node* next; 
        Node* prev; 
        Node(int _id,string _nombre,string _correo,string _carrera,int _anio) : id(_id),nombre(_nombre),correo(_correo),carrera(_carrera),anio(_anio), next(nullptr),prev(nullptr) {}
};

class listaDoble{
    private:
        Node* head; 
        int idActual; 
    public:
        listaDoble(){
            head=nullptr;
            idActual=1;
        }
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
void listaDoble::agregar(string name, string correo, string carrera, int anio){
    Node* nuevo=new Node(idActual++,name,correo,carrera,anio); 
    if(!head){
        head=nuevo;
    }
    else{
        Node* current=head;
        while(current->next){
            current=current->next;
        }
        current->next=nuevo; 
        nuevo->prev=current; 
    }
    guardarEnArchivo(); 
}
void listaDoble::mostrar(){
    Node* current=head; 
    cout<<"\nID\tNombre\t\tCorreo\t\t\tCarrera\t\tAnio\n";
    cout<<"--------------------------------------------------------------\n";
    while(current){
        cout<<current->id<<"\t"<<current->nombre<<"\t\t"<<current->correo<<"\t"<<current->carrera<<"\t"<<current->anio<<endl;
        current=current->next;
    }
    cout<<endl;
}
void listaDoble::buscarPorNombre(string nombreBuscar){
    Node* current=head;
    bool encontrado=false; 
    while(current){
        if(current->nombre==nombreBuscar){ 
            cout<<"\nRegistro encontrado:\n";
            cout<<" ID: "<<current->id<<endl;
            cout<<" Nombre: "<<current->nombre<<endl;
            cout<<" Correo: "<<current->correo<<endl;
            cout<<" Carrera: "<<current->carrera<<endl;
            cout<<" Anio: "<<current->anio<<endl;
            encontrado=true;
            break;
        }
        current=current->next;
    }
    if(!encontrado){
        cout<<"No se encontro el nombre: "<<nombreBuscar<<endl;
    }
}
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
            return;
        }
        current=current->next;
    }
    cout<<"No se encontro algun registro con la ID "<<id_b<<endl;
}
void listaDoble::modificar(int id_b){
    Node* current=head;
    while(current){
        if(current->id==id_b){
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

            current->nombre=nombre;
            current->correo=correo;
            current->carrera=carrera;
            current->anio=anio;

            return;
        }
        current=current->next;
    }
    cout<<"ID no encontrado"<<endl;
}
void listaDoble::eliminar(int idEliminar){
    Node* current=head; 
    while(current){
        if(current->id==idEliminar){ 
            if(current->prev) 
                current->prev->next=current->prev;
            else
                head=current->next;
            if(current->next)
                current->next->prev=current->prev;
            delete current; 
            guardarEnArchivo();
            cout<<"Registro eliminado con exito.\n";
            return; 
        }
        current=current->next;
    }
    cout<<"ID no encontrado.\n"; 
}
void listaDoble::guardarEnArchivo(){
    ofstream archivo("registros_le.txt"); 
    Node* current=head; 
    while(current){ 
        archivo<<current->id<<";"<<current->nombre<<";"<<current->correo<<";"<<current->carrera<<";"<<current->anio<<endl;
        current=current->next; 
    }
    archivo.close(); 
}
void listaDoble::cargarDesdeArchivo(){
    ifstream archivo("registros_le.txt"); 
    string linea;
    while(getline(archivo,linea)){ 
        stringstream ss(linea); 
        string campo; 
        int id,anio;
        string nombre,correo,carrera;
        getline(ss,campo,';');
        id=stoi(campo);
        getline(ss,nombre,';');
        getline(ss,correo,';');
        getline(ss,carrera,';');
        getline(ss,campo,';'); 
        anio=stoi(campo); 
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
    archivo.close();
}
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
        cin.ignore(); 
        if (opcion==1){
            system("cls");
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