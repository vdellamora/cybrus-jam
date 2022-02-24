#include "guitarrista.h"

Guitarrista::Guitarrista(){
    this->GerarAcordes();
}
Guitarrista::Guitarrista(MuMaterial* materialBaixo, double duracaoPulsacaoBaixo, int pulsacoesBaixo){
    this->GerarAcordes();
    this->CarregarMaterialBaixo(materialBaixo, duracaoPulsacaoBaixo, pulsacoesBaixo);
}
Guitarrista::~Guitarrista(){}

void Guitarrista::CarregarMaterialBaixo(MuMaterial* materialBaixo, double duracaoPulsacaoBaixo, int pulsacoesBaixo){
    this->materialBaixo = materialBaixo;
    this->durPulsBx = duracaoPulsacaoBaixo;
    this->pulsBx = pulsacoesBaixo;
}

void Guitarrista::GerarAcompanhamento(){
    // cout << "Gerando o acompanhamento do guitarrista..." << endl;
    materialBaixo->AddVoices(1);
    materialBaixo->SetInstrument(1,GUITARRA_CANAL);
    int notas = materialBaixo->NumberOfNotes();

    // Regra de 3 para descobrir BPM:
    // materialBaixo->Show();
    //durPulsBx -> 60
    //pulsBx -> x
    int bpm = 60 * pulsBx / durPulsBx;

    MuMaterial acordesSelecionados; acordesSelecionados.AddVoices(1);
    // Procurar as notas tônicas, separar os powerChords correspondentes e colocar um ritmo correspondente com a bateria
    for(int i = 0; i < notas; i++){
        MuNote n = materialBaixo->GetNote(i);
        if(n.Amp() != 0.1f) continue;
        bool encontrouNota = false;

        for(int j = 0; j < 3; j++){
            if(encontrouNota) continue;
            MuNote noteChord;
            long notasAcorde = powerChords.NumberOfVoices();
            for(int k = 0; k < notasAcorde; k++){
                if(encontrouNota) continue;

                noteChord = powerChords.GetNote(k,j);
                // Se a nota do baixo [n] for a nota tônica do acorde [noteChord], encontramos nosso acorde de acompanhamento
                if(noteChord.Pitch()%12 == n.Pitch()%12){
                    encontrouNota = true;
                    MuMaterial pcSelect = powerChords.GetVoice(k);
                    pcSelect.SetAmp(0,0.4f);
                    acordesSelecionados += pcSelect;
                }
            }
        }
    }

    // Encaixar os powerChords encontrados em ritmos que mesclem com a bateria, através da análise do BPM
    acordesSelecionados = acordesSelecionados.Segments(pulsBx);
    // acordesSelecionados.Show();
    MuMaterial harmonia;
    // MuMaterial harmoniaAlt;
    int pulsacaoAlternativa = -1;
    int vozHarmonia = -1;
    int vozHarmoniaAlt = -1;


    if(bpm > 300){
        // rapido
        pulsacaoAlternativa = 0;
        vozHarmonia = 4; vozHarmoniaAlt = 4;
    } else if(bpm > 120){
        // padrão
        pulsacaoAlternativa = 0;
        vozHarmonia = 2; vozHarmoniaAlt = 2;
    } else {
        // longo
        pulsacaoAlternativa = 0;
        vozHarmonia = 3; vozHarmoniaAlt = 3;
    }
    harmonia = ritmosPalhetadas.GetVoice(vozHarmonia);
    harmonia.Fit(durPulsBx);
    // harmoniaAlt = ritmosPalhetadas.GetVoice(vozHarmoniaAlt);
    harmonia.Show();

    for(int i = 0; i < pulsBx; i++){
        // if(i == pulsacaoAlternativa){
        //     harmonia = ritmosPalhetadas.GetVoice(vozHarmoniaAlt);
        // } else {
        //     harmonia = ritmosPalhetadas.GetVoice(vozHarmonia);
        // }
        // harmonia.Fit(durPulsBx);

        for(int j = 0; j < harmonia.NumberOfNotes(); j++){
            MuNote n = harmonia.GetNote(j);
            if (n.Amp() == 0.1f){
                // Nota forte
                MuMaterial powerChord = acordesSelecionados.GetVoice(i);
                powerChord.Fit(n.Dur());
                materialBaixo->Append(1, powerChord, 0);
            } else {
                // Nota fraca
                MuNote primeiraNota = acordesSelecionados.GetVoice(i).GetNote(0);
                primeiraNota.SetDur(n.Dur());
                materialBaixo->Append(1, primeiraNota);
            }
            
        }


    }
    // materialBaixo->Show();



}

