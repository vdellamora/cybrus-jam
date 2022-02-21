#include "guitarrista.h"

Guitarrista::Guitarrista(){
    this->GerarAcordes();
}
Guitarrista::Guitarrista(MuMaterial* materialBaixo){
    this->GerarAcordes();
    this->CarregarMaterialBaixo(materialBaixo);
}
Guitarrista::~Guitarrista(){}

void Guitarrista::CarregarMaterialBaixo(MuMaterial* materialBaixo){
    this->materialBaixo = materialBaixo;
}

void Guitarrista::GerarAcompanhamento(){
    cout << "Gerando o acompanhamento do guitarrista..." << endl;
    materialBaixo->AddVoices(1);
    materialBaixo->SetInstrument(1,GUITARRA_CANAL);
    int notas = materialBaixo->NumberOfNotes();
    for(int i = 0; i < notas; i++){
        MuNote n = materialBaixo->GetNote(i);
        bool encontrouNota = false;

        for(int j = 0; j < 3; j++){
            if(encontrouNota) continue;
            MuNote noteChord;
            long notasAcorde = acordes.NumberOfVoices();
            for(int k = 0; k < notasAcorde; k++){
                if(encontrouNota) continue;

                noteChord = acordes.GetNote(k,j);
                // Se a nota do baixo [n] for a nota tônica do acorde [noteChord], encontramos nosso acorde de acompanhamento
                if(noteChord.Pitch()%12 == n.Pitch()%12){
                    MuMaterial triade = acordes.GetVoice(k);
                    triade.SetAmp(0,0.4f);
                    //triade.SetInstrument(0,GUITARRA_CANAL);
                    //triade.SetStarts(0, n.Start());
                    triade.SetLengths(0,n.Dur());

                    for(int l = 0; l < triade.NumberOfNotes(0); l++){
                        noteChord = triade.GetNote(0, l);
                        noteChord.SetStart(n.Start());
                        noteChord.SetInstr(GUITARRA_CANAL);
                        materialBaixo->IncludeNote(1, noteChord);
                        
                        // note.SetPitch(inPitch);
                        // triade.SetNote(1, l, note);
                    }
                    encontrouNota = true;
                }
            }
        }
    }
    // materialBaixo->Show();
}

void Guitarrista::GerarAcordes(){
    cout << "Gerando o material do guitarrista..." << endl;
    MuMaterial triade;
    triade.MajorTriad(1);
    for(int i = 1; i <= 7; i++){
        // triade.Clear();
        triade.DiatonicTranspose(C_NAT, MAJOR_MODE, i, ASCENDING);
        acordes += triade;
    }
    // setar timbre do canal midi com função separada 0xC[0-F] (ver setMIDI no tg)
    acordes.SetInstrument(0, GUITARRA_CANAL);
    acordes = acordes.Segments(7);
    triade.Clear();
    
}