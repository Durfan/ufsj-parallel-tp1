TARGET = tp1
PROCESS_COUNT = 4
GCC_FLAGS = -Wall -g -lm -fopenmp

RUN_OPTIONS = --use-hwthread-cpus
#RUN_OPTIONS = -np ${PROCESS_COUNT}

RUN_ARGS = entrada.txt

FILES := $(shell find . -type f -name '*.c' -printf '%P\n')


# Shell commands
GCC = mpicc
RUN = mpirun ${RUN_OPTIONS}

run: ${TARGET}
	${RUN} $< ${RUN_ARGS}

${TARGET}: ${OBJS}
	${GCC} ${FILES} -o ${TARGET} ${GCC_FLAGS}

clean:
	${RM} $(shell find . -type f -name '*\.o')
	${RM} ${TARGET}