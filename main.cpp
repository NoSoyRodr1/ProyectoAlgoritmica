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
using namespace std;

// Declaraciones de funciones
void gotoxy(int x, int y);
void cuadro(int x1, int y1, int x2, int y2);
void menuprincipal();
void menu_cliente();
void menu_iniciarcliente();
void menu_iniciarregistro();
void menu_personal();
void menu_comida();
void hacerReserva(const string& username);
void verReservasUsuario(const string& username);
void verReservas();
void cancelarReserva(const string& username);
void mostrarHabitacionesDisponiblesPorTipo(const string& tipo);
bool esFechaValida(const string& fecha);

// Estructuras para representar usuario, usuario del personal, habitación y reserva
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

// Clase para manejar la gestión de usuarios
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
    // Extraer día, mes y año
    stringstream ss(fecha);
    int dia, mes, anio;
    char delim;
    ss >> dia >> delim >> mes >> delim >> anio;
    // Verificar si se pudieron extraer correctamente
    if (ss.fail()) {
        return false;
    }
    // Verificar si el año es válido
    if (anio < 2023) {
        return false;
    }
    // Verificar si el mes es válido
    if (mes < 1 || mes > 12) {
        return false;
    }
    // Verificar los días dependiendo del mes y si es año bisiesto o no
    int diasEnMes[] = {31, 28 + (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dia < 1 || dia > diasEnMes[mes - 1]) {
        return false;
    }
    // Si pasa todas las verificaciones, es una fecha válida
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
    gotoxy(2, 16); cout << "Ingrese el número de la habitación: ";
    cin >> habitacionId;
    gotoxy(2, 17); cout << "Fecha de inicio (DD/MM/YYYY): ";
    cin >> fechaInicio;
    if (!esFechaValida(fechaInicio)) {
        cout << "\nFecha de inicio no válida.\nPulse enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }
    gotoxy(2, 18); cout << "Fecha de fin (DD/MM/YYYY): ";
    cin >> fechaFin;
    if (!esFechaValida(fechaFin)) {
        cout << "\nFecha de fin no válida.\nPulse enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }
    auto it = find_if(habitaciones.begin(), habitaciones.end(), [&](const Habitacion& h){
        return h.id == habitacionId && h.disponible;
    });
    if (it != habitaciones.end()) {
        it->disponible = false;
        // Usamos nextReservaId para el ID de la nueva reserva y luego lo incrementamos
        reservas.push_back({nextReservaId++, username, habitacionId, fechaInicio, fechaFin});
        guardarHabitaciones();
        guardarReservas();
        cout << "\nReserva realizada con éxito.\nHabitación número " << habitacionId << " reservada a nombre del cliente " << username << " desde " << fechaInicio << " hasta " << fechaFin << "\nID de la reserva: " << nextReservaId - 1 << " \nPulse enter para continuar...";
    } else {
        cout << "\nHabitación no disponible o ID inválido.\nPulse enter para continuar...";
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
            // Buscamos la habitación correspondiente a la reserva
            for (const auto& habitacion : habitaciones) {
                if (habitacion.id == reserva.habitacionId) {
                    // Mostramos los detalles de la reserva
                    gotoxy(2, linea++); cout << "ID de la reserva: " << reserva.id;
                    gotoxy(2, linea++); cout << "Número de habitación: " << reserva.habitacionId;
                    gotoxy(2, linea++); cout << "Tipo de habitación: " << habitacion.tipo;
                    gotoxy(2, linea++); cout << "Fecha de inicio: " << reserva.fechaInicio;
                    gotoxy(2, linea++); cout << "Fecha de fin: " << reserva.fechaFin;
                    gotoxy(2, linea++); cout << "-------------------------";
                    reservaEncontrada = true;
                }
            }
        }
    }
    if (!reservaEncontrada) {
        gotoxy(2, 4); cout << "No tiene una reserva hecha en este momento.";
    }
    gotoxy(2, linea); cout << "Pulse enter para continuar...";
    cin.ignore();
    cin.get();

    // Regresamos a la función anterior
    return;
}




    void verReservas() {
        system("cls");
        cuadro(0, 0, 80, 24);
        gotoxy(30, 2); cout << "Reservas";
        int y = 4;
        for (const auto& r : reservas) {
            gotoxy(2, y++);
            cout << "ID Reserva: " << r.id << ", Usuario: " << r.username << ", ID Habitación: " << r.habitacionId << ", Fecha Inicio: " << r.fechaInicio << ", Fecha Fin: " << r.fechaFin;
        }
        cout << "\nPulse enter para continuar...";
        cin.ignore();
        cin.get();
    }

    void cancelarReserva(const string& username) {
        system("cls");
        cuadro(0, 0, 80, 24);
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
            cout << "\nReserva cancelada con éxito.\nPulse enter para continuar...";
        } else {
            cout << "\nID de reserva inválido o no pertenece al usuario.\nPulse enter para continuar...";
        }

        cin.ignore();
        cin.get();
    }

    // Función para mostrar las habitaciones disponibles por tipo
    void mostrarHabitacionesDisponiblesPorTipo(const string& tipo) {
        system("cls");
        cuadro(0, 0, 80, 24);
        gotoxy(30, 1); cout << "Habitaciones Disponibles - Tipo: " << tipo;
        int y = 4;
        for (const auto& hab : habitaciones) {
            if (hab.tipo == tipo && hab.disponible) {
                gotoxy(2, y++);
                cout << "Número: " << hab.id << ", Precio: " << hab.precio;
            }
        }
        cout << "\n\n\t\t\tPulse enter para continuar...";
        cin.ignore();
        cin.get();
    }

    // Función para mostrar los tipos de habitaciones disponibles
    void mostrarTiposDeHabitaciones() {
        system("cls");
        cuadro(0, 0, 80, 24);
        gotoxy(30, 1); cout << "Seleccionar Tipo de Habitacion";
        set<string> tipos;
        for (const auto& hab : habitaciones) {
            tipos.insert(hab.tipo);
        }
        int index = 1;
        vector<string> tipoVector(tipos.begin(), tipos.end());
        for (const auto& tipo : tipos) {
            gotoxy(2, 4 + index);
            cout << index++ << ") " << tipo;
        }
        int opt;
        gotoxy(2, 4 + index + 2); cout << "Opcion: ";
        cin >> opt;

        if (opt > 0 && opt <= tipoVector.size()) {
            mostrarHabitacionesDisponiblesPorTipo(tipoVector[opt - 1]);
        }
    }
};

