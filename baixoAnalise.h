#ifndef BAIXOANALISE
#define BAIXOANALISE

using namespace std;

#include <iostream>
#include <vector>
#include "MuM/MuMaterial.h"

class BaixoAnalise{
    private:
        MuMaterial* materialMusical;
        double duracaoMaterial;

    public:
        BaixoAnalise();
        BaixoAnalise(MuMIDIBuffer);
        ~BaixoAnalise();

        void CarregarFase1(MuMIDIBuffer);
        void AnaliseFase1(int);
        MuMaterial* GetMaterial();
        double GetDuracao();
        void ImprimirMaterial();
    


};

#endif //BAIXOANALISE