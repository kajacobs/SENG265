DEBUG=-DDEBUG
CFLAGS =-Wall -g -std=c99
LIBS =-I.
% : %.c
	gcc $< -o $@ $(CFLAGS) $(LIBS)
	gcc $(DEBUG) $< -o $@.debug $(CFLAGS) $(LIBS)