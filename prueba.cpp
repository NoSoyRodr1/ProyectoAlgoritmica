
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctime>
#include <iomanip>

using namespace std;

// Declaraciones de funciones
void gotoxy(int x, int y); // Función para mover el cursor a una posición específica en la consola
void cuadro(int x1, int y1, int x2, int y2); // Función para dibujar un cuadro en la consola
void menuprincipal(); // Función para mostrar el menú principal
void menu_cliente(); // Función para mostrar el menú del cliente
void menu_iniciarcliente(); // Función para mostrar el menú de inicio de sesión del cliente
void menu_iniciarregistro(); // Función para mostrar el menú de registro de usuario
void menu_personal(); // Función para mostrar el menú del personal
bool fechaActualPosterior(const string& fecha); // Función para verificar si la fecha actual es posterior a una fecha dada
bool validarFechas(const string& fechaInicio, const string& fechaFin); // Función para validar si las fechas son correctas

// Estructura para representar una habitacion
struct Habitacion {
    string numero;
    bool disponible;
};

// Estructura para representar una reserva
struct Reserva {
    string username;
    string numeroHabitacion;
    string fechaInicio;
    string fechaFin;
};
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
    vector<Reserva> reservas; // Vector para almacenar las reservas
    vector<Habitacion> habitaciones; // Vector para almacenar el estado de las habitaciones
    string habitacionesFile = "habitaciones.csv"; // Nombre del archivo CSV para las habitaciones
    string reservasFile = "reservas.csv"; // Nombre del archivo CSV para las reservas
    string historialFile = "historial.csv"; // Nombre del archivo CSV para el historial

public:
    // Constructor para cargar los usuarios desde el archivo CSV
    UserManager() {
        cargarUsuarios();
        cargarUsuarioPersonal();
        cargarHabitaciones();
        cargarReservas();
    }

