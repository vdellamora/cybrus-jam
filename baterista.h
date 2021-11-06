#ifndef BATERISTA
#define BATERISTA

using namespace std;

#include <iostream>
#include "MuM/MuMaterial.h"
#define BATERIA_INSTRUMENTO_A 35
#define BATERIA_INSTRUMENTO_B 38
// Utilizar canal 10 do MIDI

class Baterista{
    private:
        MuMaterial* materialBaixo;
        MuMaterial batidas;
    public:
        Baterista();
        Baterista(MuMaterial*);
        ~Baterista();

        void CarregarMaterialBaixo(MuMaterial*);
        void GerarAcompanhamento();
        void GerarBatidas();
    
};


#endif //BATERISTA