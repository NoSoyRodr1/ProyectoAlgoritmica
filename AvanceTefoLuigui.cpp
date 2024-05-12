#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Estructura para representar a un usuario
struct Usuario {
    string username;
    string password_hash;
};
// Estructura para los usuarios del personal
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


    //Abrir archivo y guardar las credenciales del personal
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
        cout<<"Bienvenido, cliente. Que accion desea realizar?:\n";
        cout<<"1) Hacer una reserva\n";
        cout<<"2) Hacer un pedido de comida\n";
        cout<<"3) Aumentar el plazo de estancia\n";
        cout<<"4) Retroceder al menu principal\n";
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
        cout<<"Bienvenido, personal del hotel. Que accion desea realizar?:\n";
        cout<<"1) Realizar check-in\n";
        cout<<"2) Realizar check-out\n";
        cout<<"3) Retroceder al menu principal\n";
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
int main() 
{
    UserManager um;
    string username, password;
    string choice;
    string choose;
    do 
    {
        cout<<"Bienvenido a nuestro sitio oficial del Hotel Tal.\n";
        cout<<"Es cliente o es de nuestro personal?\n";
        cout<<"1) Cliente\n";
        cout<<"2) Personal\n";
        cout<<"3) Salir\n";
        cin>>choice;

        if (choice=="1")
        {
            do
            {
                cout<<"1) Iniciar Sesion\n";
                cout<<"2) Registrarse\n";
                cout<<"3) Salir\n";
                cin>>choose;
                if (choose =="1")
                {
                    cout << "Iniciar sesion:\n";
                    cout << "Ingrese nombre de usuario: ";
                    cin >> username;
                    cout << "Ingrese contrasena: ";
                    cin >> password;
                    if (um.iniciarSesion(username, password)) {
                        cout << "Inicio de sesion exitoso." << endl;
                        OpcsDCliente(); // Mostrar opciones para el cliente
                    } else {
                        cout << "Error de inicio de sesion." << endl;
                    }
                }
                else if (choose =="2")
                {
                    cout << "Registrar usuario:\n";
                    cout << "Ingrese nombre de usuario: ";
                    cin >> username;
                    cout << "Ingrese contrasena: ";
                    cin >> password;
                    um.registrarUsuario(username, password);
                }
                else if (choose=="3") 
                {
                    cout<<"\n¡Muchas gracias por visitarnos!\n";
                } 
                else 
                {
                    cout<<"\nInvalido.\n\n";
                }
            }
            while (choose!="3");
            return 0;
        }

        else if (choice=="2") 
        {
            cout << "Iniciar sesion:\n";
            cout << "Ingrese nombre de usuario: ";
            cin >> username;
            cout << "Ingrese contrasena: ";
            cin >> password;
            if (um.iniciarSesionPer(username, password)) {
                cout << "Inicio de sesion exitoso." << endl;
                OpcDPersonal(); // Mostrar opciones para el personal
            } else {
                cout << "Error de inicio de sesion." << endl;
            }
        } 
        else if (choice=="3") 
        {
            cout<<"\n¡Muchas gracias por visitarnos!\n";
        } 
        else 
        {
            cout<<"\nInvalido.\n\n";
        }
    } 
    while (choice!="3");

    return 0;
}