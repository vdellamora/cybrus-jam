#ifndef BATERISTA
#define BATERISTA

using namespace std;

#include <iostream>
#include "MuM/MuMaterial.h"
#define BATERIA_INSTRUMENTO_A 10
#define BATERIA_INSTRUMENTO_B 11
// Utilizar canal 10 do MIDI

class Baterista{
    private:
        MuMaterial* materialBaixo;
        MuMaterial batidas;

        MuNote bumbo;
        MuNote caixa;
    public:
        Baterista();
        Baterista(MuMaterial*);
        ~Baterista();

        void CarregarMaterialBaixo(MuMaterial*);
        void GerarAcompanhamento();
        void GerarBatidas();
    
};


#endif //BATERISTA