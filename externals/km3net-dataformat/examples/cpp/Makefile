readEvtFile: readEvtFile.cc
	g++ -o $@ $< \
	-I../../../ \
	-L../../lib -lKM3NeTROOT \
	-luuid \
	`root-config --cflags --glibs`

clean:

.PHONY: clean
clean:
	rm -rf readEvtFile
