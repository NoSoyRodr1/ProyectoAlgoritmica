#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include <set>
#include <ctime>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define ENTER 13
using namespace std;

// Declaraciones de funciones
void gotoxy(int x, int y);
void cuadro(int x1, int y1, int x2, int y2);
int menu(const char *titulo, const char *opciones[],int m); //Con este menu ya no sera necesario poner el titulo y sub mas dificil
void menuprincipal();
void menu_cliente();
void menuingresopersonal();
void menu_iniciarcliente();
void menu_iniciarregistro();
void menu_personal();
void menu_comida();
void menu_desayuno();
void menu_almuerzo();
void menu_cena();
void hacerReserva(const string& username);
void verReservas();
void cancelarReserva(const string& username);
void mostrarHabitacionesDisponiblesPorTipo(const string& tipo);
bool esFechaValida(const string& fecha);



// Estructuras para representar usuario, usuario del personal, habitaciÃƒÂ³n y reserva
struct Usuario {
    string username;
    string password_hash;
};

struct UsuarioPersonal {
    string usernamePersonal;
    string passwordPersonal;
};

struct Habitacion {
    int id;
    string tipo;
    double precio;
    bool disponible;
};

struct Reserva {
    int id;
    string username;
    int habitacionId;
    string fechaInicio;
    string fechaFin;
};

// Clase para manejar la gestiÃƒÂ³n de usuarios
class UserManager {
private:
    vector<Usuario> usuarios;
    vector<UsuarioPersonal> usuariosPer;
    vector<Habitacion> habitaciones;
    vector<Reserva> reservas;
    string filename = "usuarios.csv";
    string filepersonal = "personal.csv";
    string habitacionesFile = "habitaciones.csv";
    string reservasFile = "reservas.csv";
    string historialFile = "historial.csv";
    int nextReservaId = 1;

public:
    UserManager() {
        cargarUsuarios();
        cargarUsuarioPersonal();
        cargarHabitaciones();
        cargarReservas();
    };

    void cargarUsuarios() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de usuarios." << endl;
            return;
        }
        string line, usr, pwd;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, usr, ',');
            getline(ss, pwd);
            usuarios.push_back({usr, pwd});
        }
        file.close();
    }

