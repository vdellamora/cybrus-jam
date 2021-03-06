#ifndef BATERISTA
#define BATERISTA

using namespace std;

#include <iostream>
#include "MuM/MuMaterial.h"
#define BATERIA_CANAL 10
#define BATERIA_VOLUME 0.4f

#define BATERIA_INSTRUMENTO_BUMBO 35
#define BATERIA_INSTRUMENTO_CAIXA 38
#define BATERIA_INSTRUMENTO_SURDO 41
#define BATERIA_INSTRUMENTO_TOM_L 45
#define BATERIA_INSTRUMENTO_TOM_H 50

#define BATERIA_INSTRUMENTO_CHIMBAU 42
#define BATERIA_INSTRUMENTO_CRASH 49
#define BATERIA_INSTRUMENTO_CONDUCAO 51

class Baterista{
    private:
        MuMaterial* materialBaixo;
        MuMaterial batidas;

        MuNote bumbo;
        MuNote caixa;
        MuNote surdo;
        MuNote tomL;
        MuNote tomH;

        MuNote chimbau;
        MuNote crash;
        MuNote conducao;

        double durPulsBx;
        int pulsBx;

    public:
        Baterista();
        Baterista(MuMaterial*, double, int);
        ~Baterista();

        void CarregarMaterialBaixo(MuMaterial*, double, int);
        void GerarAcompanhamento();
        void GerarBatidas();
    
};


#endif //BATERISTA