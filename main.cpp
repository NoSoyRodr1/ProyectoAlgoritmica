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

// Estructuras para representar usuario, usuario del personal, habitaciÃ³n y reserva
struct Usuario {
    string username;
    string password_hash;
    double saldo;
};
struct Cochera {
    string cocheraid;
    double precioH;
    bool disponibleC;
};
struct ReserCoche {
    int idC;
    string username;
    string cocheraid;
    string fechaInicio;
    string fechaFin;
};

struct UsuarioPersonal {
    string usernamePersonal;
    string passwordPersonal;
    int nivel;
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

// Clase para manejar la gestiÃ³n de usuarios
class UserManager {
private:
    vector<Usuario> usuarios;
    vector<UsuarioPersonal> usuariosPer;
    vector<Habitacion> habitaciones;
    vector<Reserva> reservas;
    vector<Cochera> cocheras;
    vector<ReserCoche> reservasCocheras;
    string filename = "usuarios.csv";
    string filepersonal = "personal.csv";
    string habitacionesFile = "habitaciones.csv";
    string reservasFile = "reservas.csv";
    string reservasCFile = "reservcochera.csv";
    string cocherasFile = "cochera.csv";
    string historialFile = "historial.csv";
    int nextReservaId = 1;
public:
    UserManager() {
        cargarUsuarios();
        cargarUsuarioPersonal();
        cargarHabitaciones();
        cargarReservas();
        cargarCocheras();
    };
    void cargarUsuarios() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de usuarios." << endl;
            return;
        }
        string line;
        double mon;
        while (getline(file, line)) {
            stringstream ss(line);
            string usr, pwd, monStr;
            getline(ss, usr, ',');
            getline(ss, pwd, ',');
            getline(ss, monStr, ',');
            try {
                mon = stod(monStr); // Convertir string a double
                usuarios.push_back({usr, pwd, mon});
            } catch (const invalid_argument& e) {
                cerr << "Error de formato en la linea: " << line << endl;
            } catch (const out_of_range& e) {
                cerr << "Numero fuera de rango en la linea: " << line << endl;
            }
        }
        file.close();
    }
    void cargarReservasCocheras() {
        ifstream file(reservasCFile);
        if (!file) {
            cerr << "Error al abrir el archivo de reservas de cocheras." << endl;
            return;
        }
        reservasCocheras.clear();
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idC, username, cocheraid, fechaInicio, fechaFin;
            getline(ss, idC, ',');
            getline(ss, username, ',');
            getline(ss, cocheraid, ',');
            getline(ss, fechaInicio, ',');
            getline(ss, fechaFin, ',');
            reservasCocheras.push_back({stoi(idC), username, cocheraid, fechaInicio, fechaFin});
        }
        file.close();
    }
    void cargarCocheras() {
        ifstream file(cocherasFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de cocheras." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, precio_str, disponible_str;
            getline(ss, id, ',');
            getline(ss, precio_str, ',');
            getline(ss, disponible_str, ',');
            Cochera coche = {id, stod(precio_str), stoi(disponible_str) == 1};
            cocheras.push_back(coche);
        }
        file.close();
    }
    void guardarCocheras() {
        ofstream file(cocherasFile);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de cocheras." << endl;
            return;
        }
        for (const auto& c : cocheras) {
            file << c.cocheraid << "," << c.precioH << "," << c.disponibleC << endl;
        }
        file.close();
    }
    void guardarReservasCocheras() {
        ofstream file(reservasCFile);
        if (!file) {
            cerr << "Error al abrir el archivo de reservas de cocheras." << endl;
            return;
        }
        for (const auto& reserva : reservasCocheras) {
            file << reserva.idC << "," << reserva.username << "," << reserva.cocheraid << ","
                 << reserva.fechaInicio << "," << reserva.fechaFin << endl;
        }
        file.close();
    }
    void mostrarCocherasDisponibles() {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(20, 2); cout << "Cocheras Disponibles";

        // Filtrar las cocheras disponibles
        vector<Cochera> disponibles;
        for (const auto& cochera : cocheras) {
            if (cochera.disponibleC) {
                disponibles.push_back(cochera);
            }
        }

        int totalDisponibles = disponibles.size();
        int columnas = 2;
        int filas = totalDisponibles / columnas;
        int y = 4;

        for (int fila = 0; fila < filas; ++fila) {
            for (int columna = 0; columna < columnas; ++columna) {
                int index = fila + columna * filas;
                if (index < totalDisponibles) {
                    gotoxy(columna * 40 + 2, y);
                    cout << "ID: " << disponibles[index].cocheraid << ", Precio: " << disponibles[index].precioH;
                }
            }
            y++;
        }

        cout << "\n\t\t\tPulse enter para continuar...";
        cin.ignore();
        cin.get();
    }
    double obtenerPrecioCochera(const string& cocheraid) {
        auto it = find_if(cocheras.begin(), cocheras.end(), [&](const Cochera& c) {
            return c.cocheraid == cocheraid;
        });

        if (it != cocheras.end()) {
            return it->precioH;
        } else {
            // Manejar el caso donde no se encuentra la cochera
            // Por ejemplo, lanzar una excepción o devolver un valor por defecto.
            return 0.0; // Valor por defecto, ajusta según tu lógica.
        }
    }
    void hacerReservaCochera(const string& username) {
        double saldo = obtenerSaldoUsuario(username);

        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Hacer una reserva de cochera"<<endl;
        string cocheraId, fechaInicio, fechaFin;

        // Verificar si el usuario tiene saldo suficiente
        if (saldo < 0.0) {
            cout << "El usuario '" << username << "' no existe o tiene saldo negativo." << endl;
            cin.ignore();
            cin.get();
            return;
        }

        // Mostrar el saldo actual
        cout << "\n\tSaldo actual del usuario '" << username << "': " << saldo << endl;

        // Obtener el número de cochera
        gotoxy(2, 16); cout << "Ingrese el ID de la cochera: ";
        cin >> cocheraId;

        // Obtener el precio de la cochera
        double precioCochera = obtenerPrecioCochera(cocheraId);

        if (precioCochera <= 0.0) {
            cout << "\nCochera no encontrada o precio no válido.\n";
            cin.ignore();
            cin.get();
            return;
        }

        // Pedir fechas de inicio y fin de la reserva
        gotoxy(2, 17); cout << "Fecha de inicio (DD/MM/YYYY): ";
        cin >> fechaInicio;
        if (!esFechaValida(fechaInicio)) {
            cout << "\nFecha de inicio no válida.\n";
            cin.ignore();
            cin.get();
            return;
        }
        gotoxy(2, 18); cout << "Fecha de fin (DD/MM/YYYY): ";
        cin >> fechaFin;
        if (!esFechaValida(fechaFin)) {
            cout << "\nFecha de fin no valida.\n";
            cin.ignore();
            cin.get();
            return;
        }

        // Verificar si el usuario tiene saldo suficiente para reservar
        if (saldo < precioCochera * (diasEntreFechas(fechaInicio, fechaFin) + 1)) {
            cout << "Saldo insuficiente para hacer la reserva." << endl;
            cin.ignore();
            cin.get();
            return;
        }

        // Verificar disponibilidad de la cochera
        auto it = find_if(cocheras.begin(), cocheras.end(), [&](const Cochera& c) {
            return c.cocheraid == cocheraId && c.disponibleC;
        });
        if (it != cocheras.end()) {
            // Reservar la cochera
            it->disponibleC = false;
            reservasCocheras.push_back({nextReservaId++, username, cocheraId, fechaInicio, fechaFin});
            // Actualizar el saldo del usuario
            saldo -= precioCochera * (diasEntreFechas(fechaInicio, fechaFin) + 1);
            actualizarSaldoUsuario(username, saldo);

            // Guardar cambios
            guardarCocheras();
            guardarReservasCocheras();

            // Mostrar mensaje de reserva exitosa
            cout << "\n\tReserva de cochera realizada con exito.\n"
                 << "\tCochera ID: " << cocheraId << "\n"
                 << "\tDesde " << fechaInicio << " hasta " << fechaFin << "\n"
                 << "\tID de la reserva: " << nextReservaId - 1 << "\n"
                 << "\tPulse enter para continuar...";
        } else {
            // La cochera no está disponible
            cout << "\n\tCochera no disponible o ID invalido.\n"
                 << "\tPulse enter para continuar...";
        }
        cin.ignore();
        cin.get();
    }
    double obtenerSaldoUsuario(const string& username) {
            for (const auto& user : usuarios) {
                if (user.username == username) {
                    return user.saldo;
                }
            }
            return -1; // Retorna -1 si el usuario no se encuentra
        }
    bool esFechaValida(const string& fecha) {
        // Verificar que la fecha tenga el formato correcto DD/MM/YYYY
        if (fecha.size() != 10 || fecha[2] != '/' || fecha[5] != '/') {
            return false;
        }
        // Extraer dia, mes y aÃ±o
        stringstream ss(fecha);
        int dia, mes, anio;
        char delim;
        ss >> dia >> delim >> mes >> delim >> anio;
        // Verificar si se pudieron extraer correctamente
        if (ss.fail()) {
            return false;
        }
        // Verificar si el aÃ±o es va¡lido
        if (anio < 2023) {
            return false;
        }
        // Verificar si el mes es va¡lido
        if (mes < 1 || mes > 12) {
            return false;
        }
        // Verificar los dias dependiendo del mes y si es año bisiesto o no
        int diasEnMes[] = {31, 28 + (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (dia < 1 || dia > diasEnMes[mes - 1]) {
            return false;
        }
        // Si pasa todas las verificaciones, es una fecha valida
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
        file << username << "," << hashed_password << "," << 0 << endl;
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
            int nivel;
            getline(ss, usuarioPer, ',');
            getline(ss, passwordPer, ',');
            ss>>nivel;
            usuariosPer.push_back({usuarioPer, passwordPer, nivel});
        }
        archivo.close();
    }
    bool iniciarSesionPer(const string& username, const string& password, int& nivel) {
        auto it = find_if(usuariosPer.begin(), usuariosPer.end(), [&](const UsuarioPersonal& u) {
            return u.usernamePersonal == username && u.passwordPersonal == password;
        });
        if (it != usuariosPer.end()){
            nivel= it->nivel;
            return true;

        }
        return false;
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
    double obtenerPrecioHabitacion(int habitacionId) {
    auto it = find_if(habitaciones.begin(), habitaciones.end(), [&](const Habitacion& h) {
        return h.id == habitacionId;
    });
    if (it != habitaciones.end()) {
        return it->precio;
    }
    return 0.0;
    }
    void actualizarSaldoUsuario(const string& username, double nuevoSaldo) {
    for (auto& user : usuarios) {
        if (user.username == username) {
            user.saldo = nuevoSaldo;
            // Actualizar el archivo CSV
            ofstream file(filename);
            if (!file.is_open()) {
                cerr << "Error al abrir el archivo de usuarios." << endl;
                return;
            }
            for (const auto& u : usuarios) {
                file << u.username << "," << u.password_hash << "," << u.saldo << endl;
            }
            file.close();
            return;
        }
    }
    cerr << "Usuario no encontrado." << endl;
}
    double diasEntreFechas(const string& fechaInicio, const string& fechaFin) {
        // Validar ambas fechas
        if (!esFechaValida(fechaInicio) || !esFechaValida(fechaFin)) {
            return -1; // Código de error o indicador de fechas inválidas
        }
        // Extraer dia, mes y año de la fecha de inicio
        stringstream ssInicio(fechaInicio);
        int diaInicio, mesInicio, anioInicio;
        int diasEnMes[] = {31, 28 + (anioInicio % 4 == 0 && (anioInicio % 100 != 0 || anioInicio % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        char delimInicio;
        ssInicio >> diaInicio >> delimInicio >> mesInicio >> delimInicio >> anioInicio;
        // Extraer dia, mes y año de la fecha de fin
        stringstream ssFin(fechaFin);
        int diaFin, mesFin, anioFin;
        char delimFin;
        ssFin >> diaFin >> delimFin >> mesFin >> delimFin >> anioFin;
        // Convertir ambas fechas a días desde una fecha base (aquí 01/01/2023)
        int baseAnio = 2023;
        int diasDesdeInicio = diaInicio;
        for (int m = 1; m < mesInicio; ++m) {
            diasDesdeInicio += diasEnMes[m - 1];
        }
        for (int a = baseAnio; a < anioInicio; ++a) {
            diasDesdeInicio += 365 + (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0));
        }
        int diasDesdeFin = diaFin;
        for (int m = 1; m < mesFin; ++m) {
            diasDesdeFin += diasEnMes[m - 1];
        }
        for (int a = baseAnio; a < anioFin; ++a) {
            diasDesdeFin += 365 + (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0));
        }
        // Calcular la diferencia de días
        return diasDesdeFin - diasDesdeInicio;
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
        double saldo = obtenerSaldoUsuario(username);

        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Hacer una reserva"<<endl;
        int habitacionId;
        string fechaInicio, fechaFin;

        // Verificar si el usuario tiene saldo suficiente
        if (saldo < 0.0) {
            cout << " El usuario '" << username << "' no existe o tiene saldo negativo." << endl;
            cin.ignore();
            cin.get();
            return;
        }

        // Mostrar el saldo actual
        cout << "\n Saldo actual del usuario '" << username << "': " << saldo << endl;

        // Obtener el número de habitación
        gotoxy(2, 16); cout << "Ingrese el numero de la habitacion: ";
        cin >> habitacionId;

        // Obtener el precio de la habitación
        double precioHabitacion = obtenerPrecioHabitacion(habitacionId);

        if (precioHabitacion <= 0.0) {
            cout << "\nHabitacion no encontrada o precio no valido.\n";
            cin.ignore();
            cin.get();
            return;
        }



        // Pedir fechas de inicio y fin de la reserva
        gotoxy(2, 17); cout << "Fecha de inicio (DD/MM/YYYY): ";
        cin >> fechaInicio;
        if (!esFechaValida(fechaInicio)) {
            cout << "\nFecha de inicio no valida.\n";
            cin.ignore();
            cin.get();
            return;
        }
        gotoxy(2, 18); cout << "Fecha de fin (DD/MM/YYYY): ";
        cin >> fechaFin;
        if (!esFechaValida(fechaFin)) {
            cout << "\nFecha de fin no valida.\n";
            cin.ignore();
            cin.get();
            return;
        }

        // Verificar si el usuario tiene saldo suficiente para reservar
        if (saldo < precioHabitacion*(diasEntreFechas(fechaInicio, fechaFin)+1)) {
            cout << "Saldo insuficiente para hacer la reserva." << endl;
            cin.ignore();
            cin.get();
            return;
        }

        // Verificar disponibilidad de la habitación
        auto it = find_if(habitaciones.begin(), habitaciones.end(), [&](const Habitacion& h) {
            return h.id == habitacionId && h.disponible;
        });
        if (it != habitaciones.end()) {
            // Reservar la habitación
            it->disponible = false;
            reservas.push_back({nextReservaId++, username, habitacionId, fechaInicio, fechaFin});

            // Actualizar el saldo del usuario
            saldo -= precioHabitacion*(diasEntreFechas(fechaInicio, fechaFin)+1);
            actualizarSaldoUsuario(username, saldo);

            // Guardar cambios
            guardarHabitaciones();
            guardarReservas();

            // Mostrar mensaje de reserva exitosa
            cout << "\n\tReserva realizada con exito.\n"
                 << "\tHabitacion numero " << habitacionId << " reservada a nombre del cliente " << username << "\n"
                 << "\tDesde " << fechaInicio << " hasta " << fechaFin << "\n\t"
                 << "\tID de la reserva: " << nextReservaId - 1 ;
        } else {
            // La habitación no está disponible
            cout << "\nHabitacion no disponible o ID invalido.\n";
        }
        cin.ignore();
        cin.get();
    }
    void verReservasUsuario(const string& username) {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);        
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
                        gotoxy(2, linea++); cout << "Numero de habitacion: " << reserva.habitacionId;
                        gotoxy(2, linea++); cout << "Tipo de habitacion: " << habitacion.tipo;
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
        gotoxy(2, linea); cout << "\nPulse enter para continuar...";
        cin.ignore();
        cin.get();
        // Regresamos a la función anterior
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
                guardarHabitaciones(); // Guardar la habitación actualizada
            }

            // Obtener las fechas de inicio y fin de la reserva
            string fechaInicio = it->fechaInicio;
            string fechaFin = it->fechaFin;

            // Reembolsar el saldo al usuario
            double precioHabitacion = obtenerPrecioHabitacion(it->habitacionId) * (diasEntreFechas(fechaInicio, fechaFin) + 1);
            double saldoUsuario = obtenerSaldoUsuario(username);
            double nuevoSaldo = saldoUsuario + precioHabitacion;
            actualizarSaldoUsuario(username, nuevoSaldo);

            // Eliminar la reserva
            reservas.erase(it);
            guardarReservas(); // Guardar las reservas actualizadas

            cout << "\nReserva cancelada con exito. Reembolso realizado.\n";
        } else {
            cout << "\nID de reserva invalido o no pertenece al usuario.\n";
        }

        cout << "Pulse enter para continuar...";
        cin.ignore();
        cin.get();
    }
    void cancelarReservaCochera(const string& username) {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Cancelar Reserva de Cochera";
        int reservaId;
        gotoxy(2, 4); cout << "Ingrese el ID de la reserva: ";
        cin >> reservaId;

        auto it = find_if(reservasCocheras.begin(), reservasCocheras.end(), [&](const ReserCoche& r) {
            return r.idC == reservaId && r.username == username;
        });

        if (it != reservasCocheras.end()) {
            auto cocheraIt = find_if(cocheras.begin(), cocheras.end(), [&](const Cochera& c) {
                return c.cocheraid == it->cocheraid;
            });

            if (cocheraIt != cocheras.end()) {
                cocheraIt->disponibleC = true;
                guardarCocheras(); // Guardar la cochera actualizada
            }

            // Obtener las fechas de inicio y fin de la reserva
            string fechaInicio = it->fechaInicio;
            string fechaFin = it->fechaFin;

            // Reembolsar el saldo al usuario
            double precioCochera = obtenerPrecioCochera(it->cocheraid) * (diasEntreFechas(fechaInicio, fechaFin) + 1);
            double saldoUsuario = obtenerSaldoUsuario(username);
            double nuevoSaldo = saldoUsuario + precioCochera;
            actualizarSaldoUsuario(username, nuevoSaldo);

            // Eliminar la reserva
            reservasCocheras.erase(it);
            guardarReservasCocheras(); // Guardar las reservas actualizadas

            cout << "\nReserva de cochera cancelada con exito. Reembolso realizado.\n";
        } else {
            cout << "\nID de reserva invalido o no pertenece al usuario.\n";
        }

        cout << "Pulse enter para continuar...";
        cin.ignore();
        cin.get();
    }
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
    void eliminarUsuarioPersonal() {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(20, 2); cout <<"Eliminar credenciales del personal";
        string username;
        cout << "\n\n\n\n\t\t\t\tUsuario a eliminar: ";
        cin>>username;

    auto it = find_if(usuariosPer.begin(), usuariosPer.end(), [&](const UsuarioPersonal& u) {
        return u.usernamePersonal == username && u.nivel == 2;
    });

    if (it != usuariosPer.end()) {
        usuariosPer.erase(it);

        // Actualizar el archivo
        ofstream file(filepersonal, ios::trunc);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo de personal." << endl;
            return;
        }
        for (const auto& usuario : usuariosPer) {
            file << usuario.usernamePersonal << "," << usuario.passwordPersonal << "," << usuario.nivel << endl;
        }
        file.close();

        cout << "Usuario " << username << " eliminado correctamente." << endl;
    } else {
        cout << "Usuario no encontrado o no tiene permiso para eliminar este usuario." << endl;
    }
    cout << "Pulse enter para continuar...";
    cin.ignore();
    cin.get();
    }
    void verPersonal() {
        system("cls");
        cuadro(0, 0, 80, 24);
        cuadro(1, 1, 77, 3);
        gotoxy(30, 2); cout << "Personal";
        int y = 2;
        for (const auto& p : usuariosPer) {
            gotoxy(2, y+=2);
            cout <<"Nombre: "<<p.usernamePersonal<<", Nivel: " << p.nivel;
        }
        cout << "\n\n\tPulse enter para continuar...";
        cin.ignore();
        cin.get();
    }
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
// Funciones para el menÃº del cliente iniciado sesion
void OpcsDCliente(UserManager& um, const string& username) {
    int opt;
    bool repite=true;
    const char *titulo = "Bienvenido usuario que accion desea realizar";
	const char *opciones[] = {"Ver habitaciones disponibles", "Ver sitios en cochera disponibles", "Reservar una habitacion",
     "Reservar cochera","Pedir menu","Cancelar una reserva de habitacion","Cancelar una reserva de cochera","Ver reservas hechas","Retroceder al menu principal"};
    int m = 9;
    do {
        opt=menu(titulo, opciones ,m);
        switch(opt){
            case 1:
                um.mostrarTiposDeHabitaciones();
                break;
            case 2:
                um.mostrarCocherasDisponibles();
                break;
            case 3:
                um.hacerReserva(username);
                break;
            case 4:
                um.hacerReservaCochera(username);
                break;
            case 5:
                menu_comida();
                break;
            case 6:
                um.cancelarReserva(username);
                break;
            case 7:
                um.cancelarReservaCochera(username);
                break;
            case 8:
                um.verReservasUsuario(username);
                break;
            case 9:
                repite=false;
                break;
        }
    } while (repite);
}