bool esFechaValida(const string& fecha) {
    // Verificar que la fecha tenga el formato correcto DD/MM/YYYY
    if (fecha.size() != 10 || fecha[2] != '/' || fecha[5] != '/') {
        return false;
    }
    // Extraer dÃƒÂ­a, mes y aÃƒÂ±o
    stringstream ss(fecha);
    int dia, mes, anio;
    char delim;
    ss >> dia >> delim >> mes >> delim >> anio;
    // Verificar si se pudieron extraer correctamente
    if (ss.fail()) {
        return false;
    }
    // Verificar si el aÃƒÂ±o es vÃƒÂ¡lido
    if (anio < 2023) {
        return false;
    }
    // Verificar si el mes es vÃƒÂ¡lido
    if (mes < 1 || mes > 12) {
        return false;
    }
    // Verificar los dÃƒÂ­as dependiendo del mes y si es aÃƒÂ±o bisiesto o no
    int diasEnMes[] = {31, 28 + (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dia < 1 || dia > diasEnMes[mes - 1]) {
        return false;
    }
    // Si pasa todas las verificaciones, es una fecha vÃƒÂ¡lida
    return true;
}

    void registrarUsuario(const string& username, const string& password) {
        if (usuarioExiste(username)) {
            cout << "\n\n\n\n\t\t\t\tUsuario ya existe." << endl;
            cout << "\t\t\tPulse cualquier tecla para regresar..." << endl;
            return;
        }
        string hashed_password = to_string(hash<string>{}(password));
        usuarios.push_back({username, hashed_password});
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de usuarios." << endl;
            return;
        }
        file << username << "," << hashed_password << endl;
        file.close();
        cout << "\n\n\n\n\tRegistro exitoso. Usuario: " << username << " ha sido registrado." << endl;
        cout << "\t\t\tPulse enter para continuar...";
        cin.ignore();
        cin.get();
    }

    bool usuarioExiste(const string& username) {
        return any_of(usuarios.begin(), usuarios.end(), [&](const Usuario& u) {
            return u.username == username;
        });
    }

    bool iniciarSesion(const string& username, const string& password) {
        string hashed_password = to_string(hash<string>{}(password));
        return any_of(usuarios.begin(), usuarios.end(), [&](const Usuario& u) {
            return u.username == username && u.password_hash == hashed_password;
        });
    }

    void cargarUsuarioPersonal() {
        ifstream archivo(filepersonal);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo" << endl;
            return;
        }
        string linea;
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string usuarioPer, passwordPer;
            getline(ss, usuarioPer, ',');
            getline(ss, passwordPer, ',');
            usuariosPer.push_back({usuarioPer, passwordPer});
        }
        archivo.close();
    }

    bool iniciarSesionPer(const string& username, const string& password) {
        return any_of(usuariosPer.begin(), usuariosPer.end(), [&](const UsuarioPersonal& u) {
            return u.usernamePersonal == username && u.passwordPersonal == password;
        });
    }

    void cargarHabitaciones() {
        ifstream file(habitacionesFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de habitaciones." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id_str, tipo, precio_str, disponible_str;
            getline(ss, id_str, ',');
            getline(ss, tipo, ',');
            getline(ss, precio_str, ',');
            getline(ss, disponible_str, ',');
            Habitacion hab = {stoi(id_str), tipo, stod(precio_str), stoi(disponible_str) == 1};
            habitaciones.push_back(hab);
        }
        file.close();
    }

    void guardarHabitaciones() {
        ofstream file(habitacionesFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de habitaciones." << endl;
            return;
        }
        for (const auto& h : habitaciones) {
            file << h.id << "," << h.tipo << "," << h.precio << "," << h.disponible << endl;
        }
        file.close();
    }

    void cargarReservas() {
        ifstream file(reservasFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de reservas." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Reserva r;
            ss >> r.id;
            ss.ignore();
            getline(ss, r.username, ',');
            ss >> r.habitacionId;
            ss.ignore();
            getline(ss, r.fechaInicio, ',');
            getline(ss, r.fechaFin, ',');
            reservas.push_back(r);
        }
        file.close();
    }

    void guardarReservas() {
        ofstream file(reservasFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de reservas." << endl;
            return;
        }
        for (const auto& r : reservas) {
            file << r.id << "," << r.username << "," << r.habitacionId << "," << r.fechaInicio << "," << r.fechaFin << endl;
        }
        file.close();
    }

    void hacerReserva(const string& username) {
        system("cls");
        cuadro(0, 0, 80, 24);
        gotoxy(30, 2); cout << "Hacer una reserva";
        int habitacionId;
        string fechaInicio, fechaFin;
        gotoxy(2, 16); cout << "Ingrese el numero de la habitacion: ";
        cin >> habitacionId;
        gotoxy(2, 17); cout << "Fecha de inicio (DD/MM/YYYY): ";
        cin >> fechaInicio;
        if (!esFechaValida(fechaInicio)) {
            cout << "\nFecha de inicio no valida.\nPulse enter para continuar...";
            cin.ignore();
            cin.get();
            return;
        }
        gotoxy(2, 18); cout << "Fecha de fin (DD/MM/YYYY): ";
        cin >> fechaFin;
        if (!esFechaValida(fechaFin)) {
            cout << "\nFecha de fin no valida.\nPulse enter para continuar...";
            cin.ignore();
            cin.get();
            return;
        }
        auto it = find_if(habitaciones.begin(), habitaciones.end(), [&](const Habitacion& h) {
            return h.id == habitacionId && h.disponible;
        });
        if (it != habitaciones.end()) {
            it->disponible = false;
            reservas.push_back({nextReservaId++, username, habitacionId, fechaInicio, fechaFin});
            guardarHabitaciones();
            guardarReservas();
            cout << "\n\tReserva realizada con exito.\n\tHabitacion numero " << habitacionId << " reservada a nombre del cliente " << username << " \n\tdesde " << fechaInicio << " hasta " << fechaFin << "\n\tID de la reserva: " << nextReservaId - 1 << " \n\tPulse enter para continuar...";
        } else {
            cout << "\n\tHabitacion no disponible o ID invalido.\n\tPulse enter para continuar...";
        }
        cin.ignore();
        cin.get();
    }
     void verReservasUsuario(const string& username) {
    system("cls");
    cuadro(0, 0, 80, 24);
    gotoxy(30, 2); cout << "Reservas del usuario: " << username;
    
    bool reservaEncontrada = false;
    int linea = 4;
    // Recorremos todas las reservas
    for (const auto& reserva : reservas) {
        if (reserva.username == username) {
            // Buscamos la habitaciÃ³n correspondiente a la reserva
            for (const auto& habitacion : habitaciones) {
                if (habitacion.id == reserva.habitacionId) {
                    // Mostramos los detalles de la reserva
                    gotoxy(2, linea++); cout << "ID de la reserva: " << reserva.id;
                    gotoxy(2, linea++); cout << "NÃºmero de habitaciÃ³n: " << reserva.habitacionId;
                    gotoxy(2, linea++); cout << "Tipo de habitaciÃ³n: " << habitacion.tipo;
                    gotoxy(2, linea++); cout << "Fecha de inicio: " << reserva.fechaInicio;
                    gotoxy(2, linea++); cout << "Fecha de fin: " << reserva.fechaFin;
                    gotoxy(2, linea++); cout << "-------------------------";
                    reservaEncontrada = true;
                }
            }
        }
    }
    if (!reservaEncontrada) {
        gotoxy(2, 4); cout << "\nNo tiene una reserva hecha en este momento.";
    }
    gotoxy(2, linea); cout << "\nPulse enter para continuar...";
    cin.ignore();
    cin.get();

    // Regresamos a la funciÃ³n anterior
    return;
}

    void verReservas() {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Reservas";
        int y = 2;
        for (const auto& r : reservas) {
            gotoxy(2, y+=2);
            cout << "ID Reserva: " << r.id << ", Usuario: " << r.username << ", ID Habitacion: " << r.habitacionId << ", \n\tFecha Inicio: " << r.fechaInicio << ", Fecha Fin: " << r.fechaFin;
        }
        cout << "\n\tPulse enter para continuar...";
        cin.ignore();
        cin.get();
    }

    void cancelarReserva(const string& username) {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Cancelar Reserva";
        int reservaId;
        gotoxy(2, 4); cout << "Ingrese el ID de la reserva: ";
        cin >> reservaId;

        auto it = find_if(reservas.begin(), reservas.end(), [&](const Reserva& r) {
            return r.id == reservaId && r.username == username;
        });

        if (it != reservas.end()) {
            auto habitacionIt = find_if(habitaciones.begin(), habitaciones.end(), [&](const Habitacion& h) {
                return h.id == it->habitacionId;
            });

            if (habitacionIt != habitaciones.end()) {
                habitacionIt->disponible = true;
            }

            reservas.erase(it);
            guardarHabitaciones();
            guardarReservas();
            cout << "\nReserva cancelada con exito.\nPulse enter para continuar...";
        } else {
            cout << "\nID de reserva invalido o no pertenece al usuario.\nPulse enter para continuar...";
        }

        cin.ignore();
        cin.get();
    }

    // FunciÃ³n para mostrar las habitaciones disponibles por tipo
    void mostrarHabitacionesDisponiblesPorTipo(const std::string& tipo) {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(20, 2); std::cout << "Habitaciones Disponibles - Tipo: " << tipo;
        int y = 4;
        for (const auto& hab : habitaciones) {
            if (hab.tipo == tipo && hab.disponible) {
                gotoxy(2, y++);
                std::cout << "Numero: " << hab.id << ", Precio: " << hab.precio;
            }
        }
        std::cout << "\n\t\t\tPulse enter para continuar...";
        std::cin.ignore();
        std::cin.get();
    }

    // FunciÃ³n para mostrar los tipos de habitaciones disponibles
    void mostrarTiposDeHabitaciones() {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); std::cout << "Seleccionar Tipo de Habitacion";

        std::set<std::string> tipos;
        for (const auto& hab : habitaciones) {
            tipos.insert(hab.tipo);
        }

        std::vector<std::string> tipoVector(tipos.begin(), tipos.end());
        int numTipos = tipoVector.size();
        int opcionSeleccionada = 1;
        int tecla;
        bool repite = true;

        do {
            system("cls");
            cuadro(0, 0, 80, 24);
            cuadro(1, 1, 77, 3);
            gotoxy(30, 2); std::cout << "Seleccionar Tipo de Habitacion";
            gotoxy(5, 4 + opcionSeleccionada); std::cout << "==>";

            for (int i = 0; i < numTipos; ++i) {
                gotoxy(10, 5 + i);
                std::cout << (i + 1) << ") " << tipoVector[i];
            }

            do {
                tecla = getch();
            } while (tecla != TECLA_ARRIBA && tecla != TECLA_ABAJO && tecla != ENTER);

            switch(tecla) {
                case TECLA_ARRIBA:
                    opcionSeleccionada--;
                    if (opcionSeleccionada < 1) {
                        opcionSeleccionada = numTipos;
                    }
                    break;
                case TECLA_ABAJO:
                    opcionSeleccionada++;
                    if (opcionSeleccionada > numTipos) {
                        opcionSeleccionada = 1;
                    }
                    break;
                case ENTER:
                    repite = false;
                    break;
            }
        } while (repite);

        mostrarHabitacionesDisponiblesPorTipo(tipoVector[opcionSeleccionada - 1]);
    }
};

