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
    materialBaixo->SetInstrument(2,BATERIA_INSTRUMENTO_A);
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
                    triade.SetAmp(0,0.5);
                    //triade.SetInstrument(0,GUITARRA_INSTRUMENTO);
                    //triade.SetStarts(0, n.Start());
                    triade.SetLengths(0,n.Dur());

                    for(int l = 0; l < triade.NumberOfNotes(0); l++){
                        noteChord = triade.GetNote(0, l);
                        noteChord.SetStart(n.Start());
                        // noteChord.SetInstr(GUITARRA_INSTRUMENTO);
                        materialBaixo->Append(1, noteChord);
                        
                        // note.SetPitch(inPitch);
                        // triade.SetNote(1, l, note);
                    }
                    //triade.Show();
                    // out.Play(triade,PLAYBACK_MODE_NORMAL);
                    encontrouNota = true;
                    break;
                }
            }
        }
    }
    cout << "Vozes material: " << materialBaixo->NumberOfVoices() << endl;
    materialBaixo->Show();
}

void Baterista::GerarBatidas(){
    cout << "Gerando o material do baterista..." << endl;
    MuMaterial triade;
    for(int i = 1; i <= 7; i++){
        triade.Clear();
        triade.MajorTriad(1);
        triade.DiatonicTranspose(C_NAT, MAJOR_MODE, i, ASCENDING);
        batidas += triade;
    }
    batidas.SetInstrument(0, 1);
    batidas = batidas.Segments(7);
    triade.Clear();
    
}