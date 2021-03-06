#include "Escena.h"
#include <map>

#ifndef ESCENA_CPP
#define ESCENA_CPP

ostream &operator<<(ostream& out, const Escena &e) {
	e.Imprimir(out);
	return out;
}

Escena::Escena() {
	this->numero = 0;
	this->nombre = "";
	this->cambios = new ListaImp<Referencia<Cambio>>();
}

Escena::Escena(unsigned int nro, Cadena nombre) {
	this->numero = nro;
	this->nombre = nombre;
	this->cambios = new ListaImp<Referencia<Cambio>>();
}

Escena::Escena(const Escena &e) {
	this->numero = e.numero;
	this->nombre = e.nombre;
	Iterador<Referencia<Cambio>> it = e.cambios->GetIterador();
	cambios = new ListaImp<Referencia<Cambio>>();
	while (!it.EsFin()) {
		cambios->AgregarFin(Referencia<Cambio>(it.ElementoInseguro()));
		it.Resto();
	}
	it.Principio();
}

Escena & Escena::operator=(const Escena &e) {
	if (this != &e) {
		// NO IMPLEMENTADA
	}
	return *this;
}

Escena::~Escena() {
	delete cambios;
}

bool Escena::operator==(const Escena &e) const {
	// NO IMPLEMENTADA
	return false;
}

void Escena::Imprimir(ostream& out) const {
	cout << numero << ": " << nombre << endl;
}

unsigned int Escena::GetNro() const {
	// NO IMPLEMENTADA
	return numero;
}

Cadena Escena::GetNombre() const {
	// NO IMPLEMENTADA
	return nombre;
}

void Escena::ImprimirCambios() const {
	if (cambios->EsVacia()) cout << "--No hay cambios--" << endl;
	Iterador<Referencia<Cambio>> it = cambios->GetIterador();
	while (!it.EsFin()) {
		Cambio c = it.ElementoInseguro().GetDato();
		if (c.getTipo() == LUZ) {
			cout << "- Luz: ";
			dynamic_cast<Luz*>(c.getDispositivo())->Imprimir();
		} else if (c.getTipo() == ARTEFACTO) {
			cout << "- Artefacto: ";
			dynamic_cast<Artefacto*>(c.getDispositivo())->Imprimir();
		} else if (c.getTipo() == ALARMA) {
			cout << "- Alarma: ";
			dynamic_cast<Alarma*>(c.getDispositivo())->Imprimir();
		}
		it.Resto();
	}
}

void Escena::Ejecutar(CasaInteligente *casa) const {
	Iterador<Referencia<Cambio>> it = cambios->GetIterador();
	while (!it.EsFin()) {
		Referencia<Cambio> ref = it.ElementoInseguro();
		//Delego al ejecutar de cambio, %%No delega al de cada dispositivo, lo hace todo el%%
		ref.GetDato().Ejecutar(casa);
		it.Resto();
	}
}

void Escena::AgregarCambio(const Cambio &c) {
	Referencia<Cambio> r(c);
	cambios->AgregarFin(r);
}

bool Escena::EsRara() const {
	/*Paso a hash donde la key es el numero y el valor es la cantidad de elementos que encontro,
	recorro cada hash y si hay una key con valor mayor o igual a 2 quiere decir que habian dos 
	cambios con el mismo dispositivo, devuelvo true*/
	int contAlarma = 0;
	map<Cadena, int> luces;
	map<Cadena, int> artefactos;

	Iterador<Referencia<Cambio>> it = cambios->GetIterador();
	while (!it.EsFin()) {
		Referencia<Cambio> ref = it.ElementoInseguro();
		if (ref.GetDato().getTipo() == ALARMA) {
			contAlarma++;
		} else if (ref.GetDato().getTipo() == LUZ) {
			if (luces.find(dynamic_cast<Luz *>(ref.GetDato().getDispositivo())->GetNombre()) != luces.end()) {//Si existe la key
				luces[dynamic_cast<Luz *>(ref.GetDato().getDispositivo())->GetNombre()] += 1;
			} else {
				luces[dynamic_cast<Luz *>(ref.GetDato().getDispositivo())->GetNombre()] = 1;
			}
		}if (ref.GetDato().getTipo() == ARTEFACTO) {
			if (artefactos.find(dynamic_cast<Artefacto *>(ref.GetDato().getDispositivo())->GetNombre()) != artefactos.end()) {//Si existe la key
				artefactos[dynamic_cast<Artefacto *>(ref.GetDato().getDispositivo())->GetNombre()] += 1;

			} else {
				artefactos[dynamic_cast<Artefacto *>(ref.GetDato().getDispositivo())->GetNombre()] = 1;
			}
		}
		it.Resto();
	}
	for (auto iterator = luces.begin(); iterator != luces.end(); iterator++) {
		if(iterator->second >= 2) return true;
	}

	for (auto iterator = artefactos.begin(); iterator != artefactos.end(); iterator++) {
		if (iterator->second >= 2) return true;
	}

	return contAlarma >= 2;
}
#endif

