#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;
void gotoxy(int x,int y);
void cuadro(int x1, int y1,int x2, int y2);
void menuprincipal();
void menu_cliente();
void menu_iniciarcliente();
void menu_iniciarregistro();
void menu_personal();


// Estructura para representar a un usuario
struct Usuario {
    string username;
    string password_hash;
};
struct UsuarioPersonal{
    string usernamePersonal;
    string passwordPersonal;
};

// Clase para manejar la gestión de usuarios
class UserManager {
private:
    vector<Usuario> usuarios; // Vector para almacenar los usuarios
    string filename = "usuarios.csv"; // Nombre del archivo CSV
    string filepersonal= "personal.csv";//Nombre del archivo csv para el personal
    vector<UsuarioPersonal> usuariosPer;//Vector para almacenar los usuarios del personal

public:
    // Constructor para cargar los usuarios desde el archivo CSV
    UserManager() {
        cargarUsuarios();
        cargarUsuarioPersonal();
    }

    // Método para cargar los usuarios desde el archivo CSV
    void cargarUsuarios() {
        ifstream file(filename); // Abrir el archivo
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de usuarios." << endl;
            return;
        }
        string line, usr, pwd; 
        // Leer cada línea del archivo
        while (getline(file, line)) {
            stringstream ss(line);
            // Obtener el nombre de usuario y la contraseña de la línea
            getline(ss, usr, ','); // Lee hasta la coma
            getline(ss, pwd);      // Lee el resto de la línea
            // Agregar el usuario al vector
            usuarios.push_back({usr, pwd});
        }
        file.close(); // Cerrar el archivo
    }

    // Método para registrar a un nuevo usuario
    void registrarUsuario(const string &username, const string &password) {
        // Verificar si el usuario ya existe
        if (usuarioExiste(username)) {
            cout << "Usuario ya existe." << endl;
            return;
        }
        // Simulación de hash de contraseña
        string hashed_password = to_string(hash<string>{}(password));  
        // Agregar el nuevo usuario al vector
        usuarios.push_back({username, hashed_password});
        // Abrir el archivo en modo de append
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de usuarios." << endl;
            return;
        }
        // Escribir los datos del nuevo usuario en el archivo CSV
        file << username << "," << hashed_password << endl;  
        file.close(); // Cerrar el archivo
    }

    // Método para verificar si el usuario ya existe
    bool usuarioExiste(const string &username) {
        // Iterar sobre los usuarios y verificar si el nombre de usuario ya está en uso
        return any_of(usuarios.begin(), usuarios.end(), [&](const Usuario &u) {
            return u.username == username;
        });
    }

    // Método para iniciar sesión
    bool iniciarSesion(const string &username, const string &password) {
        // Simulación de hash de contraseña
        string hashed_password = to_string(hash<string>{}(password));
        // Iterar sobre los usuarios y verificar si las credenciales son válidas
        for (const auto &user : usuarios) {
            if (user.username == username && user.password_hash == hashed_password) {
                return true; // Las credenciales son válidas
            }
        }
        return false; // Las credenciales son inválidas
    }
    void cargarUsuarioPersonal(){
        ifstream archivo(filepersonal);
        if(!archivo.is_open()){
            cout<<"Error al abrir el archivo"<<endl;
            return;
        }

        string linea;
        while(getline(archivo, linea)){
            stringstream ss(linea);
            string usuarioPer,passwordPer;
            getline(ss,usuarioPer,',');
            getline(ss,passwordPer,',');
            usuariosPer.push_back({usuarioPer, passwordPer});
        }
        archivo.close();
    }
    bool iniciarSesionPer(const string &username, const string &password) {
        // Iterar sobre los usuarios y verificar si las credenciales son válidas
        for (const auto &user : usuariosPer) {
            if (user.usernamePersonal == username && user.passwordPersonal == password) {
                return true; // Las credenciales son válidas
            }
        }
        return false; // Las credenciales son inválidas
    }


};

// Lógica de las acciones que puede realizar el cliente
void OpcsDCliente() 
{
    string opt;
    do 
    {   
        system("cls");
        cuadro(0,0,80,24);
        cuadro(1,1,77,3);
        gotoxy(30,1);cout<<"\n\t\tBienvenido, cliente. Que accion desea realizar?:\n";
        gotoxy(2,4);
        cout<<"\n\t1) Hacer una reserva\n";
        cout<<"\n\t2) Hacer un pedido de comida\n";
        cout<<"\n\t3) Aumentar el plazo de estancia\n";
        cout<<"\n\t4) Retroceder al menu principal\n";
        cout<< "\n\tOpcion: ";
        cin>>opt;

        if (opt=="1") 
        {
            // Acciones
            cout<<"...\n";
        } 
        else if (opt=="2") 
        {
            // Acciones
            cout<<"...\n";
        } 
        else if (opt=="3") 
        {
            // Acciones
            cout<<"...\n";
        } 
        else if (opt=="4") 
        {
            cout<<"\nVolviendo al menu principal...\n\n";
            // Regresa al menú principal
        } 
        else 
        {
            cout<<"\nOpcion invalida.\n\n";
        }
    } 
    while (opt!="4");
}

