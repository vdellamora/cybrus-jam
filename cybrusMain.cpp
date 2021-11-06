using namespace std;

#include <iostream>
#include <string>
#include "MuM/MuRecorder.h"
#include "MuM/MuPlayer.h"
#include "baixoAnalise.h"
#include "guitarrista.h"
#include "baterista.h"

int main(){
    //--------------Menu Inicial (parâmetros da sessão), opções de execução
    cout << "CybrusJam v0.1 -- Acompanhamento Musical" << endl;
    cout << "Carregando componentes..." << endl;
    MuRecorder input;
    int baixoPortaMIDI = 5;
    MuMIDIBuffer inputBuffer;
    MuPlayer output;
    int BPMmetronomo, repeticoes;
    BaixoAnalise* bAnalise;
    Guitarrista* guitarra;
    Baterista* bateria;


    cout  << "Carregando Recorder..." << endl;
    input.Init(100);
    cout << "Insira a porta MIDI para a entrada do baixo: ";
    cin >> baixoPortaMIDI;
    input.SelectMIDISource(baixoPortaMIDI);

    cout << "Carregando buffer..." << endl;
    inputBuffer.max = 0;
    inputBuffer.count = 0;
    inputBuffer.data = NULL;

    cout << "Carregando Player..." << endl;
    output.Init();

    cout << "Insira a quantidade de tempos desejada para a análise: ";
    cin >> repeticoes;

    //---Tradução do Baixo -> MIDI
        // O patch de PureData "ProjetoCybrusJam-MidiConverter", por José de Abreu Bacelar faz a conversão em MIDI, numa porta ALSA MIDI
        // que deve ser informada no início da execução do programa.

    
    //--------------Início da Fase 1 - Análise do Baixo recebido
    //---MIDI -> MuRecorder
        // O input MIDI do sistema será lido pelo MuRecorder, detectando as notas ativas
    bool terminaExecucao = false;
    int contagemNotas = 0;
    MuMIDIBuffer materialFase1;
    materialFase1.max = 0;
    materialFase1.count = 0;
    materialFase1.data = NULL;
    materialFase1 = input.GetData();
    cout << "Iniciando sessão (aperte Ctrl+C para finalizar)" << endl;
    while(!terminaExecucao){
        usleep(100);

        // Caso haja mensagens no buffer, começa a receber as notas
        inputBuffer = input.GetData();
        materialFase1 = MuRecorder::JoinMIDIBuffers(materialFase1, inputBuffer);
        if(inputBuffer.count){
            MuMIDIMessage msg;
            for(int i = 0; i < inputBuffer.count; i++){
                msg = inputBuffer.data[i];
                //ShowMIDIMessage(msg);
                //RemoveChannel(msg);


                // Lê e analisa a nota
                if((msg.status == MU_NOTE_ON) && (msg.data2 > 0)){
                    contagemNotas++;
                    short inPitch = msg.data1 % 12; // Definição da altura da nota recebida
                    string nomesNotas[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
                    cout << "Nota recebida: " << nomesNotas[inPitch] << endl;

                }

                // TODO: IF DO FIM DA FASE 1
                // Conta as notas. Quando for igual a quantidade de notas esperadas e receber o desligamento de uma nota, manda pra análise
                //---Análise do MIDI, reconhecimento de ritmo e altura
                    // As notas recebidas receberão uma classificação de prioridade, com base na sua proximidade da batida do metrônomo
                    // Notas mais próximas (com maior prioridade) serão as responsáveis por moldar os acordes
                    // O distanciamento entre notas será armazenado também, para que se monte uma percussão adequada
                if(contagemNotas >= repeticoes){
                    if((msg.status == MU_NOTE_OFF) || (msg.data2 == 0)){
                        terminaExecucao = true;
                        bAnalise = new BaixoAnalise(materialFase1);
                        bAnalise->ImprimirMaterial();

                        bAnalise->AnaliseFase1(repeticoes);
                        guitarra = new Guitarrista(bAnalise->GetMaterial());
                        guitarra->GerarAcompanhamento();
                        bateria = new Baterista(bAnalise->GetMaterial());
                        bateria->GerarAcompanhamento();
                    }
                }
            }
        }
    }
    // TODO: Fase 2
    // TODO: Metrônomo tocando



    

    //---Montar acorde de acompanhamento
        // A classe "guitarra" irá ser inicializada com os parâmetros midi de voz e instrumento
        // Ao receber as notas do baixo, a classe irá procurar acordes que façam sentido, no seu banco de materiais musicais
        // As notas recebidas terão classificação da prioridade, altura e distância uma da outra, para que possa ser feita --
        // -- uma busca detalhada no banco de materiais para encontrar algo que encaixe
        // Ao encontrar algo que encaixe na sessão, é montado o material para ser tocado no MuPlayer


    //---Montar percussão de acompanhamento
        // A classe "bateria" irá ser iniciaizada com os parâmetros midi de voz e instrumento(s)
        // Ao receber notas do baixo, a classe irá procurar ritmos que façam sentido, no seu banco de materiais musicais
        // As notas recebidas terão classificação de prioridade e distância entre elas, para buscar algo que encaixe na sessão
        // Ao encontrar material correspondente ao ritmo do baixo, é montado o material de saída para tocar no MuPlayer



    //---Metrônomo tocando
        // A classe "metrônomo" servirá para ajustar o BPM da sessão e tocar o metrônomo (independente da bateria)
        // Será enviado pro MuPlayer um metrônomo simples com base no BPM atual da sessão
        // De acordo com o controle do pedal de ritmo, o usuário poderá ajustar o BPM da sessão conforme desejar


    //---Acordes e Percussão -> MuPlayer
        // O MuPlayer receberá os materiais de saída da guitarra, bateria e metrônomo para dar o acompanhamento ao usuário

    output.Stop();
    delete [] inputBuffer.data;
    delete [] materialFase1.data;
    free(bAnalise);
    free(guitarra);
    return 0;
}