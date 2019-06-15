OBJ_DIR = ./obj
SUBDIRS  = ./src/server
SUBDIRS += ./src/client


all:
	@for subdir in $(SUBDIRS); do(\
		cd $$subdir && \
		make all \
	)done

clean:
	rm -f $(OBJ_DIR)/*.o
