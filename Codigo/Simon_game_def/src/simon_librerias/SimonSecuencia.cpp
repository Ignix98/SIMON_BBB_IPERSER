
#include "SimonSecuencia.hpp"

namespace BBB {

SimonSecuencia::SimonSecuencia(int max_niveles)
    : max_niveles(max_niveles)
{
    srand(time(NULL));
    secuencia.clear();
}

void SimonSecuencia::reiniciar() {
    secuencia.clear();
}

void SimonSecuencia::añadirColor() {
    if (secuencia.size() < max_niveles) {
        int nuevo_color = rand() % 4; // 0 a 3 (ROJO, VERDE, AMARILLO, AZUL)
        secuencia.push_back(nuevo_color);
    }
}

int SimonSecuencia::getColor(int indice) const {
    if (indice >= 0 && indice < secuencia.size()) {
        return secuencia[indice];
    }
    return -1; // Valor inválido
}

int SimonSecuencia::getNivelActual() const {
    return secuencia.size();
}

int SimonSecuencia::getNivelMax() const { return max_niveles; }

const std::vector<int>& SimonSecuencia::getSecuencia() const {
    return secuencia;
}

}
