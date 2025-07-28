#include "matriculacion_Vehicular.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// Variables globales
Vehiculo vehiculos[MAX_VEHICULOS];
int totalVehiculos = 0;
RTV revisiones[MAX_RTV];
int totalRTV = 0;

// ===================== VALIDACIONES =====================

int validarPlaca(char placa[]) {
	return strlen(placa) == 7 &&
		isalpha(placa[0]) && isalpha(placa[1]) && isalpha(placa[2]) &&
		isdigit(placa[3]) && isdigit(placa[4]) && isdigit(placa[5]) && isdigit(placa[6]);
}

int validarAnio(int anio) {
	return anio >= 1900 && anio <= 2025;
}

int validarCadena(char *cadena) {
	return strlen(cadena) > 0;
}

int validarCedula(char cedula[]) {
	if (strlen(cedula) != 10) return 0;
	for (int i = 0; i < 10; i++) {
		if (!isdigit(cedula[i])) return 0;
	}
	return 1;
}

int validarServicio(char *servicio) {
	char s = toupper(servicio[0]);
	return (s == 'P' || s == 'C');
}

int validarSN(char c) {
	c = toupper(c);
	return (c == 'S' || c == 'N');
}

int validarResultado(char *r) {
	return (toupper(r[0]) == 'A' || toupper(r[0]) == 'R');
}

void normalizarCadena(char *str) {
	for (int i = 0; str[i]; i++) {
		str[i] = toupper(str[i]);
	}
}

void inputTextoValidado(const char *mensaje, int (*validador)(char *), char *destino, int max) {
	char entrada[100];
	do {
		printf("%s", mensaje);
		scanf("%99s", entrada);
		if (!validador(entrada)) {
			printf("Entrada invalida. Intente nuevamente.\n");
		}
	} while (!validador(entrada));
	strncpy(destino, entrada, max);
}
// ===================== USUARIOS =====================

