BINDIR = ../bin

all: bin_dir iion qauto plambda disp_statistics

bin_dir:
	mkdir -p $(BINDIR)


iion: iio.o iion.c
	cc iio.o iion.c -lpng -ltiff -ljpeg -lm -o $(BINDIR)/iion

qauto: iio.o qauto.o
	cc iio.o qauto.o -lpng -ltiff -ljpeg -lm -o $(BINDIR)/qauto

plambda: iio.o plambda.o
	cc iio.o plambda.o -lpng -ltiff -ljpeg -lm -o $(BINDIR)/plambda

disp_statistics: disp_statistics.o iio.o
	cc disp_statistics.o iio.o -lpng -ltiff -ljpeg -o $(BINDIR)/disp_statistics


iio.o: iio.c iio.h
	cc -c -std=c99 -g -O3 -DNDEBUG -DDONT_USE_TEST_MAIN iio.c -Wno-deprecated-declarations

qauto.o: qauto.c iio.h
	cc -c -std=c99 -g -O3 -DNDEBUG -DDONT_USE_TEST_MAIN qauto.c

plambda.o: plambda.c smapa.h fail.c xmalloc.c random.c parsenumbers.c colorcoords.c getpixel.c iio.h
	cc -c -std=c99 -g -O3 -DNDEBUG -DDONT_USE_TEST_MAIN plambda.c

disp_statistics.o: disp_statistics.c
	cc -c -std=c99 -g -O3 disp_statistics.c

clean:
	rm *.o