// Funciones para el menú del cliente iniciado sesion
void OpcsDCliente(UserManager& um, const string& username) {
    string opt;
    do {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Bienvenido " << username << " Que accion desea realizar";
        gotoxy(2, 4); cout << "1) Ver habitaciones disponibles";
        gotoxy(2, 5); cout << "2) Hacer una reserva";
        gotoxy(2, 6); cout << "3) Pedir menú";
        gotoxy(2, 7); cout << "4) Cancelar una reserva";
        gotoxy(2, 8); cout << "5) Ver reserva hecha";
        gotoxy(2, 9); cout << "6) Retroceder al menú principal";
        gotoxy(2, 10); cout << "Opción: ";
        cin >> opt;

        if (opt == "1") {
            um.mostrarTiposDeHabitaciones();
        } else if (opt == "2") {
            um.hacerReserva(username);
        } else if (opt == "3") {
            menu_comida();
        } else if (opt == "4") {
            um.cancelarReserva(username);
        } else if (opt == "5") {
            um.verReservasUsuario(username);
        }

    } while (opt != "6");
}

// Función para el menú del cliente previo inicio sesión
void menu_cliente() {
    UserManager um;
    string opt;
    do {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 1); cout << "Bienvenido al hotel C++";
        gotoxy(2, 4); cout << "1) Iniciar sesion" << endl;
        gotoxy(2, 5); cout << "2) Registrarse" << endl;
        gotoxy(2, 6); cout << "3) Retroceder al menu principal" << endl;
        gotoxy(2, 8); cout << "Opcion: ";
        cin >> opt;

        if (opt == "1") {
            menu_iniciarcliente();
        } else if (opt == "2") {
            menu_iniciarregistro();
        }
    } while (opt != "3");
}

// Función para el menú de inicio de registro
void menu_iniciarregistro() {
    UserManager um;
    system("cls");
    cuadro(0, 0, 80, 24);
    gotoxy(30, 1); cout << "Registro de usuario nuevo";
    string username, password;
    cout << "\n\n\n\n\t\t\t\tUsername: ";
    cin >> username;
    cout << "\n\t\t\t\tPassword: ";
    cin >> password;
    um.registrarUsuario(username, password);
}

// Función para el menú de inicio de cliente
void menu_iniciarcliente() {
    UserManager um;
    system("cls");
    cuadro(0, 0, 80, 24);
    gotoxy(30, 1); cout << "Inicio de sesion de cliente";
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

void menu_comida() {
    string opt;
    do {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 1); cout << "Menu de Comida";
        gotoxy(2, 4); cout << "1) Pedir desayuno" << endl;
        gotoxy(2, 5); cout << "2) Pedir almuerzo" << endl;
        gotoxy(2, 6); cout << "3) Pedir cena" << endl;
        gotoxy(2, 7); cout << "4) Retroceder" << endl;
        gotoxy(2, 8); cout << "Opcion: ";
        cin >> opt;
        if (opt == "1") {
            cout << "\nDesayuno pedido.\nPulse enter para continuar...";
            cin.ignore();
            cin.get();
        } else if (opt == "2") {
            cout << "\nAlmuerzo pedido.\nPulse enter para continuar...";
            cin.ignore();
            cin.get();
        } else if (opt == "3") {
            cout << "\nCena pedida.\nPulse enter para continuar...";
            cin.ignore();
            cin.get();
        }
    } while (opt != "4");
}

void menu_personal() {
    UserManager um;
    string opt;
    do {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 1); cout << "Menu del personal";
        gotoxy(2, 4); cout << "1) Ver reservas" << endl;
        gotoxy(2, 5); cout << "2) Ver habitaciones disponibles" << endl;
        gotoxy(2, 6); cout << "3) Retroceder al menu principal" << endl;
        gotoxy(2, 8); cout << "Opcion: ";
        cin >> opt;
        if (opt == "1") {
            um.verReservas();
        } else if (opt == "2") {
            um.mostrarTiposDeHabitaciones();
        }
    } while (opt != "3");
}

void menuprincipal() {
    string opt;
    do {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 1); cout << "Hotel Management System";
        gotoxy(2, 4); cout << "1) Cliente" << endl;
        gotoxy(2, 5); cout << "2) Personal" << endl;
        gotoxy(2, 6); cout << "3) Salir" << endl;
        gotoxy(2, 8); cout << "Opcion: ";
        cin >> opt;
        if (opt == "1") {
            menu_cliente();
        } else if (opt == "2") {
            UserManager um;
            system("cls");
            cuadro(0, 0, 80, 24);
            gotoxy(30, 1); cout << "Inicio de sesion de personal";
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
    } while (opt != "3");
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
