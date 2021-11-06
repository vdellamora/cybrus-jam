#include "baterista.h"

Baterista::Baterista(){
    this->GerarBatidas();
}
Baterista::Baterista(MuMaterial* materialBaixo){
    this->GerarBatidas();
    this->CarregarMaterialBaixo(materialBaixo);
}
Baterista::~Baterista(){}

void Baterista::CarregarMaterialBaixo(MuMaterial* materialBaixo){
    this->materialBaixo = materialBaixo;
}

void Baterista::GerarAcompanhamento(){
    cout << "Gerando o acompanhamento do baterista..." << endl;
    materialBaixo->AddVoices(1);
    int canalBateria = materialBaixo->NumberOfVoices()-1;
    materialBaixo->SetInstrument(canalBateria,BATERIA_INSTRUMENTO_A);
    cout << "antes do for batera: " << materialBaixo->NumberOfNotes() << endl;
    for(int i = 0; i < materialBaixo->NumberOfNotes(0); i++){
        // cout << "batera for: " << i << endl;
        MuNote n = materialBaixo->GetNote(i);
        MuNote beat;

        if(i == materialBaixo->NumberOfNotes(0)-1){
            beat = caixa;
        } else {
            beat = bumbo;
        }
        
        beat.SetDur(n.Dur());
        beat.SetStart(n.Start());
        materialBaixo->Append(canalBateria, beat);
    }
    materialBaixo->Show();
}

void Baterista::GerarBatidas(){
    cout << "Gerando o material do baterista..." << endl;

    bumbo.SetAmp(0.5f);
    bumbo.SetPitch(BATERIA_INSTRUMENTO_A);
    bumbo.SetInstr(BATERIA_INSTRUMENTO_A);
    caixa.SetAmp(0.5f);
    caixa.SetPitch(BATERIA_INSTRUMENTO_B);
    caixa.SetInstr(BATERIA_INSTRUMENTO_B);
    
}