// Lógica de las acciones que puede realizar el personal
void OpcDPersonal() 
{
    string opt;
    do 
    {
        system("cls");
        cout<<"\n\t\t\tBienvenido, personal del hotel. Que accion desea realizar?:\n";
        cout<<"\n\t1) Realizar check-in\n";
        cout<<"\n\t2) Realizar check-out\n";
        cout<<"\n\t3) Retroceder al menu principal\n";
        cin>>opt;

        if (opt=="1") 
        {
            // Acciones
            cout<<"...\n";
        } 
        else if (opt=="2") 
        {
            // Acciones
            cout<<"...\n";
        } 
        else if (opt=="3") 
        {
            cout<<"\nVolviendo al menu principal...\n\n";
            // Regresa al menú principal
        } 
        else 
        {
            cout<<"\nOpcion invalida.\n\n";
        }
    } 
    while (opt!="3");
}

// Función principal del programa
int main() {
    system("COLOR E0");
    cuadro(0,0,80,24);
    UserManager um;
    string username, password;
    string choice;
    string choose;
    menuprincipal();
    cin.get();
    cin.get();
    return 0;
 }

void menuprincipal(){
    bool repite = true;
    int opcion;
    do{
        system("cls");
        cuadro(0,0,80,24);
        cuadro(1,1,77,3);
        gotoxy(30,1);cout<<"\n\t\t\tMENU PRINCIPAL DEL HOTEL\n";
        gotoxy(2,4);
        //Las opciones del menú
        cout<<"\n\tBienvenido a nuestro sitio oficial del Hotel Tal.\n";
        cout<<"\n\tEs cliente o es de nuestro personal?\n";
        cout<<"\n\t1) Cliente\n";
        cout<<"\n\t2) Personal\n";
        cout<<"\n\t0) Salir\n";
        cout<< "\n\tOpcion: ";
        cin>>opcion;
        switch(opcion){
            case 1:
                menu_cliente();
                break;

            case 2:
                menu_personal();
                break;

            case 0:
                repite = false;
                break;
        }
    }while (repite);
}

void menu_cliente(){
    bool repite = true;
    int opcion;
    do{
        system("cls");
        cuadro(0,0,80,24);
        cuadro(1,1,77,3);
        // El título del menú
        gotoxy(30,1);cout<<"\n\t\t\tMENU DEL CLIENTE\n";
        gotoxy(2,4);
        cout<<"\n\t1) Iniciar Sesion\n";
        cout<<"\n\t2) Registrarse\n";
        cout<<"\n\t3) Salir\n";
        cout<< "\n\tOpcion: ";
        cin>>opcion;
    switch(opcion){
            case 1:
                menu_iniciarcliente();
                break;
            case 2:
                menu_iniciarregistro();
            case 3:
                repite = false;
                break;
    }

    }while(repite);
}

void menu_iniciarcliente(){
    system("cls");
    UserManager um;
    string username;
    string password;
    cuadro(0,0,80,24);
    cuadro(1,1,77,3);
    gotoxy(30,1);cout << "\n\t\tINICIAR SESION\n";
    gotoxy(2,4);
    cout << "\n\tIngrese nombre de usuario: ";
    cin >> username;
    cout << "\n\tIngrese contrasena: ";
    cin >> password;
    if (um.iniciarSesion(username, password)) {
        cout << "Inicio de sesion exitoso." << endl;
        OpcsDCliente(); // Mostrar opciones para el cliente
    } else {
         cout << "Error de inicio de sesion." << endl;
    }
    system("pause>nul");

}

void menu_iniciarregistro(){
    system("cls");
    UserManager um;
    string username;
    string password;
    cuadro(0,0,80,24);
    cuadro(1,1,77,3);
    gotoxy(30,1);cout << "\n\t\tREGISTRAR USUARIO:\n";
    gotoxy(2,4);
    cout << "\n\tIngrese nombre de usuario: ";
    cin >> username;
    cout << "\n\tIngrese contrasena: ";
    cin >> password;
    um.registrarUsuario(username, password);
    system("pause>nul");
}

void menu_personal(){
    system("cls");
    UserManager um;
    string username;
    string password;
    system("cls");
        // El título del menú
    cuadro(0,0,80,24);
    cuadro(1,1,77,3);
    gotoxy(30,1);cout<<"\n\t\t\tMENU DEL PERSONAL\n";
    gotoxy(2,4);
    cout << "\n\tIniciar sesion:\n";
    cout << "\n\tIngrese nombre de usuario: ";
    cin >> username;
    cout << "\n\tIngrese contrasena: ";
    cin >> password;
    if (um.iniciarSesionPer(username, password)) {
        cout << "\n\tInicio de sesion exitoso." << endl;
        OpcDPersonal(); // Mostrar opciones para el personal
    } else {
        cout << "\n\tError de inicio de sesion." << endl;
    }
    system("pause>nul");
}
void cuadro(int x1, int y1,int x2, int y2){
    int i;
    for (i=x1;i<x2;i++){
        gotoxy(i,y1);printf("-");
        gotoxy(i,y2);printf("-");
    }

    for (i=y1;i<y2;i++){
        gotoxy(x1,i);printf("|");
        gotoxy(x2,i);printf("|");
    }

    gotoxy(x1,y1); printf("Ú");
    gotoxy(x1,y2); printf("À");
    gotoxy(x2,y1); printf("¿");
    gotoxy(x1,y2); printf("Ù");
}

void gotoxy(int x,int y){
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon,dwPos);
}