int usuarioExiste(char *cedula) {
	FILE *f = fopen("usuarios.txt", "r");
	if (!f) return 0;
	
	char c[20], clave[30];
	while (fscanf(f, "%19s %29s", c, clave) == 2) {
		if (strcmp(c, cedula) == 0) {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

void registrarUsuarioConCedula(char *cedula) {
	char clave[20];
	strcpy(clave, cedula);
	strcat(clave, "001");
	
	FILE *f = fopen("usuarios.txt", "a");
	if (!f) return;
	fprintf(f, "%s %s\n", cedula, clave);
	fclose(f);
	
	printf("Usuario registrado correctamente. Su clave es: %s\n", clave);
}

int registrarUsuario() {
	char cedula[20];
	printf("\n=== REGISTRO DE NUEVO USUARIO ===\n");
	do {
		printf("Ingrese cedula (10 digitos): ");
		scanf("%19s", cedula);
		if (!validarCedula(cedula)) {
			printf("Cedula invalida. Intente nuevamente.\n");
		}
	} while (!validarCedula(cedula));
	
	if (usuarioExiste(cedula)) {
		printf("Este usuario ya esta registrado.\n");
		return 0;
	}
	
	registrarUsuarioConCedula(cedula);
	return 1;
}

int verificarCredenciales(char *cedula, char *clave) {
	FILE *f = fopen("usuarios.txt", "r");
	if (!f) return 0;
	
	char c[20], k[30];
	while (fscanf(f, "%19s %29s", c, k) == 2) {
		if (strcmp(c, cedula) == 0 && strcmp(k, clave) == 0) {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

int login() {
	char cedula[20], clave[30];
	int intentos = 0;
	
	while (intentos < MAX_INTENTOS_LOGIN) {
		printf("\n=== INICIO DE SESION ===\n");
		printf("Cedula (usuario): ");
		scanf("%19s", cedula);
		
		if (!validarCedula(cedula)) {
			printf("Cedula invalida. Intente nuevamente.\n");
			continue;
		}
		
		if (!usuarioExiste(cedula)) {
			printf("Usuario no registrado. Procediendo al registro...\n");
			registrarUsuarioConCedula(cedula);
			continue;
		}
		
		printf("Clave: ");
		scanf("%29s", clave);
		
		if (verificarCredenciales(cedula, clave)) {
			printf("Acceso concedido.\n");
			return 1;
		} else {
			printf("Credenciales incorrectas. Intentos restantes: %d\n", MAX_INTENTOS_LOGIN - ++intentos);
		}
	}
	
	printf("Demasiados intentos fallidos.\n");
	return 0;
}

// ===================== MENU PRINCIPAL =====================

void limpiarPantalla() {
#ifdef _WIN32
	system("cls");  // Windows
#else
	system("clear");  // Linux/Mac
#endif
}


int menuPrincipal() {
	int opcion = 0;
	char entrada[10];
	
	do {
		printf("\n=== MENU PRINCIPAL ===\n");
		printf("1. Registrar vehiculo\n");
		printf("2. Registrar revision RTV\n");
		printf("3. Calcular matricula\n");
		printf("4. Buscar vehiculo por placa\n");
		printf("5. Listar vehiculos\n");
		printf("6. Salir\n");
		printf("Seleccione una opcion (1-6): ");
		scanf("%9s", entrada);
		
		if (strlen(entrada) == 1 && entrada[0] >= '1' && entrada[0] <= '6') {
			opcion = entrada[0] - '0';
		} else {
			printf("Opcion invalida. Intente nuevamente.\n");
		}
	} while (opcion < 1 || opcion > 6);
	
	return opcion;
}

// ===================== FUNCIONES VEHICULOS Y RTV =====================

int buscarVehiculo(char placa[8]) {
	normalizarCadena(placa);
	for (int i = 0; i < totalVehiculos; i++) {
		if (strcmp(vehiculos[i].placa, placa) == 0) {
			return i;
		}
	}
	return -1;
}

void registrarVehiculo() {
	if (totalVehiculos >= MAX_VEHICULOS) {
		printf("Capacidad maxima alcanzada.\n");
		return;
	}
	
	Vehiculo v;
	char entrada[100];
	int anioTemp;
	
	// Leer placa
	do {
		printf("Placa (ABC1234): ");
		scanf("%99s", entrada);
		normalizarCadena(entrada);
	} while (!validarPlaca(entrada));
	strncpy(v.placa, entrada, 8);
	
	// Limpiar buffer
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer marca
	printf("Marca: ");
	fgets(entrada, sizeof(entrada), stdin);
	entrada[strcspn(entrada, "\n")] = '\0';
	while (!validarCadena(entrada)) {
		printf("Marca invalida. Intente nuevamente: ");
		fgets(entrada, sizeof(entrada), stdin);
		entrada[strcspn(entrada, "\n")] = '\0';
	}
	strncpy(v.marca, entrada, 20);
	
	// Leer modelo
	printf("Modelo: ");
	fgets(entrada, sizeof(entrada), stdin);
	entrada[strcspn(entrada, "\n")] = '\0';
	while (!validarCadena(entrada)) {
		printf("Modelo invalido. Intente nuevamente: ");
		fgets(entrada, sizeof(entrada), stdin);
		entrada[strcspn(entrada, "\n")] = '\0';
	}
	strncpy(v.modelo, entrada, 20);
	
	// Leer anio
	do {
		printf("Anio (1900-2025): ");
		scanf("%99s", entrada);
		anioTemp = atoi(entrada);
	} while (!validarAnio(anioTemp));
	v.anio = anioTemp;
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer numero de chasis
	printf("Numero de chasis(1G1RC6E42BUXXXXXX): ");
	fgets(entrada, sizeof(entrada), stdin);
	entrada[strcspn(entrada, "\n")] = '\0';
	strncpy(v.chasis, entrada, 25);
	
	// Leer numero de motor
	printf("Numero de motor(M123ABC): ");
	fgets(entrada, sizeof(entrada), stdin);
	entrada[strcspn(entrada, "\n")] = '\0';
	strncpy(v.numerodelmotor, entrada, 25);
	
	// Leer servicio
	do {
		printf("Servicio (P/C): ");
		scanf("%99s", entrada);
		normalizarCadena(entrada);
	} while (!validarServicio(entrada));
	strncpy(v.servicio, entrada, 2);
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer cedula
	do {
		printf("Cedula (10 digitos): ");
		scanf("%99s", entrada);
	} while (!validarCedula(entrada));
	strncpy(v.cedula, entrada, 15);
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer si es nuevo
	do {
		printf("Es nuevo? (S/N): ");
		scanf("%99s", entrada);
		v.esNuevo = toupper(entrada[0]);
	} while (!validarSN(v.esNuevo));
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer si esta exonerado
	do {
		printf("Esta exonerado? (S/N): ");
		scanf("%99s", entrada);
		v.esExonerado = toupper(entrada[0]);
	} while (!validarSN(v.esExonerado));
	
	vehiculos[totalVehiculos++] = v;
	printf("Vehiculo registrado correctamente.\n");
}


void registrarRTV() {
	if (totalRTV >= MAX_RTV) {
		printf("Limite de RTV alcanzado.\n");
		return;
	}
	
	RTV r;
	char entrada[100];
	int ch;
	
	// Leer placa del vehiculo
	do {
		printf("Placa del vehiculo: ");
		scanf("%99s", entrada);
		normalizarCadena(entrada);
	} while (!validarPlaca(entrada));
	strncpy(r.placa, entrada, 8);
	
	if (buscarVehiculo(r.placa) == -1) {
		printf("Vehiculo no registrado.\n");
		return;
	}
	
	// Fecha actual automatica
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	strftime(r.fecha, 11, "%d/%m/%Y", tm_info);
	printf("Fecha: %s\n", r.fecha);
	
	// Limpiar buffer antes de fgets()
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer centro
	printf("Centro: ");
	fgets(entrada, sizeof(entrada), stdin);
	entrada[strcspn(entrada, "\n")] = '\0';
	while (!validarCadena(entrada)) {
		printf("Centro invalido. Intente nuevamente: ");
		fgets(entrada, sizeof(entrada), stdin);
		entrada[strcspn(entrada, "\n")] = '\0';
	}
	strncpy(r.centro, entrada, 30);
	
	// Leer tecnico
	printf("Tecnico: ");
	fgets(entrada, sizeof(entrada), stdin);
	entrada[strcspn(entrada, "\n")] = '\0';
	while (!validarCadena(entrada)) {
		printf("Tecnico invalido. Intente nuevamente: ");
		fgets(entrada, sizeof(entrada), stdin);
		entrada[strcspn(entrada, "\n")] = '\0';
	}
	strncpy(r.tecnico, entrada, 20);
	
	// Leer resultado
	do {
		printf("Resultado (A/R): ");
		scanf("%99s", entrada);
		normalizarCadena(entrada);
	} while (!validarResultado(entrada));
	strncpy(r.resultado, entrada, 2);
	
	// Limpiar buffer antes de fgets para observaciones
	while ((ch = getchar()) != '\n' && ch != EOF);
	
	// Leer observaciones
	printf("Observaciones: ");
	fgets(r.observacion, sizeof(r.observacion), stdin);
	r.observacion[strcspn(r.observacion, "\n")] = '\0';
	
	revisiones[totalRTV++] = r;
	printf("RTV registrada correctamente.\n");
}


void calcularMatricula() {
	char placa[8];
	printf("Ingrese placa: ");
	scanf("%7s", placa);
	normalizarCadena(placa);
	
	int pos = buscarVehiculo(placa);
	if (pos == -1) {
		printf("Vehiculo no registrado.\n");
		return;
	}
	
	int aprobado = 0;
	for (int i = 0; i < totalRTV; i++) {
		if (strcmp(revisiones[i].placa, placa) == 0 &&
			toupper(revisiones[i].resultado[0]) == 'A') {
			aprobado = 1;
			break;
		}
	}
	
	if (!aprobado) {
		printf("Este vehiculo no tiene RTV aprobada.\n");
		return;
	}
	
	Vehiculo v = vehiculos[pos];
	float base = (v.servicio[0] == 'C') ? 300.0 : 200.0;
	float antiguedad = (2025 - v.anio) * 5.0;
	float tasaAmbiental = (v.servicio[0] == 'C') ? 50.0 : 40.0;
	float tasaRTV = 35.0;
	float descuento = (v.esExonerado == 'S') ? (base + tasaAmbiental) * 0.5 : 0.0;
	float subtotal = base + antiguedad + tasaAmbiental + tasaRTV;
	float total = subtotal - descuento;
	
	printf("\n--- MATRICULA VEHICULAR ---\n");
	printf("Base: $%.2f\n", base);
	printf("Antiguedad: $%.2f\n", antiguedad);
	printf("Tasa ambiental: $%.2f\n", tasaAmbiental);
	printf("Tasa RTV: $%.2f\n", tasaRTV);
	printf("Subtotal: $%.2f\n", subtotal);
	if (descuento > 0) {
		printf("Descuento (50%% por exoneracion): -$%.2f\n", descuento);
	}	
	printf("TOTAL: $%.2f\n", total);
	
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	
	// Crear archivo comprobante
	char nombreArchivo[50];
	strftime(nombreArchivo, sizeof(nombreArchivo), "comprobante_%d-%m-%Y.txt", tm_info);
	
	FILE *fc = fopen(nombreArchivo, "w");
	if (fc) {
		fprintf(fc, "--- COMPROBANTE DE MATRICULA ---\n");
		fprintf(fc, "Fecha de emision: %s\n", nombreArchivo + 12); // parte de la fecha
		fprintf(fc, "Placa: %s\n", v.placa);
		fprintf(fc, "Marca: %s\n", v.marca);
		fprintf(fc, "Modelo: %s\n", v.modelo);
		fprintf(fc, "Anio: %d\n", v.anio);
		fprintf(fc, "Servicio: %s\n", (v.servicio[0] == 'C') ? "COMERCIAL" : "PARTICULAR");
		fprintf(fc, "Cedula: %s\n", v.cedula);
		fprintf(fc, "-------------------------------\n");
		fprintf(fc, "Base: $%.2f\n", base);
		fprintf(fc, "Antiguedad: $%.2f\n", antiguedad);
		fprintf(fc, "Tasa ambiental: $%.2f\n", tasaAmbiental);
		fprintf(fc, "Tasa RTV: $%.2f\n", tasaRTV);
		fprintf(fc, "Subtotal: $%.2f\n", subtotal);
		if (descuento > 0) {
			fprintf(fc, "Descuento (50%% por exoneracion): -$%.2f\n", descuento);
		}
		fprintf(fc, "TOTAL A PAGAR: $%.2f\n", total);
		fprintf(fc, "-------------------------------\n");
		fclose(fc);
		printf("\nComprobante generado: %s\n", nombreArchivo);
	} else {
		printf("Error al generar comprobante.\n");
	}
	
}

void listarVehiculos() {
	printf("\nLISTADO DE VEHICULOS (%d)\n", totalVehiculos);
	if (totalVehiculos == 0) {
		printf("No hay vehiculos registrados.\n");
		return;
	}
	
	// Crear nombre de archivo con fecha
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	char archivoNombre[50];
	strftime(archivoNombre, sizeof(archivoNombre), "listado_vehiculos_%d-%m-%Y.txt", tm_info);
	
	FILE *f = fopen(archivoNombre, "w");
	if (!f) {
		printf("Error al crear el archivo de listado.\n");
		return;
	}
	
	for (int i = 0; i < totalVehiculos; i++) {
		Vehiculo v = vehiculos[i];
		
		// Mostrar en consola
		printf("\nVehiculo #%d\n", i + 1);
		printf("Placa: %s\n", v.placa);
		printf("Marca/Modelo: %s %s\n", v.marca, v.modelo);
		printf("Anio: %d | Servicio: %s\n", v.anio,
			   (v.servicio[0] == 'C') ? "COMERCIAL" : "PARTICULAR");
		printf("Propietario: %s | Nuevo: %c\n", v.cedula, v.esNuevo);
		
		// Guardar en archivo
		fprintf(f, "Vehiculo #%d\n", i + 1);
		fprintf(f, "Placa: %s\n", v.placa);
		fprintf(f, "Marca/Modelo: %s %s\n", v.marca, v.modelo);
		fprintf(f, "Anio: %d | Servicio: %s\n", v.anio,
				(v.servicio[0] == 'C') ? "COMERCIAL" : "PARTICULAR");
		fprintf(f, "Propietario: %s | Nuevo: %c\n", v.cedula, v.esNuevo);
		fprintf(f, "--------------------------------------\n");
	}
	
	fclose(f);
	printf("\nArchivo generado: %s\n", archivoNombre);
}
