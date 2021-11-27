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
    int vozBateria = materialBaixo->NumberOfVoices()-1;
    materialBaixo->SetInstrument(vozBateria,BATERIA_CANAL);
    cout << "antes do for batera: " << materialBaixo->NumberOfNotes() << endl;
    for(int i = 0; i < materialBaixo->NumberOfNotes(0); i++){
        MuNote n = materialBaixo->GetNote(i);
        MuNote beat;

        if(i == 0){
            beat = caixa;
        } else {
            beat = bumbo;
        }
        
        beat.SetDur(n.Dur());
        beat.SetStart(n.Start());
        materialBaixo->IncludeNote(vozBateria, beat);
    }
    // materialBaixo->Show();
}

void Baterista::GerarBatidas(){
    // Ajustar para uso em GeneralMIDI
    cout << "Gerando o material do baterista..." << endl;

    bumbo.SetAmp(BATERIA_VOLUME);
    bumbo.SetPitch(BATERIA_INSTRUMENTO_BUMBO);
    bumbo.SetInstr(BATERIA_CANAL);

    caixa.SetAmp(BATERIA_VOLUME);
    caixa.SetPitch(BATERIA_INSTRUMENTO_CAIXA);
    caixa.SetInstr(BATERIA_CANAL);
    
    surdo.SetAmp(BATERIA_VOLUME);
    surdo.SetPitch(BATERIA_INSTRUMENTO_SURDO);
    surdo.SetInstr(BATERIA_CANAL);
    
    tomL.SetAmp(BATERIA_VOLUME);
    tomL.SetPitch(BATERIA_INSTRUMENTO_TOM_L);
    tomL.SetInstr(BATERIA_CANAL);
    
    tomH.SetAmp(BATERIA_VOLUME);
    tomH.SetPitch(BATERIA_INSTRUMENTO_TOM_H);
    tomH.SetInstr(BATERIA_CANAL);
    

    chimbau.SetAmp(BATERIA_VOLUME);
    chimbau.SetPitch(BATERIA_INSTRUMENTO_CHIMBAU);
    chimbau.SetInstr(BATERIA_CANAL);
    
    crash.SetAmp(BATERIA_VOLUME);
    crash.SetPitch(BATERIA_INSTRUMENTO_CRASH);
    crash.SetInstr(BATERIA_CANAL);
    
    conducao.SetAmp(BATERIA_VOLUME);
    conducao.SetPitch(BATERIA_INSTRUMENTO_CONDUCAO);
    conducao.SetInstr(BATERIA_CANAL);
    
}