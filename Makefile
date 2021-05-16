EXE = pipe_sync pipe_ls_wc
all: ${EXE}

clean:
	${RM} ${EXE} *.o
