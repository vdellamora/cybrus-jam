#ifndef GUITARRISTA
#define GUITARRISTA

using namespace std;

#include <iostream>
#include "MuM/MuMaterial.h"
#define GUITARRA_CANAL 2

class Guitarrista{
    private:
        MuMaterial* materialBaixo;

        MuMaterial acordes;
        MuMaterial powerChords;
        MuMaterial ritmosPalhetadas;

        double durPulsBx;
        int pulsBx;

    public:
        Guitarrista();
        Guitarrista(MuMaterial*, double, int);
        ~Guitarrista();

        void CarregarMaterialBaixo(MuMaterial*, double, int);
        void GerarAcompanhamento();
        void GerarAcordes();
    
};

#endif //GUITARRISTA