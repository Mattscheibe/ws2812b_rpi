#Name of main file (without .c):
NAME=main

#More C-files:
MORE=ws2812b_rpi ws2812-RPi

#Additional files to be looked at:
NOTICE=Makefile

#Set according to your needs, e.g. gedit:
EDITOR=joe


CFLAGS=-g -O0 -Wall -Wformat
LDFLAGS=-lm

#Uncomment to include wiringPi (e.g. for delays):
#LDFLAGS+=-lwiringPi

SRC=$(NAME).c $(MORE:%=%.c)
OBJ=$(SRC:%.c=%.o)
NOTICE+=$(MORE:%=%.h)

all: $(NAME).bin

$(NAME).bin: $(OBJ) $(NOTICE)
	gcc ${CFLAGS} ${LDFLAGS} $(OBJ) -o $(NAME).bin

edit:
	$(EDITOR) $(NAME).c

make:
	$(EDITOR) Makefile

run: $(NAME).bin
	-sudo ./$(NAME).bin

clean:
	rm -f $(OBJ) $(NAME).bin D???JOE *~ *.bak
	
.PHONY: all make run clean edit
.SILENT:
