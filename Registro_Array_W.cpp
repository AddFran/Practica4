#include <windows.h> // Libreria que da acceso a la api de windows, permitiendo trabajar con, por ejemplo, ventanas e interfaces
#include <fstream> // Libreria para trabajar con archivos
#include <sstream> // Libreria para manipular cadenas de texto (stringstream)
#include <string> // Libreria para trabajar con cadenas de texto
#include <vector> // Libreria para trabajar con arrays
using namespace std;

// Definimos un struct para almacenar la info de cada estudiante del array
struct Estudiante{
    string nombre;
    string correo;
    string carrera;
    int anio;
};

// Creamos un array de "Estudiante"
vector<Estudiante> lista; // En este caso usaremos arrays en lugar de listas enlazadas

// HWND: Es un identificador para una ventana, en este caso para mostrar las casillas de cada elemento
HWND hNombre, hCorreo, hCarrera, hAnio, hLista;

// Funcion para agregar estudiantes al sistema
void AgregarEstudiante(HWND hwnd) { // Pasamos como parametro un identificar de ventana
    char nombre[100], correo[100], carrera[100], anioStr[10]; // Creamos cadenas de caaracteres para cada dato
    // GetWindowText: Funcion que obtiene el texto de una cadena y lo copia a una variable (buffer)
    GetWindowText(hNombre, nombre, 100); // Obtiene el texto de ingresado en la ventana "hNombre", lo copia a "nombre" y delimita el maximo de caracteres a 100
    GetWindowText(hCorreo, correo, 100); 
    GetWindowText(hCarrera, carrera, 100);
    GetWindowText(hAnio, anioStr, 10);
    int anio = atoi(anioStr); // atoi: Funcion que convierte una serie de caracteres a entero
    Estudiante e = { nombre, correo, carrera, anio }; // Creamos una instancia de la estructura con los datos obtenidos por GetWindowText
    lista.push_back(e); // Insertamos la instancia creada en el array
    // SendMessage: Funcion que envia un mensaje a una ventana o control, en este caso se lo estamos enviando a "hLista"
    SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)(string(nombre) + " - " + correo).c_str());
    /* 
        LB_ADDSTRING: Indica que agregaremos una cadena de texto a la ventana
        0: wPram, no se usa, por eso el 0
            WPARAM (Word Parameter): Es un parametro usado en el manejo de mensajes de ventana, usado para enviar datos simples como un entero
        (LPARAM)(string(nombre) + " - " + correo).c_str(): Se forma el texto a mostrar.
            (string(nombre) + " - " + correo): Formamos el texto que enviaremos
            .c_str(): Convertimos el texto formado de string a un puntero const char
            (LPARAM): Enviamos la el texto
                LPARAM (Long Parameter): De forma similar a WPARAM es usado para enviar datos, pero mas complejos (en este caso un puntero)

            ¿Para que se utilizan WPARAN y LPARAM?
            Porque el sistema de mensajería de Windows necesita una forma flexible de pasar 
            información variada (números, punteros, estructuras, etc.) sin cambiar el prototipo 
            de la función SendMessage para cada tipo de mensaje.
    */
    // SetWindowText: Funcion que cambia el mensaje mostrado por una pantalla
    SetWindowText(hNombre, ""); // En este caso estamos diciendo que cambiaremos el texto mostrado por medio del identificados "hNombre" a "", osea nada
    SetWindowText(hCorreo, "");
    SetWindowText(hCarrera, "");
    SetWindowText(hAnio, "");
}
// Funcion para guardar el registro en un archivo .txt
void GuardarArchivo() {
    ofstream file("registros_a.txt"); // Creamos (a sobreescribimos) un archivo .txt
    // Usamos un foreach para crear el archivo
    for(auto& e : lista){ // Creamos una variable automatica "e" que sera una referencia a cada elemento de lista 
        file<<e.nombre<<";"<<e.correo<<";"<<e.carrera<<";"<<e.anio<<"\n"; // Escribimos en cada linea del archivo .txt, accediendo a los datos de cada elemento del array con "e"
    }
    file.close(); // Cerramos el archivo
}
// Funcion para cargar el archivo .txt
void CargarArchivo(HWND hwnd){ // Pasamos como parametro
    lista.clear(); // Funcion para limpiar el array
    SendMessage(hLista, LB_RESETCONTENT, 0, 0); // Enviamos un mensaje a la ventana hLista, LB_RESENTCONTENT que limipia todos los elementos de la ventana hLista
    ifstream file("registros_a.txt"); // Abrimos el archivo .txt
    string linea; // Creamos una variable string para guardar toda una linea del archivo
    while(getline(file, linea)){ // Por medio de while y getline obtenemos toda una linea del archivo .txt, en cada iteracion cambia de linea
        stringstream ss(linea); // Guardamos el texto de la linea extraida en ss, por medio de stringstream podremos extraer pedazos de la linea
        string nombre, correo, carrera, anioStr; // Variables auxiliares
        getline(ss, nombre, ';'); // Obtenemos el texto guardado en ss  hasta que aparezca ";", y lo almacenamos en la variable "nombre"
        getline(ss, correo, ';');
        getline(ss, carrera, ';');
        getline(ss, anioStr, ';');
        int anio=stoi(anioStr); // Transformamos "anioStr" de string a entero y lo guardamos en la variable "anio"
        Estudiante e = { nombre, correo, carrera, anio }; // Creamos una instancia de la estructura y le asignamos los datos obtenidos
        lista.push_back(e); // Insetamos la instancia al array
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)(nombre + " - " + correo).c_str()); // Actualizamos la ventana hLista, de la misma forma que en la funcion AgregarEstudiante()
    }
    file.close(); // Cerramos el archivo .txt
}
// Funcion vital, es el "procedimiento de ventana", la función que maneja todos los eventos o mensajes que recibe la ventana
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){ // Identificador de ventana, mensaje que llega, parametro simple, parametro complejo
    switch(msg){ // Dependiendo del mensaje que llega...
        case WM_COMMAND: // En caso que se WM_COMMAND, osea, el usuario haya hecho algo (comando, mensaje de notificacion o pulsar algo) en este caso pulsar un boton
            switch (wp) { // DEpendiendo del tipo de WPARAM enviado...
                case 1: AgregarEstudiante(hwnd); break; // Agregamos estudiante
                case 2: GuardarArchivo(); break; // Guardamos el cambio
                case 3: CargarArchivo(hwnd); break; // Cargamos el archivo
            }
            break;
        case WM_DESTROY: // WM_DESTROY: Se envía cuando se destruye una ventana.
            PostQuitMessage(0); break; // Indica al sistema que un hilo ha solicitado su terminación (salida).
        default:
            return DefWindowProc(hwnd, msg, wp, lp); // Se encarga de los comportamientos por defecto de Windows
    }
    return 0;
}

