# cybrus-jam

CybrusJam é um software de acompanhamento musical que fornece instrumentos a uma sessão de improvisação para músicos que queiram praticar sozinhos, fazendo uso da biblioteca MuM do professor Dr. Carlos Eduardo Vianna de Mello, do Departamento de Música da Universidade de Brasília. Este é o projeto de trabalho de graduação do curso de Ciência da Computação na Universidade de Brasília do aluno Vitor Moraes Dellamora.

# Conexões necessárias

- Conecte o baixo em uma interface de áudio ou diretamente na entrada de microfone do computador;
- Conecte o pedal Teensy na porta USB do computador;
- Abra o arquivo do PureData (ProjetoCybrusJam-MidiConverter.pd);
- Abra as portas MIDI do sintetizador que será utilizado (por exemplo, Timidity);
- Defina um dispositivo de saída de áudio do computador como a saída padrão;

# Instruções de execução

Para compilar o software, compile a biblioteca utilizando:
`make compileMUM`

Em seguida, compile o projeto utilizando:
`make compile`

Configure a entrada de áudio e as portas ALSA-MIDI no PureData.

Execute o programa com 
`./CYBRUSJAM`

A seguir, selecione as entradas de MIDI para o baixo e para o pedal.
Escolha a entrada com "PureData" para o baixo e "Teensy" para o pedal.
Após isso, selecione a saída MIDI desejada (por exemplo, TiMidity port).

Execute uma linha de baixo à sua escolha e pressione o pedal quando estiver satisfeito.
Toque junto do acompanhamento ou fique cinco segundos sem utilizar o instrumento para encerrar a execução.