// Funciones para el menÃƒÂº del cliente iniciado sesion
void OpcsDCliente(UserManager& um, const string& username) {
    int opt;
    bool repite=true;
    const char *titulo = "Que accion desea realizar";
	const char *opciones[] = {"Ver habitaciones disponibles", "Hacer una reserva","Pedir menu","Cancelar una reserva","Ver reservas hechas","Retroceder al menu principal"};
    int m=6;
    do {
        opt=menu(titulo, opciones ,m);
 
        switch(opt){
            case 1:

                um.mostrarTiposDeHabitaciones();
                break;
            case 2:
                um.hacerReserva(username);
                break;
            case 3:
                menu_comida();
                break;
            case 4:
                um.cancelarReserva(username);
                break;
            case 5:
                um.verReservasUsuario(username);
                break;
            case 6:
                repite=false;
                break;
        }
    } while (repite);
}

// FunciÃƒÂ³n para el menÃƒÂº del cliente previo inicio sesiÃƒÂ³n
void menu_cliente() {
    UserManager um;
    bool repite=true;
    int opt;
    const char *titulo = "Bienvenido al Hotel C++";
	const char *opciones[] = {"Iniciar sesion", "Registrarse","Retroceder al menu principal"};
    int m=3;
    do {
        opt=menu(titulo, opciones ,m);


        switch(opt){
            case 1:
                menu_iniciarcliente();
                break;
            case 2:
                menu_iniciarregistro();
                break;
            case 3:
                repite=false;
                break;
        }
    } while (repite);
}

