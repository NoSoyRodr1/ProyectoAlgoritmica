#include <iostream>
#include <string>

using namespace std;

// Lógica de las acciones que puede realizar el cliente
void OpcsDCliente() 
{
    string opt;
    do 
    {
        cout<<"Bienvenido, cliente. ¿Qué acción desea realizar?:\n";
        cout<<"1) Hacer una reserva\n";
        cout<<"2) Hacer un pedido de comida\n";
        cout<<"3) Aumentar el plazo de estancia\n";
        cout<<"4) Retroceder al menú principal\n";
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
            cout<<"\nVolviendo al menú principal...\n\n";
            // Vuele al menú porque sigue siendo la funcion principal despues de este void
        } 
        else 
        {
            cout<<"\nOpción inválida.\n\n";
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
        cout<<"Bienvenido, personal del hotel. ¿Qué acción desea realizar?:\n";
        cout<<"1) Realizar check-in\n";
        cout<<"2) Realizar check-out\n";
        cout<<"3) Retroceder al menú principal\n";
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
            cout<<"\nVolviendo al menú principal...\n\n";
            // Vuele al menú porque sigue siendo la funcion principal despues de este void
        } 
        else 
        {
            cout<<"\nOpción inválida.\n\n";
        }
    } 
    while (opt!="3");
}

// Inicio donde se pregunta la identificación del usuario para mostrarle las opciones disponibles
int main() 
{
    string choice;
    string correo;
    string contraseña;

    do 
    {
        cout<<"Bienvenido a nuestro sitio oficial del Hotel Tal.\n";
        cout<<"¿Es cliente o es de nuestro personal?\n";
        cout<<"1) Cliente\n";
        cout<<"2) Personal\n";
        cout<<"3) Salir\n";
        cin>>choice;

        if (choice=="1") 
        {
            cout<<"Por favor ingrese su correo y contraseña para continuar.\n";
            // Supongo que aqui haran los correos y contraseñas validos para ingresar
            cout<<"Correo: ";
            cin>>correo;
            cout<<"Contraseña: ";
            cin>>contraseña;
            cout<<"\n";
            // Redirige a las opciones para el cliente
            OpcsDCliente();
        } 
        else if (choice=="2") 
        {
            cout<<"Por favor ingrese su correo y contraseña para continuar.\n";
            // Supongo que aqui haran los correos y contraseñas validos para ingresar
            cout<<"Correo: ";
            cin>>correo;
            cout<<"Contraseña: ";
            cin>>contraseña;
            cout<<"\n";
            // Redirige a las opciones para el personal
            OpcDPersonal();
        } 
        else if (choice=="3") 
        {
            cout<<"\n¡Muchas gracias por visitarnos!\n";
        } 
        else 
        {
            cout<<"\nInválido.\n\n";
        }
    } 
    while (choice!="3");
    // utilice switch pero no era tan eficiente a parte que me botaba error, asi que termino siendo con if y else, pero ta bonito

    return 0;
}
