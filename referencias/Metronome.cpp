//
//  Metronome.cpp
//  metronome
//
//  Created by CEM on 23/09/21.
//

#include "Metronome.h"

// Constructor:
Metronome::Metronome(void)
{
    tracking = false;
    lastStamp = 0;
    currDelta = 0;
    timeOut = 0;
    latitude = DEFAULT_LATITUDE;
    cout << "metronome created!" << endl;
}

// destructor...
Metronome::~Metronome(void)
{
    // nothing to do here
    cout << "metronome destroyed!" << endl;
}

void Metronome::SetLatitude(double lat)
{
    if(lat >= 0)
        latitude = lat;
}

void Metronome::SetInitialBPM(double newTempo)
{
    if(newTempo > 0)
        currDelta = ((1.0/newTempo)*60);
}

// This method takes a time stamp from a recently
// received MIDI event and calculates a tempo value
// based on previous hits.
void Metronome::NewStamp(double newStamp)
{
    // if this is the first stamp received,
    // we start tracking time...
    if(!tracking)
    {
        lastStamp = newStamp;
        tracking = true;
    }
    else // if we are already tracking...
    {
        double max = currDelta + (latitude * currDelta);
        double min = currDelta - (latitude * currDelta);
        double delta = newStamp - lastStamp;
        
        // If the new beat is not too far from ongoing
        // tempo, we assume the new tempo..
        if(delta >= min && delta <= max)
        {
            currDelta = delta;
            lastStamp = newStamp;
        }
        else
        {
            // otherwise we keep the old tempo and...
            // stop tracking until we get a new event
            tracking = false;
            
        }
    }
}

double Metronome::BPM(void)
{
    return ((1.0/currDelta)*60);
}

double Metronome::Tempo(void)
{
    return currDelta;
}