// Metodo para obtener las habitaciones disponibles
    vector<Habitacion> getHabitaciones() const {
        return habitaciones;
    }

        // Metodo para obtener las reservas
    vector<Reserva> getReservas() const {
        return reservas;
    }

    vector<Habitacion> getHabitacionesDisponibles() const {
        vector<Habitacion> disponibles;
        for (const auto& habitacion : habitaciones) {
            if (habitacion.disponible) {
                disponibles.push_back(habitacion);
            }
        }
        return disponibles;
    }
        void actualizarDisponibilidadHabitaciones() {
        // Reiniciar la disponibilidad de todas las habitaciones
        for (auto& habitacion : habitaciones) {
            habitacion.disponible = true;
        }

        // Marcar como no disponibles las habitaciones reservadas
        for (const auto& reserva : reservas) {
            for (auto& habitacion : habitaciones) {
                if (habitacion.numero == reserva.numeroHabitacion) {
                    habitacion.disponible = false;
                    break;
                }
            }
        }
    }

    void moverReservaAHistorial(const Reserva& reserva) {
        // Mueve la reserva al archivo de historial
        ofstream file(historialFile, ios::app);
        if (file.is_open()) {
            file << reserva.username << "," << reserva.numeroHabitacion << "," << reserva.fechaInicio << "," << reserva.fechaFin << endl;
            file.close();
        }

        // Elimina la reserva del archivo de reservas
        vector<Reserva> nuevasReservas;
        for (const auto& res : reservas) {
            if (!(res.username == reserva.username && res.numeroHabitacion == reserva.numeroHabitacion &&
                  res.fechaInicio == reserva.fechaInicio && res.fechaFin == reserva.fechaFin)) {
                nuevasReservas.push_back(res);
            }
        }

        // Abre el archivo de reservas en modo de escritura para sobrescribir el contenido
        ofstream reservaFile(reservasFile, ios::out);
        if (reservaFile.is_open()) {
            // Escribe todas las reservas restantes en el archivo de reservas
            for (const auto& res : nuevasReservas) {
                reservaFile << res.username << "," << res.numeroHabitacion << "," << res.fechaInicio << "," << res.fechaFin << endl;
            }
            reservaFile.close();
        }

        // Actualiza la lista de reservas en el UserManager
        reservas = nuevasReservas;
    }

    void actualizarReservas() {
        // Actualiza las reservas verificando si alguna ha finalizado
        vector<Reserva> nuevasReservas;
        for (auto& reserva : reservas) {
            if (fechaActualPosterior(reserva.fechaFin)) {
                moverReservaAHistorial(reserva);
            } else {
                nuevasReservas.push_back(reserva);
            }
        }
        reservas = nuevasReservas;
    }

    // Metodo para cargar habitaciones y reservas desde archivos CSV
    void cargarHabitaciones() {
        ifstream file(habitacionesFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de habitaciones." << endl;
            return;
        }
        string line, numero;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, numero, ',');
            // Supongamos que todas las habitaciones estan disponibles al inicio
            habitaciones.push_back({numero, true});
        }
        file.close();
    }

    void cargarReservas() {
        ifstream file(reservasFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de reservas." << endl;
            return;
        }
        string line, username, numeroHabitacion, fechaInicio, fechaFin;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, username, ',');
            getline(ss, numeroHabitacion, ',');
            getline(ss, fechaInicio, ',');
            getline(ss, fechaFin);
            reservas.push_back({username, numeroHabitacion, fechaInicio, fechaFin});
            // Marcar la habitacion como no disponible durante el tiempo de la reserva
            marcarHabitacionNoDisponible(numeroHabitacion);
        }
        file.close();
    }

    // Metodo para marcar una habitacion como no disponible durante el tiempo de la reserva
    void marcarHabitacionNoDisponible(const string& numeroHabitacion) {
        for (auto& habitacion : habitaciones) {
            if (habitacion.numero == numeroHabitacion) {
                habitacion.disponible = false;
                break;
            }
        }
    }

    // Metodo para registrar una reserva
    void registrarReserva(const string& username, const string& numeroHabitacion, const string& fechaInicio, const string& fechaFin) {
        reservas.push_back({username, numeroHabitacion, fechaInicio, fechaFin});
        // Marcar la habitacion como no disponible durante el tiempo de la reserva
        marcarHabitacionNoDisponible(numeroHabitacion);
        // Actualizar la disponibilidad de las habitaciones
        actualizarDisponibilidadHabitaciones();
        // Guardar la reserva en el archivo CSV
        ofstream file(reservasFile, ios::app);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de reservas." << endl;
            return;
        }
        file << username << "," << numeroHabitacion << "," << fechaInicio << "," << fechaFin << endl;
        file.close();
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
        cout << "\n\n\n\n\t\t\t\tUsuario ya existe." << endl;
        cout<<"\t\t\tPulse cualquier tecla para regresar..."<< endl;
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

    // Mensaje de registro exitoso
    cout << "\n\n\n\n\tRegistro exitoso. Usuario: " << username << " ha sido registrado." << endl;
    cout << "\t\t\tPulse enter para continuar...";
    cin.get(); // Esperar a que el usuario presione enter
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
void OpcsDCliente(UserManager& um, const string& username) {
    string opt;
    do {
        system("cls");
        um.actualizarReservas();
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30,1);cout<<"\n\t\tBienvenido " << username << ". Que accion desea realizar?:\n"<< endl;
        gotoxy(2, 4);
        cout << "\n\t1) Ver habitaciones disponibles\n";
        cout << "\n\t2) Hacer una reserva\n";
        cout << "\n\t3) Aumentar el plazo de estancia\n";
        cout << "\n\t4) Retroceder al menu principal\n";
        cout << "\n\tOpcion: ";
        cin >> opt;
        um.cargarReservas();
        if (opt == "1") {
            // Mostrar habitaciones disponibles
            system("cls");
            cuadro(0, 0, 80, 24);
            gotoxy(30, 1);
            cout << "\n\t\tHABITACIONES DISPONIBLES\n\n";
            // Obtener las habitaciones disponibles actualizadas
            auto habitacionesDisponibles = um.getHabitacionesDisponibles();
            int count = 0;
            for (const auto& habitacion : habitacionesDisponibles) {
                cout << habitacion.numero << "\t";
                count++;
                if (count % 10 == 0) {
                    cout << endl;
                }
            }
            cout << endl << endl << "Presione cualquier tecla para volver al menú..." << endl;
            system("pause>nul");
        } else if (opt == "2") {
            // Realizar una reserva
            string numeroHabitacion, fechaInicio, fechaFin;
            cout << "\nIngrese el numero de habitacion que desea reservar: ";
            cin >> numeroHabitacion;

            // Verificar si la habitacion esta disponible
            bool habitacionDisponible = false;
            for (const auto& habitacion : um.getHabitaciones()) {
                if (habitacion.numero == numeroHabitacion && habitacion.disponible) {
                    habitacionDisponible = true;
                    break;
                }
            }

            if (!habitacionDisponible) {
                cout << "\nLa habitacion ingresada no existe o no esta disponible. Por favor, intente de nuevo." << endl;
            } else {
                // Solicitar la fecha de inicio de la reserva
                cout << "\nIngrese la fecha de inicio de la reserva (YYYY-MM-DD): ";
                cin >> fechaInicio;
                
                // Verificar si la fecha de inicio es anterior a la fecha actual
                if (fechaActualPosterior(fechaInicio)) {
                    cout << "\nLa fecha de inicio de la reserva no puede ser anterior a la fecha actual. Por favor, intente de nuevo." << endl;
                    continue;  // Volver al inicio del bucle
                }

                // Solicitar la fecha de fin de la reserva
                cout << "\nIngrese la fecha de fin de la reserva (YYYY-MM-DD): ";
                cin >> fechaFin;

                // Verificar si la fecha de fin es posterior a la fecha de inicio
                if (!validarFechas(fechaInicio, fechaFin)) {
                    cout << "\nLa fecha de fin de la reserva debe ser posterior a la fecha de inicio. Por favor, intente de nuevo." << endl;
                    continue;  // Volver al inicio del bucle
                }

                // Realizar la reserva
                um.registrarReserva(username, numeroHabitacion, fechaInicio, fechaFin);
                cout << "\nReserva realizada con exito." << endl;
            }

            cout << "\nPresione cualquier tecla para volver al menu..." << endl;
            system("pause>nul");
        } else if (opt == "3") {
            // Acciones
            cout << "...\n";
        } else if (opt == "4") {
            cout << "\nVolviendo al menu principal...\n\n";
            // Regresar al menu principal
        } else {
            cout << "\nOpcion invalida.\n\n";
        }
    } while (opt != "4");
}

