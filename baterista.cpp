#include "baterista.h"

Baterista::Baterista(){
    this->GerarBatidas();
}
Baterista::Baterista(MuMaterial* materialBaixo, double duracaoPulsacaoBaixo, int pulsacoesBaixo){
    this->GerarBatidas();
    this->CarregarMaterialBaixo(materialBaixo, duracaoPulsacaoBaixo, pulsacoesBaixo);
}
Baterista::~Baterista(){}

void Baterista::CarregarMaterialBaixo(MuMaterial* materialBaixo, double duracaoPulsacaoBaixo, int pulsacoesBaixo){
    this->materialBaixo = materialBaixo;
    this->durPulsBx = duracaoPulsacaoBaixo;
    this->pulsBx = pulsacoesBaixo;
}

void Baterista::GerarAcompanhamento(){
    cout << "Gerando o acompanhamento do baterista..." << endl;
    materialBaixo->AddVoices(1);
    int vozBateria = materialBaixo->NumberOfVoices()-1;

    // Jeito antigo com pulsações fixas:
        // cout << "antes do for batera: " << materialBaixo->NumberOfNotes() << endl;
        // for(int i = 0; i < materialBaixo->NumberOfNotes(0); i++){
        //     MuNote n = materialBaixo->GetNote(i);
        //     MuNote beat;
        //     if(i == 0){
        //         beat = caixa;
        //     } else {
        //         beat = bumbo;
        //     }      
        //     beat.SetDur(n.Dur());
        //     beat.SetStart(n.Start());
        //     materialBaixo->IncludeNote(vozBateria, beat);
        // }
    //
    
    // Regra de 3 para descobrir BPM:
    // materialBaixo->Show();
    //durPulsBx -> 60
    //pulsBx -> x
    int bpm = 60*pulsBx/durPulsBx;

    // 0: leve
    // 1: leveFirst
    // 2: rapido
    // 3: rapidoFirst
    int primeiraBatida, restoBatidas;
    if(bpm >= 90){
        // Usa as batidas rápidas
        primeiraBatida = 3;
        restoBatidas = 2;
        cout << "Batida rápida, BPM: " << bpm << endl;
    } else {
        // Usa as batidas leves
        primeiraBatida = 1;
        restoBatidas = 0;
        cout << "Batida leve, BPM: " << bpm << endl;

    }

    MuMaterial proxBatida = batidas.GetVoice(primeiraBatida);
    proxBatida.Fit(durPulsBx);
    materialBaixo->Append(vozBateria, proxBatida, 0);

    for(int i = 1; i < pulsBx; i++){
        proxBatida = batidas.GetVoice(restoBatidas);
        proxBatida.Fit(durPulsBx);
        materialBaixo->Append(vozBateria, proxBatida, 0);
    }

    materialBaixo->SetInstrument(vozBateria, BATERIA_CANAL);



}

void Baterista::GerarBatidas(){
    // Ajustar para uso em GeneralMIDI
    cout << "Gerando o material do baterista..." << endl;

    bumbo.SetAmp(1);
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

    // Construção das batidas pré-montadas da bateria, para serem encaixadas conforme as pulsações do baixo e duração das mesmas
    /////////////////////// LEVES
    MuMaterial leve;
    bumbo.SetDur(1);
    caixa.SetDur(1);
    chimbau.SetDur(0.5f);
    crash.SetDur(1);

    leve.AddVoices(1);

    bumbo.SetStart(0);
    leve += bumbo;
    leve += bumbo;
    bumbo.SetDur(0.5);
    leve += bumbo;
    leve += bumbo;
    bumbo.SetDur(1);
    leve += bumbo;

    caixa.SetStart(1);
    leve.AddNote(caixa);
    caixa.SetStart(3);
    leve.AddNote(caixa);

    chimbau.SetStart(0.5);
    leve.AddNote(chimbau);
    chimbau.SetStart(1.5);
    leve.AddNote(chimbau);
    chimbau.SetStart(2.5);
    leve.AddNote(chimbau);
    chimbau.SetStart(3.5);
    leve.AddNote(chimbau);

    MuMaterial leveFirst;

    leveFirst = leve;
    crash.SetStart(0); crash.SetDur(1);
    leveFirst.AddNote(crash);
    
    /////////////////////// RAPIDAS
    MuMaterial rapido;
    rapido.AddVoices(1);

    bumbo.SetStart(0);
    bumbo.SetAmp(1); bumbo.SetDur(0.5); rapido += bumbo;
    // bumbo.SetAmp(0); bumbo.SetDur(0.25); rapido += bumbo;
    bumbo.SetAmp(1); bumbo.SetDur(0.25); rapido += bumbo;
    bumbo.SetAmp(1); bumbo.SetDur(0.25); rapido += bumbo;
    
    bumbo.SetAmp(1); bumbo.SetDur(0.5); rapido += bumbo;
    // bumbo.SetAmp(0); bumbo.SetDur(0.25); rapido += bumbo;
    bumbo.SetAmp(1); bumbo.SetDur(0.25); rapido += bumbo;
    bumbo.SetAmp(1); bumbo.SetDur(0.25); rapido += bumbo;

    caixa.SetStart(0); caixa.SetDur(1); rapido.AddNote(caixa);

    surdo.SetStart(1); surdo.SetDur(1); rapido.AddNote(surdo);

    conducao.SetStart(0); conducao.SetDur(1); rapido.AddNote(conducao);
    conducao.SetStart(1); conducao.SetDur(1); rapido.AddNote(conducao);

    MuMaterial rapidoFirst;
    rapidoFirst = rapido;
    crash.SetStart(0); crash.SetDur(1);
    rapidoFirst.AddNote(crash);


    //setvoice geral
    batidas.SetVoice(0, leve, 0);
    batidas.SetVoice(1, leveFirst, 0);
    batidas.SetVoice(2, rapido, 0);
    batidas.SetVoice(3, rapidoFirst, 0);

    batidas.SetInstrument(0, 10);
    batidas.SetInstrument(1, 10);
    batidas.SetInstrument(2, 10);
    batidas.SetInstrument(3, 10);
}