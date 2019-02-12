SRC=main.C image.C sink.C source.C PNMreader.C PNMwriter.C filters.C logging.C
OBJ=$(SRC:.C=.o)

prog: $(OBJ)
	g++ $(OBJ) -o image_manipulator

.C.o: $<
	g++  -g -I. -c $<

clean:
	rm *.o image_manipulator logger.txt checksum
