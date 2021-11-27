#include "MuM/MuMaterial.h"
#include <iostream>
using namespace std;


int main(int argc, const char * argv[])
{
    MuMaterial mat;
    mat.MajorScale(1);
    mat.Show();
    MuNote note;
    
    
    int n = mat.NumberOfNotes();
    for (int i = 0; i < n; i++)
    {
        note = mat.GetNote(0,i);
        note.SetPitch(note.Pitch() + 12);
        mat.SetNote(0,i,note);
        cout << note.CsString() << endl;
    }
    
    return 0;
}