#ifndef NOTAS_H
#define NOTAS_H

/*  Escala temperada: A4 = 440 Hz
 *  Notación:  C = do, Cs = do sostenido (C#), D = re, …, B = si
 *  Rango práctico C0 (16 Hz) – C8 (4186 Hz)
 */

constexpr double REST = 0.0;   // silencio

/* --- Octava 0 --- */
constexpr double C0  = 16.35;  constexpr double Cs0 = 17.32;
constexpr double D0  = 18.35;  constexpr double Ds0 = 19.45;
constexpr double E0  = 20.60;  constexpr double F0  = 21.83;
constexpr double Fs0 = 23.12;  constexpr double G0  = 24.50;
constexpr double Gs0 = 25.96;  constexpr double A0  = 27.50;
constexpr double As0 = 29.14;  constexpr double B0  = 30.87;

/* --- Octava 1 --- */
constexpr double C1  = 32.70;  constexpr double Cs1 = 34.65;
constexpr double D1  = 36.71;  constexpr double Ds1 = 38.89;
constexpr double E1  = 41.20;  constexpr double F1  = 43.65;
constexpr double Fs1 = 46.25;  constexpr double G1  = 49.00;
constexpr double Gs1 = 51.91;  constexpr double A1  = 55.00;
constexpr double As1 = 58.27;  constexpr double B1  = 61.74;

/* --- Octava 2 --- */
constexpr double C2  = 65.41;  constexpr double Cs2 = 69.30;
constexpr double D2  = 73.42;  constexpr double Ds2 = 77.78;
constexpr double E2  = 82.41;  constexpr double F2  = 87.31;
constexpr double Fs2 = 92.50;  constexpr double G2  = 98.00;
constexpr double Gs2 = 103.83; constexpr double A2  = 110.00;
constexpr double As2 = 116.54; constexpr double B2  = 123.47;

/* --- Octava 3 --- */
constexpr double C3  = 130.81; constexpr double Cs3 = 138.59;
constexpr double D3  = 146.83; constexpr double Ds3 = 155.56;
constexpr double E3  = 164.81; constexpr double F3  = 174.61;
constexpr double Fs3 = 185.00; constexpr double G3  = 196.00;
constexpr double Gs3 = 207.65; constexpr double A3  = 220.00;
constexpr double As3 = 233.08; constexpr double B3  = 246.94;

/* --- Octava 4 --- */
constexpr double C4  = 261.63; constexpr double Cs4 = 277.18;
constexpr double D4  = 293.66; constexpr double Ds4 = 311.13;
constexpr double E4  = 329.63; constexpr double F4  = 349.23;
constexpr double Fs4 = 369.99; constexpr double G4  = 392.00;
constexpr double Gs4 = 415.30; constexpr double A4  = 440.00;
constexpr double As4 = 466.16; constexpr double B4  = 493.88;
constexpr float Eb4 = 311.13f;



/* --- Octava 5 --- */
constexpr double C5  = 523.25; constexpr double Cs5 = 554.37;
constexpr double D5  = 587.33; constexpr double Ds5 = 622.25;
constexpr double E5  = 659.26; constexpr double F5  = 698.46;
constexpr double Fs5 = 739.99; constexpr double G5  = 784.00;
constexpr double Gs5 = 830.61; constexpr double A5  = 880.00;
constexpr double As5 = 932.33; constexpr double B5  = 987.77;
static constexpr int Bb5 = 932;    // Si bemol 5

/* --- Octava 6 --- */
constexpr double C6  = 1046.50; constexpr double Cs6 = 1108.73;
constexpr double D6  = 1174.66; constexpr double Ds6 = 1244.51;
constexpr double E6  = 1318.51; constexpr double F6  = 1396.91;
constexpr double Fs6 = 1479.98; constexpr double G6  = 1567.98;
constexpr double Gs6 = 1661.22; constexpr double A6  = 1760.00;
constexpr double As6 = 1864.66; constexpr double B6  = 1975.53;

/* --- Octava 7 --- */
constexpr double C7  = 2093.00; constexpr double Cs7 = 2217.46;
constexpr double D7  = 2349.32; constexpr double Ds7 = 2489.02;
constexpr double E7  = 2637.02; constexpr double F7  = 2793.83;
constexpr double Fs7 = 2959.96; constexpr double G7  = 3135.96;
constexpr double Gs7 = 3322.44; constexpr double A7  = 3520.00;
constexpr double As7 = 3729.31; constexpr double B7  = 3951.07;

/* --- Octava 8 (solo C8 para referencia) --- */
constexpr double C8  = 4186.01;
constexpr double D8  = 4698.64; constexpr double Ds8 = 4978.03;
constexpr double E8  = 5274.04; constexpr double F8  = 5587.65;
constexpr double Fs8 = 5919.91; constexpr double G8  = 6271.93;
constexpr double Gs8 = 6644.88; constexpr double A8  = 7040.00;
constexpr double As8 = 7458.62; constexpr double B8  = 7902.13;

// En notas.h, junto al resto de notas:





#endif  // NOTAS_H
