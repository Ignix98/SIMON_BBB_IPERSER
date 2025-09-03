
#ifndef SIMON_SECUENCIA_HPP_
#define SIMON_SECUENCIA_HPP_

#include <vector>
#include <cstdlib>
#include <ctime>

namespace BBB {

/**
 * Clase que gestiona la generación y verificación de la secuencia del juego Simon.
 */
class SimonSecuencia {
public:
    SimonSecuencia(int max_niveles = 20);

    // Reinicia la secuencia
    void reiniciar();

    // Añade un nuevo color aleatorio a la secuencia
    void añadirColor();

    // Devuelve el color esperado en una posición
    int getColor(int indice) const;

    // Devuelve la longitud actual de la secuencia
    int getNivelActual() const;

    // Devuelve el nivel maximo
    int getNivelMax() const;

    // Devuelve la secuencia completa
    const std::vector<int>& getSecuencia() const;

private:
    std::vector<int> secuencia;
    int max_niveles;
};

} // namespace BBB

#endif
