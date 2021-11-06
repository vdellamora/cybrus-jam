OUTFOLDER=compiledMuM
MUMFOLDER=MuM
compile:
	echo "Compilando CybrusJam..."
	g++ -g -D__LINUX_ALSA__ -o CYBRUSJAM cybrusMain.cpp baixoAnalise.cpp guitarrista.cpp baterista.cpp $(OUTFOLDER)/MuUtil.o $(OUTFOLDER)/MuError.o $(OUTFOLDER)/MuParamBlock.o $(OUTFOLDER)/MuNote.o $(OUTFOLDER)/MuVoice.o $(OUTFOLDER)/MuMaterial.o $(OUTFOLDER)/MuPlayer.o $(OUTFOLDER)/MuRecorder.o $(MUMFOLDER)/RtMidi.cpp -lasound -lpthread
	echo "Alterando permissoes do arquivo executavel..."
	chmod 755 CYBRUSJAM

compileFULL:
	echo "Compilando CybrusJam..."
	g++ -g -D__LINUX_ALSA__ -o CYBRUSJAM cybrusMain.cpp $(OUTFOLDER)/MuUtil.o $(OUTFOLDER)/MuError.o $(OUTFOLDER)/MuParamBlock.o $(OUTFOLDER)/MuNote.o $(OUTFOLDER)/MuVoice.o $(OUTFOLDER)/MuMaterial.o $(OUTFOLDER)/MuPlayer.o $(OUTFOLDER)/MuRecorder.o $(MUMFOLDER)/RtMidi.cpp -lasound -lpthread
	echo "Alterando permissoes do arquivo executavel..."
	chmod 755 CYBRUSJAM

compileMUM:
	mkdir $(OUTFOLDER)
	cd $(OUTFOLDER)/
	echo "Compilando MuUtil..."
	g++ -g -c $(MUMFOLDER)/MuUtil.cpp -o ./$(OUTFOLDER)/MuUtil.o
	echo "Compilando MuError..."
	g++ -g -c $(MUMFOLDER)/MuError.cpp -o ./$(OUTFOLDER)/MuError.o
	echo "Compilando MuParamBlock..."
	g++ -g -c $(MUMFOLDER)/MuParamBlock.cpp -o ./$(OUTFOLDER)/MuParamBlock.o
	echo "Compilando MuNote..."
	g++ -g -c $(MUMFOLDER)/MuNote.cpp -o ./$(OUTFOLDER)/MuNote.o
	echo "Compilando MuVoice..."
	g++ -g -c $(MUMFOLDER)/MuVoice.cpp -o ./$(OUTFOLDER)/MuVoice.o
	echo "Compilando MuMaterial..."
	g++ -g -c $(MUMFOLDER)/MuMaterial.cpp -o ./$(OUTFOLDER)/MuMaterial.o
	echo "Compilando MuPlayer..."
	g++ -g -c $(MUMFOLDER)/MuPlayer.cpp -o ./$(OUTFOLDER)/MuPlayer.o
	echo "Compilando MuRecorder..."
	g++ -c $(MUMFOLDER)/MuRecorder.cpp -o ./$(OUTFOLDER)/MuRecorder.o



	