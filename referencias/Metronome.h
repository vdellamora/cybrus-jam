//
//  Metronome.hpp
//  metronome
//
//  Created by CEM on 23/09/21.
//

#ifndef Metronome_hpp
#define Metronome_hpp

#include <stdio.h>
#include <iostream>
using namespace std;


// This class creates a tempo control that can be used in
// music related code. The main difference from a regular
// metronome is that the tempo is set from a musician's
// pedal action, which sends timestamped MIDI events
// for calculating tempo. When the class gets the first
// hit from the pedal it starts tracking for new hits and
// updates the tempo, making an average with recent events.
// If the user stops tapping, it keeps the last calculated tempo
// until it gets another hit and stars tracking again.
// The class informs current tempo as an average time interval
// between time stamps in seconds.

// CONSTANTS:
const double DEFAULT_LATITUDE = 0.5;

class Metronome
{
    private:
    
    bool tracking;
    double lastStamp;
    double currDelta;
    double timeOut;
    double latitude;
    
    public:
    
    // Constructor & Destructor ==================================
    Metronome(void);
    ~Metronome(void);
    // ===========================================================
    
    // SetLatitude() ============================================
    // The metronome starts tracking beats as soon as first
    // timestamp arrives. The following method sets
    // how much latitude will be allowed when tracking beats
    // "latitude" here is defined as how much the user can strech
    // or contract the time between beats. It is used to decide
    // when to stop tracking.
    // ===========================================================
    void SetLatitude(double lat);
    // ===========================================================
    
    // SetInitialBPM() ===========================================
    // sets initial tempo as beats per minute
    // ===========================================================
    void SetInitialBPM(double newTempo);
    // ===========================================================
    
    // NewStamp() ================================================
    // adds a new time stamp to tempo calculation. If there is
    // a previous stamp (we are already tracking beats) this
    // new time is averaged with current tempo. If not, the
    // value is stored and tracking starts.
    // ===========================================================
    void NewStamp(double currStamp);
    // ===========================================================
    
    // BPM() =====================================================
    // return the tempo as beats per minute
    // ===========================================================
    double BPM(void);
    // ===========================================================
    
    // Tempo() ===================================================
    // return current tempo: how much time (seconds) between beats
    // ===========================================================
    double Tempo(void);
    // ===========================================================

};

#endif /* Metronome_hpp */
