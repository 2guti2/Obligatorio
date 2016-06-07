#include "CasaInteligente.h"



#ifndef CASAINTELIGENTE_CPP
#define CASAINTELIGENTE_CPP


CasaInteligente::CasaInteligente() {
	lucesNumero = new ABBImp<Asociacion<int, Referencia<Luz>>>();
	lucesNombre = new ABBImp<Asociacion<Cadena, Referencia<Luz>>>();
	artefactosNumero = new ABBImp<Asociacion<int, Referencia<Artefacto>>>();
	artefactosNombre = new ABBImp<Asociacion<Cadena, Referencia<Artefacto>>>();
	escenasNumero = new ABBImp<Asociacion<int, Referencia<Escena>>>();
	escenasNombre = new ABBImp<Asociacion<Cadena, Referencia<Escena>>>();
	alarma = new Referencia<Alarma>(Alarma());
	sensores = new ABBImp<Asociacion<int, Referencia<Sensor>>>();
	enEscena = false;
	escenaActual = new Referencia<Escena>(Escena());
}

CasaInteligente::CasaInteligente(unsigned int CANT_SENSORES) {
	lucesNumero = new ABBImp<Asociacion<int, Referencia<Luz>>>();
	lucesNombre = new ABBImp<Asociacion<Cadena, Referencia<Luz>>>();
	artefactosNumero = new ABBImp<Asociacion<int, Referencia<Artefacto>>>();
	artefactosNombre = new ABBImp<Asociacion<Cadena, Referencia<Artefacto>>>();
	escenasNumero = new ABBImp<Asociacion<int, Referencia<Escena>>>();
	escenasNombre = new ABBImp<Asociacion<Cadena, Referencia<Escena>>>();
	alarma = new Referencia<Alarma>(Alarma());
	sensores = new ABBImp<Asociacion<int, Referencia<Sensor>>>();
	enEscena = false;
	escenaActual = new Referencia<Escena>(Escena());
	for (unsigned int i = 1; i <= CANT_SENSORES; i++) {
		Sensor s(i);
		Referencia<Sensor> r(s);
		Asociacion<int, Referencia<Sensor>> a(i, r);
		sensores->Insertar(a);
	}
}

CasaInteligente::CasaInteligente(const CasaInteligente &casa) {
	assert(false);
}

CasaInteligente::~CasaInteligente() {
	// NO IMPLEMENTADA->>>HACER
}

CasaInteligente &CasaInteligente::operator=(const CasaInteligente &casa) {
	if (this != &casa) {
		// NO IMPLEMENTADA
	}
	return *this;
}

bool CasaInteligente::operator==(const CasaInteligente &casa) const {
	// NO IMPLEMENTADA
	return false;
}

bool CasaInteligente::operator<(const CasaInteligente &casa) const {
	// NO IMPLEMENTADA
	return false;
}

bool CasaInteligente::isEnEscena() {
	return this->enEscena;
}

