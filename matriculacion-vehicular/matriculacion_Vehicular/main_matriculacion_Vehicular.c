#include "matriculacion_Vehicular.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
	int opcionInicio;
	char entrada[10];
	
	// Menu de registro e inicio de sesion
	do {
		printf("\n=== BIENVENIDO AL SISTEMA DE MATRICULACION ===\n");
		printf("1. Registrarse\n");
		printf("2. Iniciar sesion\n");
		printf("Seleccione una opcion: ");
		scanf("%9s", entrada);
		limpiarPantalla();
		if (strlen(entrada) == 1 && (entrada[0] == '1' || entrada[0] == '2')) {
			opcionInicio = entrada[0] - '0';
		} else {
			printf("Opcion invalida. Intente nuevamente.\n");
			opcionInicio = 0;
		}
		
		if (opcionInicio == 1) {
			registrarUsuario();
		}
	} while (opcionInicio != 2);
	
	if (!login()) return 0;
	
	// Menu principal
	int opcion;
	do {
		opcion = menuPrincipal();
		limpiarPantalla();
		switch (opcion) {
		case 1:
			registrarVehiculo();
			break;
		case 2:
			registrarRTV();
			break;
		case 3:
			calcularMatricula();
			break;
		case 4: {
			char placa[8];
			printf("Ingrese placa: ");
			scanf("%7s", placa);
			int pos = buscarVehiculo(placa);
			if (pos == -1) {
				printf("Vehiculo no encontrado.\n");
			} else {
				Vehiculo v = vehiculos[pos];
				printf("\n--- DATOS DEL VEHICULO ---\n");
				printf("Placa: %s\n", v.placa);
				printf("Marca: %s\n", v.marca);
				printf("Modelo: %s\n", v.modelo);
				printf("Anio: %d\n", v.anio);
				printf("Chasis: %s\n", v.chasis);
				printf("Motor: %s\n", v.numerodelmotor);
				printf("Servicio: %s\n", (v.servicio[0] == 'C') ? "COMERCIAL" : "PARTICULAR");
				printf("Cedula: %s\n", v.cedula);
				printf("Nuevo: %c\n", v.esNuevo);
				printf("Exonerado: %c\n", v.esExonerado);
			}
			break;
		}
		case 5:
			listarVehiculos();
			break;
		case 6:
			printf("Saliendo del sistema...\n");
			break;
		default:
			printf("Opcion invalida.\n");
		}
	} while (opcion != 6);
	
	return 0;
}