// FunciÃƒÂ³n para el menÃƒÂº de inicio de registro
void menu_iniciarregistro() {
    UserManager um;
    system("cls");
    cuadro(0, 0, 80, 24);
    cuadro(1, 1, 77, 3);
    gotoxy(30, 2); cout << "Registro de usuario nuevo";
    string username, password;
    cout << "\n\n\n\n\t\t\t\tUsername: ";
    cin >> username;
    cout << "\n\t\t\t\tPassword: ";
    cin >> password;
    um.registrarUsuario(username, password);
}

// FunciÃƒÂ³n para el menÃƒÂº de inicio de cliente
void menu_iniciarcliente() {
    UserManager um;
    system("cls");
    cuadro(0, 0, 80, 24);
    cuadro(1, 1, 77, 3);
    gotoxy(30, 2); cout << "Inicio de sesion de cliente";
    string username, password;
    cout << "\n\n\n\n\t\t\t\tUsername: ";
    cin >> username;
    cout << "\n\t\t\t\tPassword: ";
    cin >> password;
    if (um.iniciarSesion(username, password)) {
        OpcsDCliente(um, username);
    } else {
        cout << "\n\n\n\n\t\t\t\tCredenciales incorrectas." << endl;
        cout << "\t\t\tPulse enter para regresar...";
        cin.ignore();
        cin.get();
    }
}