TipoRetorno CasaInteligente::AgregarLuz(unsigned int nroLuz, Cadena nombre) {
	if (!enEscena) {
		Asociacion<int, Referencia<Luz>> asocNro = Asociacion<int, Referencia<Luz>>(nroLuz, Referencia<Luz>(Luz(nroLuz, nombre)));
		Asociacion<Cadena, Referencia<Luz>> asocNombre = Asociacion<Cadena, Referencia<Luz>>(nombre, Referencia<Luz>(Luz(nroLuz, nombre)));
		if (lucesNumero->Existe(asocNro)) {
			cout << "ERROR:	Ya	existe	una	luz	con	el	mismo	numero.	" << endl;
			return ERROR;
		} else if (lucesNombre->Existe(asocNombre)) {
			cout << "ERROR: Ya	existe	una	luz	con	el	mismo	nombre.	" << endl;
			return ERROR;
		} else {
			lucesNumero->Insertar(asocNro);
			lucesNombre->Insertar(asocNombre);
			return OK;
		}
	} else {
		cout << "ERROR: Fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::AgregarArtefacto(unsigned int nroArt, Cadena nombre) {
	if (!enEscena) {
		Asociacion<int, Referencia<Artefacto>> asocNro = Asociacion<int, Referencia<Artefacto>>(nroArt, Referencia<Artefacto>(Artefacto(nroArt, nombre)));
		Asociacion<Cadena, Referencia<Artefacto>> asocNombre = Asociacion<Cadena, Referencia<Artefacto>>(nombre, Referencia<Artefacto>(Artefacto(nroArt, nombre)));
		if (artefactosNumero->Existe(asocNro)) {
			cout << "ERROR:	Ya	existe	un artefacto	con	el	mismo	numero.	" << endl;
			return ERROR;
		} else if (artefactosNombre->Existe(asocNombre)) {
			cout << "ERROR: Ya	existe	un artefacto	con	el	mismo	nombre.	" << endl;
			return ERROR;
		} else {
			artefactosNumero->Insertar(asocNro);
			artefactosNombre->Insertar(asocNombre);
			return OK;
		}
	} else {
		cout << "ERROR: Fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::CambiarEstadoLuz(unsigned int nroLuz, unsigned int porcentaje) {
	//unos bypass te dan error y otros como este te lo agregan a la escena
	if (porcentaje <= 100) {
		if (!enEscena) {
			Asociacion<int, Referencia<Luz>> e = lucesNumero->fetch(Asociacion<int, Referencia<Luz>>(nroLuz, Referencia<Luz>(Luz())));
			if (e == Asociacion<int, Referencia<Luz>>()) {
				cout << "ERROR: No existe una luz con ese numero." << endl;
				return ERROR;
			}
			e.GetRango().GetDato().SetIntensidad(porcentaje);
			return OK;
		} else {
			Asociacion<int, Referencia<Luz>> e = lucesNumero->fetch(Asociacion<int, Referencia<Luz>>(nroLuz, Referencia<Luz>(Luz())));
			Luz l(e.GetRangoInseguro().GetDato());
			l.SetIntensidad(porcentaje);
			escenaActual->GetDato().AgregarCambio(Cambio(l));
			return OK;
		}
	} else {
		cout << "ERROR: El porcentaje debe ser menor o igual a 100" << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::CambiarEstadoArtefacto(unsigned int nroArt, EstadoArtefacto nuevoEstado) {
	Asociacion<int, Referencia<Artefacto>> e = artefactosNumero->fetch(Asociacion<int, Referencia<Artefacto>>(nroArt, Referencia<Artefacto>(Artefacto())));
	if (e == Asociacion<int, Referencia<Artefacto>>()) {
		cout << "ERROR: No existe un artefacto con ese numero." << endl;
		return ERROR;
	}
	if (enEscena) {
		Artefacto a(e.GetRangoInseguro().GetDato());
		a.SetEstado(nuevoEstado);
		escenaActual->GetDato().AgregarCambio(Cambio(a));
	} else {
		e.GetRango().GetDato().SetEstado(nuevoEstado);
	}
	return OK;
}

TipoRetorno CasaInteligente::CambiarEstadoAlarma(EstadoAlarma nuevoEstado) {
	if (this->alarma->GetDato().GetEstado() == nuevoEstado) {
		cout << "ERROR:	La	alarma	ya	se	encuentra	en	el	nuevo	estado." << endl;
		return ERROR;
	} else if (this->puedoCambiarAlarma(sensores->getRaiz()) || nuevoEstado == DESACTIVADA) {
		if (enEscena) {
			Alarma a = Alarma(alarma->GetDato());
			a.SetEstado(nuevoEstado);
			escenaActual->GetDato().AgregarCambio(Cambio(a));
		} else {
			this->alarma->GetDato().SetEstado(nuevoEstado);
		}
		return OK;
	} else {
		cout << "ERROR:	No	se	puede	activar,	hay	uno	o	mas	sensores	ENALARMA." << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::ImprimirEstadoCasa() const {
	if (!enEscena) {
		cout << "Estado de la casa:" << endl;
		cout << this->alarma->GetDato();//La referencia se interponia con la alarma. No se imprimia
		cout << "- Sensores:" << endl;
		NodoLista <Asociacion<int, Referencia<Sensor>>> * lSensores = NULL;
		sensores->aNodoLista(lSensores);
		while (lSensores != NULL) {
			cout << lSensores->dato;
			lSensores = lSensores->sig;
		}
		cout << "- Luces:" << endl;
		NodoLista<Asociacion<int, Referencia<Luz>>> * lLuces = NULL;
		lucesNumero->aNodoLista(lLuces);
		if (lLuces == NULL) cout << "--No hay luces--" << endl;
		while (lLuces != NULL) {
			cout << lLuces->dato;
			lLuces = lLuces->sig;
		}
		cout << "- Artefactos:" << endl;
		NodoLista<Asociacion<int, Referencia<Artefacto>>> * lArtefactos = NULL;
		artefactosNumero->aNodoLista(lArtefactos);
		if (lArtefactos == NULL) cout << "--No hay artefactos--" << endl;
		while (lArtefactos != NULL) {
			cout << lArtefactos->dato;
			lArtefactos = lArtefactos->sig;
		}

		return OK;
	} else {
		cout << "ERROR: Fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::CrearCondicion(unsigned int nroCondicion, void(*seCumpleCondicion)(int), void(*seDejaDeCumplirCondicion)(int)) {
	return NO_IMPLEMENTADA;
}

TipoRetorno CasaInteligente::AgregarSensorACondicion(unsigned int nroCondicion, unsigned int nroSensor, EstadoSensor estado) {
	return NO_IMPLEMENTADA;
}

TipoRetorno CasaInteligente::CambiarEstadoSensor(unsigned int nroSensor, EstadoSensor estado) {
	if (!enEscena) {
		Asociacion<int, Referencia<Sensor>> e = sensores->fetch(Asociacion<int, Referencia<Sensor>>(nroSensor, Referencia<Sensor>(Sensor())));
		if (e == Asociacion<int, Referencia<Sensor>>()) {
			cout << "ERROR: No existe un sensor con ese numero." << endl;
			return ERROR;
		}
		e.GetRango().GetDato().SetEstado(estado);
		return OK;
	} else {
		cout << "ERROR: Fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::InicioEscena(unsigned int nroEscena, Cadena nombre) {
	if (!this->enEscena) {
		if (escenasNombre->fetch(Asociacion<Cadena, Referencia<Escena>>(nombre, Referencia<Escena>(Escena())))
			!= Asociacion<Cadena, Referencia<Escena>>()) {
			cout << "ERROR:	Ya	existe	una	escena	con	el	mismo	nombre." << endl;
			return ERROR;
		} else if (escenasNumero->fetch(Asociacion<int, Referencia<Escena>>(nroEscena, Referencia<Escena>(Escena())))
			!= Asociacion<int, Referencia<Escena>>()) {
			cout << "ERROR:	Ya	existe	una	escena	con	el	mismo	numero." << endl;
			return ERROR;
		} else {
			this->enEscena = true;
			this->escenaActual = new Referencia<Escena>(Escena(nroEscena, nombre));
			return OK;
		}

	} else {
		cout << "ERROR:	Fue	iniciada	la	grabacion	de	una	escena	" << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::FinEscena() {
	if (!enEscena) {
		cout << "ERROR: No fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	}
	enEscena = false;
	escenasNombre->Insertar(Asociacion<Cadena, Referencia<Escena>>(escenaActual->GetDato().GetNombre(), Referencia<Escena>(escenaActual->GetDato())));
	escenasNumero->Insertar(Asociacion<int, Referencia<Escena>>(escenaActual->GetDato().GetNro(), Referencia<Escena>(escenaActual->GetDato())));
	escenaActual = NULL;
	return OK;
}

TipoRetorno CasaInteligente::EjecutarEscena(unsigned int nroEscena) {
	if (enEscena) {
		cout << "ERROR: Fue iniciada la grabacion de una escena." << endl;
		return ERROR;
	}
	Asociacion<int, Referencia<Escena>> escena = escenasNumero->fetch(Asociacion<int, Referencia<Escena>>(nroEscena, Referencia<Escena>()));
	if (escena != Asociacion<int, Referencia<Escena>>()) {
		escena.GetRango().GetDato().Ejecutar(this);
	} else {
		cout << "ERROR: No existe una escena con ese numero." << endl;
		return ERROR;
	}

	return OK;
}

TipoRetorno CasaInteligente::ImprimirEscenas() const {
	if (enEscena) {
		cout << "ERROR: Fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	} else {
		NodoLista<Asociacion<int, Referencia<Escena>>> * lEscenas = NULL;
		this->escenasNumero->aNodoLista(lEscenas);
		cout << "Escenas:" << endl;
		if (lEscenas == NULL) cout << "--No hay escenas--" << endl;
		while (lEscenas != NULL) {
			cout << lEscenas->dato;
			lEscenas = lEscenas->sig;
		}
		return OK;
	}
}

TipoRetorno CasaInteligente::ImprimirEscena(unsigned int nroEscena) const {
	Asociacion<int, Referencia<Escena>> escena = escenasNumero->fetch(Asociacion<int, Referencia<Escena>>(nroEscena, Referencia<Escena>()));
	if (escena != Asociacion<int, Referencia<Escena>>()) {
		if (enEscena) {
			cout << "ERROR: Fue iniciada la grabacion de una escena." << endl;
			return ERROR;
		}
		cout << "Escena " << escena.GetDominio() << " - " << escena.GetRangoInseguro().GetDato().GetNombre() << ":" << endl;
		escena.GetRango().GetDato().ImprimirCambios();
		return OK;
	} else {
		cout << "ERROR: No existe una escena con ese numero." << endl;
		return ERROR;
	}
}

TipoRetorno CasaInteligente::ImprimirEscenasRaras() const {
	if (enEscena) {
		cout << "ERROR: Fue iniciada la grabacion de una escena" << endl;
		return ERROR;
	} else {
		NodoLista<Asociacion<int, Referencia<Escena>>> * lEscenas = NULL;
		this->escenasNumero->aNodoLista(lEscenas);
		cout << "Escenas raras:" << endl;
		if (lEscenas == NULL) cout << "--No hay escenas raras--" << endl;
		while (lEscenas != NULL) {
			if (lEscenas->dato.GetRangoInseguro().GetDato().EsRara())
				cout << lEscenas->dato;
			lEscenas = lEscenas->sig;
		}
		return OK;
	}
}

//aux

bool CasaInteligente::puedoCambiarAlarma(NodoABB<Asociacion<int, Referencia<Sensor>>>* sens) {
	if (sens == NULL) return true;
	else if (sens->dato.GetRangoInseguro().GetDato().GetEstado() == NORMAL)
		return true && puedoCambiarAlarma(sens->hDer) && puedoCambiarAlarma(sens->hIzq);
	else  return false;
}

#endif
