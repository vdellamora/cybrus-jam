#ifndef GUITARRISTA
#define GUITARRISTA

using namespace std;

#include <iostream>
#include "MuM/MuMaterial.h"
#define GUITARRA_INSTRUMENTO 3

class Guitarrista{
    private:
        MuMaterial* materialBaixo;
        MuMaterial acordes;
    public:
        Guitarrista();
        Guitarrista(MuMaterial*);
        ~Guitarrista();

        void CarregarMaterialBaixo(MuMaterial*);
        void GerarAcompanhamento();
        void GerarAcordes();
    
};

#endif //GUITARRISTA