void menu_desayuno(){
    int opt;
    bool repite=true;
    const char *titulo = "Menu de Desayuno";
	const char *opciones[] = {"Chicharron de cerdo 1/4", "Tamal con zarza","Pan con huevo y salchicha huachana",
    "Tostada francesa","Cafe","Jugo de papaya","Capuccino","Jugo de fresa con leche","Regresar"};
    int m=9;

    do {
        opt=menu(titulo,opciones,m);
        switch(opt){
            
            case 1:
                gotoxy(6,18);cout<<"Usted ha pedido un 1/4 de chicharron,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 2:
                gotoxy(6,18);cout<<"Usted ha pedido un tamal con zarza,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 3:
                gotoxy(6,18);cout<<"Usted ha pedido un pan con huevo y salchicha,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 4:
                gotoxy(6,18);cout<<"Usted ha pedido una tostada francesa,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 5:
                gotoxy(6,18);cout<<"Usted ha pedido un cafe,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 6:
                gotoxy(6,18);cout<<"Usted ha pedido un jugo de papaya,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 7:
                gotoxy(6,18);cout<<"Usted ha pedido un capuccino,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 8:
                gotoxy(6,18);cout<<"Usted ha pedido un jugo de fresa con leche,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 9:
                repite=false;         
                break;
        }
    } while (repite);

}
void menu_cena(){
    int opt;
    bool repite=true;
    const char *titulo = "Menu de Cena";
	const char *opciones[] = {"Chicharron de cerdo 1/4", "Arroz con atun","Pan con huevo y salchicha huachana",
    "Tostada francesa","Cafe","Jugo de papaya","Capuccino","Jugo de fresa con leche","Regresar"};
    int m=9;

    do {
        opt=menu(titulo,opciones,m);
        switch(opt){
            case 1:
                gotoxy(6,18);cout<<"Usted ha pedido un 1/4 de chicharron,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 2:
                gotoxy(6,18);cout<<"Usted ha pedido un arroz con atun,muchas gracias"<<endl;
                system("pause");
                repite=false;

                break;
            case 3:
                gotoxy(6,18);cout<<"Usted ha pedido un pan con huevo y salchicha,muchas gracias"<<endl;
                system("pause");
                repite=false;

                break;
            case 4:
                gotoxy(6,18);cout<<"Usted ha pedido una tostada francesa,muchas gracias"<<endl;
                system("pause");
                repite=false;         
                break;
            case 5:
                gotoxy(6,18);cout<<"Usted ha pedido un cafe,muchas gracias"<<endl;
                system("pause");
                repite=false;        
                break;
            case 6:
                gotoxy(6,18);cout<<"Usted ha pedido un jugo de papaya,muchas gracias"<<endl;
                system("pause");
                repite=false;           
                break;
            case 7:
                gotoxy(6,18);cout<<"Usted ha pedido un capuccino,muchas gracias"<<endl;
                system("pause");
                repite=false;      
                break;
            case 8:
                gotoxy(6,18);cout<<"Usted ha pedido un jugo de fresa con leche,muchas gracias"<<endl;
                 system("pause");
                repite=false;      
                break;
            case 9:
                repite=false;         
                break;
        }
    } while (repite);

}
void menu_almuerzo(){
    int opt;
    bool repite=true;
    const char *titulo = "Menu de Almuerzo";
	const char *opciones[] = {"Arroz a la cubana", "Pechuga a la plancha","Pescado frito",
    "Arroz con atun","Cafe","Chicha a litro","Capuccino","Maracuya","Regresar"};
    int m=9;

    do {
        opt=menu(titulo,opciones,m);  
        switch(opt){
            case 1:
                gotoxy(6,18);cout<<"Usted ha pedido un arroz a la cubana,muchas gracias"<<endl;
                system("pause");
                repite=false;
                break;
            case 2:
                gotoxy(6,18);cout<<"Usted ha pedido una pechuga a la plancha,muchas gracias"<<endl;
                system("pause");
                repite=false;

                break;
            case 3:
                gotoxy(6,18);cout<<"Usted ha pedido un pescado frito,muchas gracias"<<endl;
                system("pause");
                repite=false; 

                break;
            case 4:
                gotoxy(6,18);cout<<"Usted ha pedido un arroz con atun,muchas gracias"<<endl;
                system("pause");
                repite=false;           
                break;
            case 5:
                gotoxy(6,18);cout<<"Usted ha pedido un cafe,muchas gracias"<<endl;         
                system("pause");
                repite=false;                
                break;
            case 6:
                gotoxy(6,18);cout<<"Usted ha pedido una chicha a litro,muchas gracias"<<endl;
                system("pause");
                repite=false;          
                break;
            case 7:
                gotoxy(6,18);cout<<"Usted ha pedido un capuccino,muchas gracias"<<endl;    
                system("pause");
                repite=false;       
                break;
            case 8:
                gotoxy(6,18);cout<<"Usted ha pedido una maracuya,muchas gracias"<<endl;
                system("pause");
                repite=false;           
                break;
            case 9:
                repite=false;         
                break;
        }
    } while (repite);


}
void menu_comida() {
    int opt;
    bool repite=true;
    const char *titulo = "Menu de Comida";
	const char *opciones[] = {"Pedir desayuno", "Pedir almuerzo","Pedir cena","Retroceder"};
    int m=4;
    do {
        opt=menu(titulo, opciones ,m);//Lo que esta debajo de verde era la antigua forma de poner el menu ya no sera necesario eso
        switch(opt){
            case 1:
                menu_desayuno();
                break;
            case 2:
                menu_almuerzo();
                break;
            case 3:
                menu_cena();
                break;
            case 4:
                repite=false;
                break;
        }
    } while (repite);
}