void Guitarrista::GerarAcordes(){
    // cout << "Gerando o material do guitarrista..." << endl;

    MuNote tonica; tonica.SetPitch(38); tonica.SetStart(0); tonica.SetDur(1); tonica.SetAmp(0.4f);
    MuNote quinta; quinta.SetPitch(45); quinta.SetStart(0); quinta.SetDur(1); quinta.SetAmp(0.4f);
    MuNote oitava; oitava.SetPitch(50); oitava.SetStart(0); oitava.SetDur(1); oitava.SetAmp(0.4f);

    MuMaterial powerTriade;
    powerTriade.AddVoices(1);
    powerTriade.AddNote(tonica);
    powerTriade.AddNote(quinta);
    powerTriade.AddNote(oitava);
    powerChords += powerTriade;
    for(int i = 1; i<12; i++){
        powerTriade.Transpose(1);
        powerChords += powerTriade;
    }
    powerChords.SetInstrument(0, GUITARRA_CANAL);
    // powerChords.Show();
    powerChords = powerChords.Segments(12);
    powerTriade.Clear();

    // Modo antigo de acordes, por tríades:
        // MuMaterial triade;
        // triade.MajorTriad(1);
        // for(int i = 1; i <= 7; i++){
        //     // triade.Clear();
        //     triade.DiatonicTranspose(C_NAT, MAJOR_MODE, i, ASCENDING);
        //     acordes += triade;
        // }
        // // setar timbre do canal midi com função separada 0xC[0-F] (ver setMIDI no tg)
        // acordes.SetInstrument(0, GUITARRA_CANAL);
        // acordes.Show();
        // acordes = acordes.Segments(7);
        // triade.Clear();


    // Fazendo banco de ritmos:

    // Rápido, BPM > 400
    MuMaterial rapido; rapido.AddVoices(1);
    MuNote notaRitmo; notaRitmo.SetStart(0); notaRitmo.SetPitch(0);
    int qtdNotas = 16;
    notaRitmo.SetDur(2.0f/qtdNotas); notaRitmo.SetAmp(0.1f); rapido += notaRitmo;
    notaRitmo.SetDur(1.0f/qtdNotas); notaRitmo.SetAmp(0.05f); rapido += notaRitmo;
    rapido += notaRitmo;
    rapido += notaRitmo;
    rapido += notaRitmo;
    rapido += notaRitmo;
    rapido += notaRitmo;

    MuMaterial rapidoResto; rapidoResto.AddVoices(1);
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;
    rapidoResto += notaRitmo;


    // Padrão, BPM > 120
    qtdNotas = 8;
    MuMaterial padrao; padrao.AddVoices(1); notaRitmo.SetDur(1.0f/qtdNotas);
    notaRitmo.SetAmp(0.1f);  padrao += notaRitmo;
    notaRitmo.SetAmp(0.05f); padrao += notaRitmo;
    notaRitmo.SetAmp(0.05f); padrao += notaRitmo;
    notaRitmo.SetAmp(0.1f);  padrao += notaRitmo;
    notaRitmo.SetAmp(0.05f); padrao += notaRitmo;
    notaRitmo.SetAmp(0.05f); padrao += notaRitmo;
    notaRitmo.SetAmp(0.1f);  padrao += notaRitmo;
    notaRitmo.SetAmp(0.05f); padrao += notaRitmo;

    qtdNotas = 1;
    MuMaterial longo; longo.AddVoices(1);
    notaRitmo.SetDur(1.0f/qtdNotas); notaRitmo.SetAmp(0.1f); longo += notaRitmo;


    // Colocando ritmos no banco:
    ritmosPalhetadas.SetVoice(0, rapido, 0);
    ritmosPalhetadas.SetVoice(1, rapidoResto, 0);
    ritmosPalhetadas.SetVoice(2, padrao, 0);
    ritmosPalhetadas.SetVoice(3, longo, 0);

    MuMaterial rapidoCompleto = rapido + rapidoResto;
    ritmosPalhetadas.SetVoice(4, rapidoCompleto, 0);

    ritmosPalhetadas.SetInstrument(0, GUITARRA_CANAL);
    ritmosPalhetadas.SetInstrument(1, GUITARRA_CANAL);
    ritmosPalhetadas.SetInstrument(2, GUITARRA_CANAL);
    ritmosPalhetadas.SetInstrument(3, GUITARRA_CANAL);
    ritmosPalhetadas.SetInstrument(4, GUITARRA_CANAL);
}