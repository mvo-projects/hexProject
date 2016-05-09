CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi
INCLUDE = includes/
SRC = hexboard.c hexsave.c hexdisplay.c hexhistory.c hexgame.c main.c
LIBS = -lSDL -lSDL_image -lSDL_ttf
ALLPROGRAMS = hex test_board test_save
PROGRAMS = hex
TEST_U_BOARD = test_board
TEST_U_SAVE = test_save
TEST_U_BOARD_SRC = hexboard.c test_u_hexboard.c
TEST_U_SAVE_SRC = hexboard.c hexsave.c test_u_hexsave.c
OBJETS = *.o

.SUFFIXES: .c .h .o

all: ${PROGRAMS}

hex:
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} $(SRC) -o $(PROGRAMS) $(LIBS)
	@echo
	@echo "(\\_(\\"
	@echo "(= ':')  COMPILATION TERMINEE, ELLERO/LAUTIE/VO - L2 G4.2" 
	@echo "(,(\")(\")"

tests:
	@echo
	@echo "################  COMPILATION TESTS  ################"
	${CC} -I${INCLUDE} ${CFLAGS} $(TEST_U_BOARD_SRC) -o $(TEST_U_BOARD)
	${CC} -I${INCLUDE} ${CFLAGS} $(TEST_U_SAVE_SRC) -o $(TEST_U_SAVE)
	@echo
	@echo "(\\_(\\"
	@echo "(= ':')  COMPILATION TERMINEE, ELLERO/LAUTIE/VO - L2 G4.2" 
	@echo "(,(\")(\")"

clean:
	rm -f  ${ALLPROGRAMS} ${OBJETS}

re:
	make clean
	make