void menu_personal() {
    UserManager um;
    bool repite=true;
    int opt;
    const char *titulo = "Menu del personal";
	const char *opciones[] = {"Ver reservas", "Ver habitaciones disponibles","Retroceder al menu principal"};
    int m=3;    
    do {
        opt=menu(titulo, opciones ,m);

        switch(opt){
            case 1:
                um.verReservas();
                break;
            case 2:
                um.mostrarTiposDeHabitaciones();
                break;
            case 3:
                repite=false;
                break;
        }
    } while (repite);
}

void menuingresopersonal(){
    UserManager um;
    system("cls");
    cuadro(0, 0, 80, 24);
    cuadro(1, 1, 77, 3);
    gotoxy(30, 2); cout << "Inicio de sesion de personal";
    string username, password;
    cout << "\n\n\n\n\t\t\t\tUsername: ";
    cin >> username;
    cout << "\n\t\t\t\tPassword: ";
    cin >> password;
    if (um.iniciarSesionPer(username, password)) {
        menu_personal();
    } else {
        cout << "\n\n\n\n\t\t\t\tCredenciales incorrectas." << endl;
        cout << "\t\t\tPulse enter para regresar...";
        cin.ignore();
        cin.get();
    }    
}
void menuprincipal() {
    bool repite=true;
    int opt;
    system("COLOR B0");
    const char *titulo = "Hotel Management System"; //Aqui ya no sera necesario poner uno por uno las funciones
	const char *opciones[] = {"Cliente", "Personal","Salir"};//Tan solo agregas la opcion nueva aqui
    int m=3;//y agregas +1 cuando agregues una funcion
    do {
        opt=menu(titulo, opciones ,m);


        switch(opt){
            case 1:
                menu_cliente();
                break;
            case 2:
                menuingresopersonal();//ingresopersonal originalmente estaba todas sus funciones en el case 2,pero
                //solia darme a cada rato errores,asi que la solucion que encontre era asignarle su propia funcion
                break;
            case 3:
                repite = false;
                break;
        }

}while(repite);
}

void gotoxy(int x,int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X=x;
	dwPos.Y=y;
	SetConsoleCursorPosition(hCon,dwPos);
}

void cuadro(int x1, int y1, int x2, int y2) {
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y1);
        cout << "-";
        gotoxy(i, y2);
        cout << "-";
    }

    for (int i = y1; i <= y2; i++) {
        gotoxy(x1, i);
        cout << "|";
        gotoxy(x2, i);
        cout << "|";
    }

    gotoxy(x1, y1);
    cout << "+";
    gotoxy(x1, y2);
    cout << "+";
    gotoxy(x2, y1);
    cout << "+";
    gotoxy(x2, y2);
    cout << "+";
}

int main() {
    menuprincipal();
    return 0;
}

//FUNCION DESTINADA A REALIZAR EL TITULO Y SUS OPCIONES,YA NO SERA NECESARIO PONERLE UNO APOR UNO
int menu(const char *titulo,const char *opciones[], int m)
{
	int opcionSeleccionada = 1;
	int tecla;
	bool repite = true;
	
	do{
		system("cls");
		cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
		gotoxy(5,3 + opcionSeleccionada);cout<< "==>";
		//imprime el titulo
		gotoxy(15,2);cout <<titulo;
		
		for(int i=0;i<m;i++){
			gotoxy(10,4+i);cout<<i + 1 << ") " << opciones[i];
		}
		
		do{
			tecla=getch();
		}while(tecla!=TECLA_ARRIBA && tecla != TECLA_ABAJO && tecla != ENTER);
		
		switch(tecla){
			case TECLA_ARRIBA:
				opcionSeleccionada--;
				
				if(opcionSeleccionada == 0){
					opcionSeleccionada =m;
				}
				break;
			case TECLA_ABAJO:
				opcionSeleccionada++;
				if(opcionSeleccionada> m){
					opcionSeleccionada = 1;
				}
				break;
			
			case ENTER:
				repite = false;
				break;
		}
		
	} while(repite);
	
	
	
	return opcionSeleccionada;
}