// FunciÃ³n para el menÃº del cliente previo inicio sesiÃ³n
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

// FunciÃ³n para el menÃº de inicio de registro
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

// FunciÃ³n para el menÃº de inicio de cliente
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
        opt=menu(titulo, opciones ,m);
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

void menu_personal(int nivel) {
    UserManager um;
    int opt;
    const char *titulo = "Menu del personal";
	const char *opciones1[] = {"Ver reservas", "Ver habitaciones disponibles","Ver personal activo","Eliminar personal" ,"Retroceder al menu principal"};
    const char *opciones2[] = {"Ver reservas", "Ver habitaciones disponibles","Retroceder al menu principal"};
    int m1=5,m2=3;
    if(nivel==1){
        do{
            opt=menu(titulo, opciones1 ,m1);
        if (opt == 1) {
            um.verReservas();
        } else if (opt == 2) {
            um.mostrarTiposDeHabitaciones();
        } else if (opt == 3){
            um.verPersonal();
        }else if(opt == 4){
            um.eliminarUsuarioPersonal();

        }
        }while (opt != 5);
    }else if(nivel==2){
        do{
            opt=menu(titulo, opciones2 ,m2);
        if (opt == 1) {
            um.verReservas();
        } else if (opt == 2) {
            um.mostrarTiposDeHabitaciones();
        }

        }while (opt != 3);
    }
    
}

void menuingresopersonal(){
    UserManager um;
    system("cls");
    cuadro(0, 0, 80, 24);
    cuadro(1, 1, 77, 3);
    gotoxy(30, 2); cout << "Inicio de sesion de personal";
    string username, password;
    int nivel;
    cout << "\n\n\n\n\t\t\t\tUsername: ";
    cin >> username;
    cout << "\n\t\t\t\tPassword: ";
    cin >> password;
    if (um.iniciarSesionPer(username, password, nivel)) {
        menu_personal(nivel);
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
    const char *titulo = "FISIHOTEL"; //Aqui ya no sera necesario poner uno por uno las funciones
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
