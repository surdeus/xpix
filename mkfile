<mkconfig

SRC = `{ ls *.c }
OBJ = ${SRC:%.c=%.o}
#HDR = `{ ls *.h }
TGT = $NAME

all :VQ: $TGT
	echo -n
$TGT : $OBJ
	$LD -o $target $LDFLAGS $prereq
%.o : %.c
	$CC -c -o $target $CFLAGS $prereq
%.c :Q: $HDR
	touch $target
%.h :Q:
	touch $target
install : $TGT
	mkdir -p $BIN
	cp -f $TGT $BIN
	chmod 0755 $BIN/$TGT 
uninstall: 
	rm -f $BIN/$TGT
clean :
	rm -rf $TGT *.o 
