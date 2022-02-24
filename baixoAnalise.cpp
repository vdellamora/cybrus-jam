using namespace std;

#include "baixoAnalise.h"
BaixoAnalise::BaixoAnalise(){
    duracaoMaterial = 0;
    materialMusical = new MuMaterial();

}
BaixoAnalise::BaixoAnalise(MuMIDIBuffer buffer){
    duracaoMaterial = 0;
    // cout << "baixoAnalise iniciado" << endl;
    materialMusical = new MuMaterial();
    this->CarregarFase1(buffer);
}
BaixoAnalise::~BaixoAnalise(){}


void BaixoAnalise::CarregarFase1(MuMIDIBuffer buffer){
    // cout << "baixoAnalise carregando Fase1" << endl;
    materialMusical->LoadMIDIBuffer(buffer, MIDI_BUFFER_MODE_EXTEND);
    //LoadMidiBuffer materialFase1
    // cout << "baixoAnalise Fase1 Carregada" << endl;
}

//---Análise do MIDI, reconhecimento de ritmo e altura
    // Será feita uma busca de quantas pulsações houveram
    // A melhor quantidade de pulsações (baseada em quantas notas tônicas há no início de cada pulsação) será definida
    // As notas recebidas receberão uma classificação de prioridade, com base na sua proximidade do início da pulsação
    // Notas mais próximas (com maior prioridade) serão as responsáveis por moldar os acordes
    // O distanciamento entre notas será armazenado também, para que se monte uma percussão adequada
void BaixoAnalise::AnaliseFase1(){
    pulsacoesMaterial = 0;
    int notasTonicasSelect = 0;
    int notas = materialMusical->NumberOfNotes();

    MuNote n = materialMusical->GetNote(0);
    n.SetStart(0);
    n.SetAmp(0);
    materialMusical->SetNote(0,0,n);
    for(int i = 1; i < notas; i++){
        n = materialMusical->GetNote(i);
        MuNote m = materialMusical->GetNote(i-1);
        n.SetStart(m.Start() + m.Dur());
        n.SetAmp(0);
        materialMusical->SetNote(0,i,n);
    }

    n = materialMusical->GetNote(notas-1);
    duracaoMaterial = n.Start() + n.Dur();

    float durPuls;
    int notasTonicas;
    float duracaoEsperada;

    for(int i = 1; i <= notas; i *= 2){
        durPuls = duracaoMaterial / i;
        notasTonicas = 0;
        duracaoEsperada = 0;

        for(int j = 0; j < notas; j++){
            n = materialMusical->GetNote(j);

            // Se a nota for encontrada no início de cada pulsação (com tolerância de 10% da duração), adiciona na contagem
            if((n.Start() >= duracaoEsperada - durPuls/10) && (n.Start() <= duracaoEsperada + durPuls/10)){
                notasTonicas++;
                duracaoEsperada += durPuls;
            }
        }

        if(notasTonicas > notasTonicasSelect){
            pulsacoesMaterial = i;
            notasTonicasSelect = notasTonicas;
            duracaoPulsacao = durPuls;
        }
    }


    // Identificando as notas tônicas através de sua amplitude
    // durPuls = duracaoMaterial / pulsacoesMaterial;
    duracaoEsperada = 0;
    for(int j = 0; j < notas; j++){
        n = materialMusical->GetNote(j);

        // cout << "Nota: " << j << "\t dN: " << n.Start() << "\t dE: " << duracaoEsperada;

        if((n.Start() >= duracaoEsperada - duracaoPulsacao/10) && (n.Start() <= duracaoEsperada + duracaoPulsacao/10)){
            n.SetAmp(0.1);
            materialMusical->SetNote(0,j,n);
            duracaoEsperada += duracaoPulsacao;
            
            // cout << " OK";
        }
        // cout << endl;
    }
    cout << "Pulsações definidas: " << pulsacoesMaterial << endl;
    cout << "Duração de pulsação: " << duracaoPulsacao << endl;
    
    return;
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
    duracaoMaterial = duracaoMediaNota * notas;
    for(int i = 0; i < notas; i++){
        MuNote n = materialMusical->GetNote(i);
        n.SetStart(inicioNota);
        n.SetDur(duracaoMediaNota);
        n.SetAmp(0);
        inicioNota += duracaoMediaNota;

        materialMusical->SetNote(0,i,n); // algumas paradas consideram amp 0 como pausa, isso pode dar errado depois
    }
    // pitch /12 + 3 == Notação CSound

    return;
}

// Muta a voz do baixo para não sair junto do acompanhamento
void BaixoAnalise::PrepararReproducao(){
    materialMusical->RemoveVoice(0);
}

void BaixoAnalise::ImprimirMaterial(){
    // cout << "baixoAnalise imprimindo fase1" << endl;
    materialMusical->Show();
}

MuMaterial* BaixoAnalise::GetMaterial(){
    return materialMusical;
}

int BaixoAnalise::GetPulsacoes(){
    return pulsacoesMaterial;
}
double BaixoAnalise::GetDuracao(){
    return duracaoMaterial;
}
double BaixoAnalise::GetDuracaoPulsacao(){
    return duracaoPulsacao;
}