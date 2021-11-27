#include <iostream>
using namespace std;
#include "MuPlayer.h"
#include "MuRecorder.h"
#include "Metronome.h"

// PROTOTYPES ==========================================

// Builds music for one beat using  requested chord
MuMaterial BuildOneBeat(MuMaterial chord);

// gets current system time in seconds
double Now(void);
// =====================================================

// CONSTANTS ===========================================

const int BEATS_PER_MEASURE = 4;

// =====================================================

int main(void)
{
    cout << "Generating chords..." << endl;
    MuMaterial chords,triad;
    int chordProgression[BEATS_PER_MEASURE] = {0,0,3,4};
    int nextChord = 0;
    
    for(int i = 1; i <= 7; i++)
    {
        triad.Clear();
        triad.MajorTriad(1);
        triad.DiatonicTranspose(C_NAT, MAJOR_MODE, i, ASCENDING);
        chords += triad;
    }
    chords.SetInstrument(0, 1);
    
    cout << "Placing chords in separate voices..." << endl;
    chords = chords.Segments(7);
    
    cout << "Setting up Metronome..." << endl;
    Metronome met;
    met.SetLatitude(1.0);
    met.SetInitialBPM(60);
    double endOfBeat = 0;
    
    cout  << "Loading Recorder..." << endl;
    MuRecorder in;
    in.Init(100);
    in.SelectMIDISource(1);
    MuMIDIBuffer inBuff;
    inBuff.max = 0;
    inBuff.count = 0;
    inBuff.data = NULL;
    
    cout << endl << "Loading Player..." << endl;
    MuPlayer out;
    out.Init();
     
    cout << "Starting playback and waiting for pedal events..." << endl;
    MuMaterial beat;
    while(true)
    {
        usleep(100);
        
        // If we are starting playback, play current beat
        // and remember when it ends...
        if(endOfBeat == 0)
        {
            beat = BuildOneBeat(chords.GetVoice(chordProgression[nextChord]));
            nextChord++;
            if(nextChord >= BEATS_PER_MEASURE)
                nextChord = 0;
            beat.Scale(met.Tempo());
            out.Play(beat, PLAYBACK_MODE_NORMAL);
            endOfBeat = Now() + beat.Dur();
        }
        else
        {
            // If beat has ended, we flag so that
            // next time wround we play the next beat...
            if(Now() >= endOfBeat)
            {
                endOfBeat = 0.0;
            }
        }
        
        inBuff = in.GetData();
        if(inBuff.count > 0)
        {
            MuMIDIMessage msg;
            for(int i = 0; i < inBuff.count; i++)
            {
                msg = inBuff.data[i];
                //ShowMIDIMessage(msg);
                RemoveChannel(msg);
                
                if((msg.status == MU_CONTROL) && (msg.data1 == 0x10))
                {
                    met.NewStamp(msg.time);
                }
            }
        }
    }
    
    return 0;
}

MuMaterial BuildOneBeat(MuMaterial chord)
{
    //cout << "Building Rhythm..." << endl;
    MuMaterial rhythm,beat;
    MuNote rhythmNote,chordNote;
    
    // define note
    rhythmNote.SetInstr(2);
    rhythmNote.SetStart(0);
    rhythmNote.SetPitch(48);
    rhythmNote.SetAmp(1.0);
    
    // define durations
    // (sixteenth-sixteenth-eight)
    rhythmNote.SetDur(0.25);
    rhythm += rhythmNote;
    rhythmNote.SetDur(0.25);
    rhythm += rhythmNote;
    rhythmNote.SetDur(0.5);
    rhythm += rhythmNote;
    
    //cout << "Assembling beat..." << endl;
    // For each rhythm duration, add each of
    // the chord's notes
    for(int i = 0 ; i < rhythm.NumberOfNotes(); i++)
    {
        rhythmNote = rhythm.GetNote(i);
        for(int j = 0; j < chord.NumberOfNotes(); j++)
        {
            chordNote = chord.GetNote(j);
            rhythmNote.SetPitch(chordNote.Pitch());
            rhythmNote.SetInstr(2);
            beat.AddNote(0,rhythmNote);
        }
    }
    
    return beat;
}

double Now(void)
{
    timeval t;
    gettimeofday(&t, 0);
    double stamp = t.tv_sec + (t.tv_usec/1000000.0);
    return stamp;
}
