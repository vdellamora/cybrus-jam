using namespace std;

#include "baixoAnalise.h"
BaixoAnalise::BaixoAnalise(){
    materialMusical = new MuMaterial();

}
BaixoAnalise::BaixoAnalise(MuMIDIBuffer buffer){
    cout << "baixoAnalise iniciado" << endl;
    materialMusical = new MuMaterial();
    this->CarregarFase1(buffer);
}
BaixoAnalise::~BaixoAnalise(){}


void BaixoAnalise::CarregarFase1(MuMIDIBuffer buffer){
    cout << "baixoAnalise carregando Fase1" << endl;
    materialMusical->LoadMIDIBuffer(buffer, MIDI_BUFFER_MODE_EXTEND);
    //LoadMidiBuffer materialFase1
    cout << "baixoAnalise Fase1 Carregada" << endl;
    // materialMusical->Show();
}

//---Análise do MIDI, reconhecimento de ritmo e altura
    // As notas recebidas receberão uma classificação de prioridade, com base na sua proximidade da batida do metrônomo
    // Notas mais próximas (com maior prioridade) serão as responsáveis por moldar os acordes
    // O distanciamento entre notas será armazenado também, para que se monte uma percussão adequada
void BaixoAnalise::AnaliseFase1(int repeticoes){
    float duracaoMediaNota = 0;
    float inicioNota = 0;

    int notas = materialMusical->NumberOfNotes();
    for(int i = 0; i < notas-1; i++){
        MuNote n1 = materialMusical->GetNote(i);
        MuNote n2 = materialMusical->GetNote(i+1);
        duracaoMediaNota += n2.Start() - n1.Start();

    }
    duracaoMediaNota /= notas-1;
    for(int i = 0; i < notas; i++){
        MuNote n = materialMusical->GetNote(i);
        n.SetStart(inicioNota);
        n.SetDur(duracaoMediaNota);
        n.SetAmp(0);
        inicioNota += duracaoMediaNota;


        materialMusical->SetNote(0,i,n);
    }

    // pitch /12 + 3 == Notação CSound
    return;
}

void BaixoAnalise::ImprimirMaterial(){
    cout << "baixoAnalise imprimindo fase1" << endl;
    materialMusical->Show();
}

MuMaterial* BaixoAnalise::GetMaterial(){
    return materialMusical;
}