void CrearControles(HWND hwnd) {
    // Creamos las ventanas de texto...
    
    CreateWindow("STATIC", "Nombre:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, NULL, NULL);
    // (texto fijo, texto que aparece, esta sera una ventana visible | secundaria, posicion (x=20, y=20) y tamanio (80x20), ventana padre (donde se colocara), datos que no usaremos)
    hNombre = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 200, 20, hwnd, NULL, NULL, NULL);
    // guardamos en hNombre para trabajar despues (texto editable, no tiene label, esta sera una ventana visible | secundaria | con borde, posicion (x=100, y=20) y tamanio (200x20), ventana padr e, datos que no usaremos)

    // Lo anterior tambien se aplica a estos
    CreateWindow("STATIC", "Correo:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
    hCorreo = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 200, 20, hwnd, NULL, NULL, NULL);

    CreateWindow("STATIC", "Carrera:", WS_VISIBLE | WS_CHILD, 20, 80, 80, 20, hwnd, NULL, NULL, NULL);
    hCarrera = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 80, 200, 20, hwnd, NULL, NULL, NULL);

    CreateWindow("STATIC", "Anio:", WS_VISIBLE | WS_CHILD, 20, 110, 80, 20, hwnd, NULL, NULL, NULL);
    hAnio = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 110, 100, 20, hwnd, NULL, NULL, NULL);

    // Creamos los botones...

    CreateWindow("BUTTON", "Agregar", WS_VISIBLE | WS_CHILD, 320, 20, 100, 30, hwnd, (HMENU)1, NULL, NULL);
    // (boton, texto que aparece en el boton, es visible | secundario, posicion (x=320, y=20) y tamanio (100x30), ventana padre, ID del boton 1 (usado en el switch de WindowProcedure), cosas no usadas)
    CreateWindow("BUTTON", "Guardar", WS_VISIBLE | WS_CHILD, 320, 60, 100, 30, hwnd, (HMENU)2, NULL, NULL);
    CreateWindow("BUTTON", "Cargar", WS_VISIBLE | WS_CHILD, 320, 100, 100, 30, hwnd, (HMENU)3, NULL, NULL);

    hLista = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY, 20, 150, 400, 200, hwnd, NULL, NULL, NULL);
    // guardamos en hLista para trabajar despues (lista desplegada, sin texto, es visible | secundario | tiene borde | envia una notificacion, posicion (x=20, y=150) y tamanio (400x200), ventana padre, cosas no usadas)
}
// Funcion main del programa, solo que orientado a aplicacion como esta
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int nCmdShow){
    // Definimos la una instancia de la clase "WINDCLASS" para la ventana
    WNDCLASS wc={0};
    // Establecemos sus caracteristicas
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // Color de fondo
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Forma del cursos
    wc.hInstance = hInst; // Instancia
    wc.lpszClassName = "RegistroWin"; // Nombre de la clase, para mas tarde
    wc.lpfnWndProc = WindowProcedure; // Funcion para manejar los botones (la funcion de arriba)

    // Registramos la clase, en caso que falle cierra el programa
    if (!RegisterClass(&wc)) return -1;

    // Creamos la ventana
    HWND hwnd = CreateWindow("RegistroWin", "Sistema de Registro Académico", // Pasamos el nombre de la clase y el titulo de la ventana
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Indicamos que sera una ventana normal (bordes, titulo, botones de cerrar, etc) y sera visible
                             100, 100, 470, 420, NULL, NULL, NULL, NULL); // Posicion (x=100, y=100) y tamanio (470x420)
    
    // Creamos las casillas de texto y botones con la funcion CrearControles
    CrearControles(hwnd);

    
    // Creamos el bucle para que nuestra aplicacion no se cierre
    MSG msg = { 0 }; // Instancia de la clase MSG para poder enviar info de esta parte
    while (GetMessage(&msg, NULL, 0, 0)) { // GetMessage: espera un mensaje, como un click o apretar un boton
        TranslateMessage(&msg); // Convierte los inputs (teclado o raton) a caracteres legibles
        DispatchMessage(&msg); // Envia los datos al programa principal (WindowsProcedure)
    }
    return 0;
}

/* 
Recursos utiles 
    - Para LB_?: https://learn.microsoft.com/es-es/windows/win32/controls/lb-addstring
    - Para WM_?: https://learn.microsoft.com/es-es/windows/win32/winmsg/wm-destroy
                 https://learn.microsoft.com/es-es/windows/win32/menurc/wm-command
    - Para ventanas WS_?: https://learn.microsoft.com/es-es/windows/win32/winmsg/window-styles
    - Para lista LBS_?: https://learn.microsoft.com/en-us/windows/win32/controls/list-box-styles
*/