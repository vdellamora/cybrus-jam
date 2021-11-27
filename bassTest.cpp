#include <iostream>
using namespace std;
#include "MuM/MuPlayer.h"
#include "MuM/MuRecorder.h"

MuMaterial ConstruirPadroesRitmicos(void);

int main(void)
{
    MuMaterial acordes,triade,ritmos;
    bool done = false;
    bool noteFound = false;
    
    ritmos = ConstruirPadroesRitmicos();
    ritmos.SetInstrument(0, 2);
    
    cout  << "Carregando Recorder..." << endl;
    MuRecorder in;
    in.Init(100);
    in.SelectMIDISource(1);
    MuMIDIBuffer inBuff;
    inBuff.max = 0;
    inBuff.count = 0;
    inBuff.data = NULL;
    
    cout << endl << "Carregando Player..." << endl;
    MuPlayer out;
    out.Init();
    
    cout << "Gerando os acordes..." << endl;
    for(int i = 1; i <= 7; i++)
    {
        triade.Clear();
        triade.MajorTriad(1);
        triade.DiatonicTranspose(C_NAT, MAJOR_MODE, i, ASCENDING);
        acordes += triade;
    }
    acordes.SetInstrument(0, 1);
    triade.Clear();
    //acordes.Show();
    //out.Play(ritmos,PLAYBACK_MODE_NORMAL);
    
    cout << endl << "Colocando os acordes em vozes separadas..." << endl;
    acordes = acordes.Segments(7);
    //acordes.Show();
    
    cout << endl << "Ouvindo..." << endl;
    
    while(!done)
    {
        usleep(100);
        inBuff = in.GetData();
        if(inBuff.count > 0)
        {
            MuMIDIMessage msg;
            for(int i = 0; i < inBuff.count; i++)
            {
                msg = inBuff.data[i];
                //ShowMIDIMessage(msg);
                RemoveChannel(msg);
                
                if((msg.status == MU_NOTE_ON) && (msg.data2 > 0))
                {
                    short inPitch = msg.data1;
                    for(int j = 0; j < acordes.NumberOfVoices(); j ++)
                    {
                        MuNote note;
                        long n = acordes.NumberOfNotes(j);
                        for(int k = 0; k < n; k++)
                        {
                            note = acordes.GetNote(j,k);
                            if(note.Pitch() == MIDIToPitchClass(inPitch))
                            {
                                triade = acordes.GetVoice(j);
                                triade.SetAmp(0,0.3);
                                triade.SetInstrument(0,2);
                                triade.SetLengths(0,1);
                                triade.SetVoice(1,ritmos,0);
                                triade.SetInstrument(1,1);
                                triade.SetAmp(1,0.9);
                                for(int l = 0; l < triade.NumberOfNotes(1); l++)
                                {
                                    note = triade.GetNote(1, l);
                                    note.SetPitch(inPitch);
                                    triade.SetNote(1, l, note);
                                }
                                //triade.Show();
                                out.Play(triade,PLAYBACK_MODE_NORMAL);
                                noteFound = true;
                                break;
                            }
                        }
                        if(noteFound)
                        {
                            noteFound = false;
                            break;
                        }
                            
                    }
                }
                
                if((msg.status == MU_CONTROL) && (msg.data1 == 0x10))
                {
                    done = true;
                }
            }
            // releasing buffer memory...
            if(inBuff.data)
                delete [] inBuff.data;
            inBuff.count = 0;
            inBuff.max = 0;
        }
    }
    out.Stop();
    return 0;
}

MuMaterial ConstruirPadroesRitmicos(void)
{
    MuMaterial temp;
    
    MuNote nota;
   
    nota.SetPitch(48);
    nota.SetAmp(1.0);
    nota.SetStart(0);
    nota.SetInstr(1);
    
    // one beat...
    nota.SetDur(0.25);
    temp += nota;
    nota.SetDur(0.25);
    temp += nota;
    nota.SetDur(0.5);
    temp += nota;
    
    return temp;
}

