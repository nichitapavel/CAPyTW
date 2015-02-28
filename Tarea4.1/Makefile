EXECS=matrixSPMD-PavelNichita
MPICC?=mpicc

all: ${EXECS}

matrixSPMD-PavelNichita: matrixSPMD-PavelNichita.c
	${MPICC} -o matrixSPMD-PavelNichita.out matrixSPMD-PavelNichita.c

clean:
	rm ${EXECS}