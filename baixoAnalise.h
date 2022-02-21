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
        double duracaoPulsacao;
        int    pulsacoesMaterial;

    public:
        BaixoAnalise();
        BaixoAnalise(MuMIDIBuffer);
        ~BaixoAnalise();

        void CarregarFase1(MuMIDIBuffer);
        void AnaliseFase1();
        void AnaliseFase1(int);

        MuMaterial* GetMaterial();
        int GetPulsacoes();
        double GetDuracao();
        double GetDuracaoPulsacao();
        
        void PrepararReproducao();
        void ImprimirMaterial();
    


};

#endif //BAIXOANALISE