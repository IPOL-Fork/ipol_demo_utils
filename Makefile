CC = cc
CFLAGS = -std=c99 -g -O3 -DNDEBUG -DDONT_USE_TEST_MAIN
LDFLAGS = -lpng -ltiff -ljpeg -lm
SRC = src
PROGS = iion qauto plambda disp_statistics

all: $(PROGS)

iion: $(SRC)/iio.o $(SRC)/iion.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

qauto: $(SRC)/iio.o $(SRC)/qauto.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

plambda: $(SRC)/iio.o $(SRC)/plambda.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

disp_statistics: $(SRC)/disp_statistics.o $(SRC)/iio.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@


$(SRC)/iio.o: $(SRC)/iio.c $(SRC)/iio.h
	$(CC) $(CFLAGS) -Wno-deprecated-declarations -c $< -o $@

$(SRC)/qauto.o: $(SRC)/qauto.c $(SRC)/iio.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC)/plambda.o: $(SRC)/plambda.c $(SRC)/smapa.h $(SRC)/fail.c $(SRC)/xmalloc.c $(SRC)/random.c $(SRC)/parsenumbers.c $(SRC)/colorcoords.c $(SRC)/getpixel.c $(SRC)/iio.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC)/disp_statistics.o: $(SRC)/disp_statistics.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(SRC)/*.o $(PROGS)
