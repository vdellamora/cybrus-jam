void ConfigureMIDIInstruments(void){
    // THIS BUFFER NEEDS TO BE DYNAMICALLY ALLOCATED OTHERWISE
    // IT WILL BE INVALID BY THE TIME IT NEEDS TO BE USED!!!!
    // FIX IT!
    // set up instrument choice for every channel...
    MuMIDIBuffer programChanges;
    programChanges.data = new MuMIDIMessage[4];
    if(programChanges.data){
        programChanges.max = 4;
        programChanges.count = 4;
        
        // CHANNEL 1 = PROGRESSION
        programChanges.data[0].time = 0;
        programChanges.data[0].status = 0xC0;
        programChanges.data[0].data1 = 0;
        programChanges.data[0].data2 = 0;
        // CHANNEL 2 = TROMBONE
        programChanges.data[1].time = 0;
        programChanges.data[1].status = 0xC1;
        programChanges.data[1].data1 = 57;
        programChanges.data[1].data2 = 57;
        // CHANNEL 3 = PIANO RIGHT HAND
        programChanges.data[2].time = 0;
        programChanges.data[2].status = 0xC2;
        programChanges.data[2].data1 = 0;
        programChanges.data[2].data2 = 0;
        // CHANNEL 4 = PIANO LEFT HAND
        programChanges.data[3].time = 0;
        programChanges.data[3].status = 0xC3;
        programChanges.data[3].data1 = 0;
        programChanges.data[3].data2 = 0;
        
        if (!player.SendEvents(programChanges)){
            cout << "Nao foi possivel configurar canais/instrumentos!" << endl;
        }
    }
}