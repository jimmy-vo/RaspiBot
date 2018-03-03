CFLAGS=	-Imongoose -pthread -lwiringPi -g

all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl" ; \
	  $(CC) $(CFLAGS) RaspiBot.c mongoose.c  $$LIBS $(ADD) -o RaspiBot
