#ifndef MATRICULACION_VEHICULAR_H
#define MATRICULACION_VEHICULAR_H

#include <string.h>
#include <ctype.h>

// Constantes generales
#define MAX_VEHICULOS 100
#define MAX_RTV 100
#define MAX_INTENTOS_LOGIN 3

// Estructura de un vehiculo
typedef struct {
	char placa[8];
	char marca[20];
	char modelo[20];
	int anio;
	char chasis[25];
	char numerodelmotor[25];
	char servicio[2];     // "P" o "C"
	char cedula[15];
	char esNuevo;
	char esExonerado;
} Vehiculo;

// Estructura de una revision tecnica vehicular
typedef struct {
	char placa[8];
	char fecha[11];
	char centro[30];
	char tecnico[20];
	char resultado[2]; // "A" o "R"
	char observacion[100];
} RTV;

// Variables globales
extern Vehiculo vehiculos[MAX_VEHICULOS];
extern int totalVehiculos;
extern RTV revisiones[MAX_RTV];
extern int totalRTV;

// Menu y funcionalidades
int menuPrincipal();
void registrarVehiculo();
void registrarRTV();
void calcularMatricula();
void listarVehiculos();
int buscarVehiculo(char placa[8]);

// Validaciones
int validarPlaca(char placa[]);
int validarAnio(int anio);
int validarCadena(char *cadena);
int validarCedula(char cedula[]);
int validarServicio(char *servicio);
int validarSN(char c);
int validarResultado(char *r);
void normalizarCadena(char *str);

// Entrada de texto validada
void inputTextoValidado(const char *mensaje, int (*validador)(char *), char *destino, int max);
void limpiarPantalla();

// Usuarios
int registrarUsuario();
void registrarUsuarioConCedula(char *cedula);
int usuarioExiste(char *cedula);
int verificarCredenciales(char *cedula, char *clave);
int login();

#endif

