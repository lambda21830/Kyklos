<div align="center"><img src="/assets/img/kyklos.png" alt="Kyklos Logo"></div>

# Kyklos

Kyklos è un programma a riga di comando, scritto in C++, per calcolare l'equazione di una circonferenza.

Il programma supporta 3 modalità di calcolo:

1. Dati 3 punti non allineati
2. Dato il centro e un punto
3. Dato il centro e il raggio non negativo

## Come funziona

### Modalità 1 - Tre punti

Per determinare l'equazione di una circonferenza dati 3 punti, Kyklos adotta il seguente approccio:

1. **Imposta un sistema lineare**: Sostituisce le coordinate di ciascun punto (x, y) nell'equazione generica `x²+y²+ax+by+c=0`, ottenendo un sistema di tre equazioni lineari nelle incognite `a`, `b` e `c`
2. **Eliminazione gaussiana**: Il sistema viene rappresentato come una matrice aumentata e risolto tramite l'algoritmo di eliminazione gaussiana
3. **Stabilità numerica**: Per garantire la massima precisione e robustezza, viene implementata la strategia del pivoting parziale, che minimizza gli errori di arrotondamento
4. **Back-substitution**: Una volta che la matrice è stata trasformata in forma triangolare superiore (a gradini), i valori dei coefficienti `a`, `b` e `c` vengono ricavati tramite sostituzione all'indietro

### Modalità 2 e 3 - Centro e punto/raggio

1. **Calcolo coefficienti**: Si calcolano i coefficienti `a`, `b` e `c` con le appropriate formule algebriche
2. **Composizione dell'equazione**: Si compone l'equazione con i coefficienti ricavati

## Guida all'utilizzo

### Prerequisiti

Per utilizzare Kyklos è necessario scaricare [Git](https://git-scm.com/downloads), un C++ toolchain (`g++` o `clang++` e `make`) su [Windows](https://code.visualstudio.com/docs/cpp/config-mingw), [MacOS](https://code.visualstudio.com/docs/cpp/config-clang-mac) o una distribuzione [GNU/Linux](https://code.visualstudio.com/docs/cpp/config-linux).

### Download

1. Apri il terminale e clona la repository: `git clone https://github.com/lambda21830/Kyklos.git`
2. Vai nella repository clonata: `cd Kyklos`
3. Compila ed esegui il programma: `make run`
