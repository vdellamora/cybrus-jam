using namespace std;

#include <iostream>
#include <string>
#include "MuM/MuRecorder.h"
#include "MuM/MuPlayer.h"
#include "baixoAnalise.h"
#include "guitarrista.h"
#include "baterista.h"

#define GUITARRA_INSTRUMENTO 31
#define BATERIA_INSTRUMENTO 1


// Função que retorna o tempo atual do sistema
double currTime(void){
    timeval t;
    gettimeofday(&t, 0);
    double stamp = t.tv_sec + (t.tv_usec/1000000.0);
    return stamp;
}

int main(){
    //--------------Menu Inicial (parâmetros da sessão), opções de execução
    cout << "CybrusJam v0.1 -- Acompanhamento Musical" << endl;
    cout << "Carregando componentes..." << endl;
    MuRecorder input;
    int baixoPortaMIDI, saidaPortaMIDI, pulsacoes;
    MuMIDIBuffer inputBuffer;
    MuPlayer output;
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
    cout << "Insira a porta MIDI para a saída do acompanhamento: ";
    cin >> saidaPortaMIDI;
    output.SelectMIDIDestination(saidaPortaMIDI);
    cout << "A saída MIDI é relacionada a CSound? (S/N) ";
    string ehCSound; bool MIDIcsound = false;
    cin >> ehCSound;
    if ((ehCSound == "s") || (ehCSound == "S"))  MIDIcsound = true;

    usleep(500);
    while(!MIDIcsound){
        MuMIDIBuffer programChanges;
        programChanges.data = new MuMIDIMessage[4];
        if(programChanges.data){
            programChanges.max = 4;
            programChanges.count = 4;
            
            // Canal 2 = Guitarra
            programChanges.data[0].time = 0;
            programChanges.data[0].status = 0xC0;
            programChanges.data[0].data1 = GUITARRA_INSTRUMENTO;
            programChanges.data[0].data2 = GUITARRA_INSTRUMENTO;
            // Canal 2 = Guitarra
            programChanges.data[1].time = 0;
            programChanges.data[1].status = 0xC1;
            programChanges.data[1].data1 = GUITARRA_INSTRUMENTO;
            programChanges.data[1].data2 = GUITARRA_INSTRUMENTO;
            // Canal 2 = Guitarra
            programChanges.data[2].time = 0;
            programChanges.data[2].status = 0xC2;
            programChanges.data[2].data1 = GUITARRA_INSTRUMENTO;
            programChanges.data[2].data2 = GUITARRA_INSTRUMENTO;
            // Canal 10 = Bateria
            programChanges.data[3].time = 0;
            programChanges.data[3].status = 0xC9;
            programChanges.data[3].data1 = BATERIA_INSTRUMENTO;
            programChanges.data[3].data2 = BATERIA_INSTRUMENTO;

            if (!output.SendEvents(programChanges)){
                cout << "Erro ao configurar canais/instrumentos." << endl;
                return -1;
            } else {
                MIDIcsound = true;
            }
        }
    }
    usleep(500);

    cout << "Insira a quantidade de pulsações desejada para a análise: ";
    cin >> pulsacoes;

    //---Tradução do Baixo -> MIDI
        // O patch de PureData "ProjetoCybrusJam-MidiConverter", por José de Abreu Bacelar faz a conversão em MIDI, numa porta ALSA MIDI
        // que deve ser informada no início da execução do programa.

    
    //--------------Início da Fase 1 - Análise do Baixo recebido
    //---MIDI -> MuRecorder
        // O input MIDI do sistema será lido pelo MuRecorder, detectando as notas ativas
    bool terminaExecucao = false;
    int contagemNotas = 0;
    MuMIDIBuffer materialFase1; // entradaFase1
    materialFase1.max = 0;
    materialFase1.count = 0;
    materialFase1.data = NULL;
    materialFase1 = input.GetData();
    cout << "Iniciando sessão (aperte Ctrl+C para cancelar)" << endl;
    while(!terminaExecucao){
        usleep(100);

        // Caso haja mensagens no buffer, começa a receber as notas
        inputBuffer = input.GetData();
        // MuMIDIBuffer temp = joinMidi...
        materialFase1 = MuRecorder::JoinMIDIBuffers(materialFase1, inputBuffer); // possível memory leak
        // chamar delete [] pros datas dos buffers
        if(inputBuffer.count){
            MuMIDIMessage msg;
            for(int i = 0; i < inputBuffer.count; i++){
                msg = inputBuffer.data[i];


                // Lê e analisa a nota
                if((msg.status == MU_NOTE_ON) && (msg.data2 > 0)){
                    contagemNotas++;
                    
                    // Controle de qual nota é tocada
                    short inPitch = msg.data1 % 12; // Definição da altura da nota recebida
                    string nomesNotas[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
                    cout << "Nota recebida: " << nomesNotas[inPitch] << endl;

                }

                // Conta as notas. Quando for igual a quantidade de notas esperadas e receber o desligamento de uma nota, manda pra análise
                //---Análise do MIDI, reconhecimento de ritmo e altura
                    // As notas recebidas receberão uma classificação de prioridade, com base na sua proximidade da batida do metrônomo
                    // Notas mais próximas (com maior prioridade) serão as responsáveis por moldar os acordes
                    // O distanciamento entre notas será armazenado também, para que se monte uma percussão adequada
                if(contagemNotas >= pulsacoes){
                    if((msg.status == MU_NOTE_OFF) || (msg.data2 == 0)){
                        terminaExecucao = true;
                        
                    }
                }


            }
        }


    }

    // Setar as vozes de cada instrumento por constante, na criação do bAnalise
    bAnalise = new BaixoAnalise(materialFase1);
    bAnalise->ImprimirMaterial();
    bAnalise->AnaliseFase1(pulsacoes);

    //Pensar em trabalhar com as classes estáticas, inclusive o MuMaterial do baixo
    // Utilizar herança pros instrumentos

    //---Montar acorde de acompanhamento
        // A classe "guitarra" irá ser inicializada com os parâmetros midi de voz e instrumento
        // Ao receber as notas do baixo, a classe irá procurar acordes que façam sentido, no seu banco de materiais musicais
        // As notas recebidas terão classificação da prioridade, altura e distância uma da outra, para que possa ser feita --
        // -- uma busca detalhada no banco de materiais para encontrar algo que encaixe
        // Ao encontrar algo que encaixe na sessão, é montado o material para ser tocado no MuPlayer
    guitarra = new Guitarrista(bAnalise->GetMaterial());
    guitarra->GerarAcompanhamento();


    //---Montar percussão de acompanhamento
        // A classe "bateria" irá ser iniciaizada com os parâmetros midi de voz e instrumento(s)
        // Ao receber notas do baixo, a classe irá procurar ritmos que façam sentido, no seu banco de materiais musicais
        // As notas recebidas terão classificação de prioridade e distância entre elas, para buscar algo que encaixe na sessão
        // Ao encontrar material correspondente ao ritmo do baixo, é montado o material de saída para tocar no MuPlayer
    bateria = new Baterista(bAnalise->GetMaterial());
    bateria->GerarAcompanhamento();


    bAnalise->ImprimirMaterial();
    // Fase 2 - Acompanhamento
    //---Metrônomo tocando
        // A classe "metrônomo" servirá para ajustar o BPM da sessão e tocar o metrônomo (independente da bateria)
        // Será enviado pro MuPlayer um metrônomo simples com base no BPM atual da sessão
        // De acordo com o controle do pedal de ritmo, o usuário poderá ajustar o BPM da sessão conforme desejar



    //---Acordes e Percussão -> MuPlayer
        // O MuPlayer receberá os materiais de saída da guitarra, bateria e metrônomo para dar o acompanhamento ao usuário
    terminaExecucao = false;
    inputBuffer.max = 0;
    inputBuffer.count = 0;
    inputBuffer.data = NULL;

    cout << "Iniciando acompanhamento (fique 5 segundos sem tocar para finalizar)" << endl;
    output.Play(* bAnalise->GetMaterial(), PLAYBACK_MODE_NORMAL);
    double inicioPlayer = currTime();
    double ultimaNota = currTime();
    while(!terminaExecucao){
        usleep(100);
        inputBuffer = input.GetData();

        // Checagem se a execução terminou e colocar pra tocar novamente
        if(currTime() - inicioPlayer >= bAnalise->GetDuracao()){
            // cout << "Reiniciando acompanhamento... " << endl;
            output.Play(* bAnalise->GetMaterial(), PLAYBACK_MODE_NORMAL);
            inicioPlayer = currTime();
        }

        // Procura por notas tocadas para manter o acompanhamento
        if(inputBuffer.count){
            MuMIDIMessage msg;
            for(int i = 0; i < inputBuffer.count; i++){
                msg = inputBuffer.data[i];

                // Atualiza o tempo da última nota
                if((msg.status == MU_NOTE_ON) && (msg.data2 > 0)){
                    ultimaNota = currTime();
                    // cout << "Atualizei: " << ultimaNota << endl;

                }

            }
        }
        
        // If de saida
        if(currTime() - ultimaNota >= 5){
            output.Stop();
            terminaExecucao = true;
        }
    }


    







    // output.Stop();
    delete [] inputBuffer.data;
    delete [] materialFase1.data;
    free(bAnalise);
    free(guitarra);
    free(bateria);
    return 0;
}