// Lógica de las acciones que puede realizar el personal
void OpcDPersonal(string &username) {
    bool repite =true;  // lo mismo que el anterior,ahora esta en funcion switch y en cada case abria una opcion
    int opcion;
    do {
        system("cls");
        cuadro(0,0,80,24);
        cuadro(1,1,77,3);
        gotoxy(30,1);cout<<"\n\tBienvenido al trabajo " << username << ". Que accion desea realizar?:\n"<<endl;
        gotoxy(2,4);
        cout<<"\n\t1) Realizar check-in\n";
        cout<<"\n\t2) Realizar check-out\n";
        cout<<"\n\t3) Retroceder al menu principal\n";
        cin>>opcion;

        switch(opcion){
            case 1:
                cout<<"...\n";
            break;

            case 2:
                cout<<"...\n";
            break;

            case 3:
                repite = false;
                cout<<"\n\n\n\tTendra que volver a ingresar,pulse cualquier letra para continuar";
                break;
        }  
    }while (repite);
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
        gotoxy(30,1);cout<<"\n\t\t\t\tMENU DEL CLIENTE\n";
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
    gotoxy(30,1);cout << "\n\t\t\t\tINICIAR SESION\n";
    gotoxy(2,4);
    cout << "\n\tIngrese nombre de usuario: ";
    cin >> username;
    cout << "\n\tIngrese contrasena: ";
    cin >> password;
    if (um.iniciarSesion(username, password)) {
        cout << "Inicio de sesion exitoso." << endl;
        OpcsDCliente(um, username); // Mostrar opciones para el cliente
    } else {
         cout << "\n\n\n\n\t\t\tError de inicio de sesion." << endl;
         cout<<"\t\t\tPulse cualquier tecla para regresar..."<< endl;
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
    gotoxy(30,1);cout << "\n\t\t\t\tREGISTRAR USUARIO:\n";
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
    gotoxy(30,1);cout<<"\n\t\t\t\tMENU DEL PERSONAL\n";
    gotoxy(2,4);
    cout << "\n\tIniciar sesion:\n";
    cout << "\n\tIngrese nombre de usuario: ";
    cin >> username;
    cout << "\n\tIngrese contrasena: ";
    cin >> password;
    if (um.iniciarSesionPer(username, password)) {
        cout << "\n\tInicio de sesion exitoso." << endl;
        OpcDPersonal(username); // Mostrar opciones para el personal
    } else {
        cout << "\n\n\n\n\t\t\tError de inicio de sesion." << endl;
        cout<<"\t\t\tPulse cualquier tecla para regresar..."<< endl;
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
bool fechaActualPosterior(const string& fechaInicio) {
    // Obtener la fecha y hora actual
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int year_now = 1900 + ltm->tm_year;
    int month_now = 1 + ltm->tm_mon;
    int day_now = ltm->tm_mday;

    // Separar la fecha de inicio en año, mes y día
    stringstream ss(fechaInicio);
    string year_inicio, month_inicio, day_inicio;
    getline(ss, year_inicio, '-');
    getline(ss, month_inicio, '-');
    getline(ss, day_inicio, '-');

    int year_ini = stoi(year_inicio);
    int month_ini = stoi(month_inicio);
    int day_ini = stoi(day_inicio);

    // Comparar las fechas
    if (year_ini > year_now || (year_ini == year_now && month_ini > month_now) || (year_ini == year_now && month_ini == month_now && day_ini > day_now)) {
        return false;
    }
    return true;
}

bool validarFechas(const string& fechaInicio, const string& fechaFin) {
    // Separar la fecha de inicio en año, mes y día
    stringstream ssInicio(fechaInicio);
    string year_inicio, month_inicio, day_inicio;
    getline(ssInicio, year_inicio, '-');
    getline(ssInicio, month_inicio, '-');
    getline(ssInicio, day_inicio, '-');

    int year_ini = stoi(year_inicio);
    int month_ini = stoi(month_inicio);
    int day_ini = stoi(day_inicio);

    // Separar la fecha de fin en año, mes y día
    stringstream ssFin(fechaFin);
    string year_fin, month_fin, day_fin;
    getline(ssFin, year_fin, '-');
    getline(ssFin, month_fin, '-');
    getline(ssFin, day_fin, '-');

    int year_f = stoi(year_fin);
    int month_f = stoi(month_fin);
    int day_f = stoi(day_fin);

    // Comparar las fechas
    if (year_f > year_ini || (year_f == year_ini && month_f > month_ini) || (year_f == year_ini && month_f == month_ini && day_f >= day_ini)) {
        return true;
    }
    return false;
}

bool reservaHaFinalizado(const string& fechaFin) {
    // Obtener la fecha y hora actual
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int year_now = 1900 + ltm->tm_year;
    int month_now = 1 + ltm->tm_mon;
    int day_now = ltm->tm_mday;

    // Separar la fecha de fin en año, mes y día
    stringstream ss(fechaFin);
    string year_fin, month_fin, day_fin;
    getline(ss, year_fin, '-');
    getline(ss, month_fin, '-');
    getline(ss, day_fin);

    // Convertir los strings a enteros
    int year_fin_int = stoi(year_fin);
    int month_fin_int = stoi(month_fin);
    int day_fin_int = stoi(day_fin);

    // Verificar si la fecha de fin ha pasado
    if (year_now > year_fin_int) {
        return true;
    } else if (year_now == year_fin_int) {
        if (month_now > month_fin_int) {
            return true;
        } else if (month_now == month_fin_int) {
            if (day_now > day_fin_int) {
                return true;
            }
        }
    }
    return false;
}
