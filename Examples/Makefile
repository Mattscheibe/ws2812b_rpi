#Name of main file (without .c)
NAME=main5

#Name of binary executable file to be produced 
TARGET=test

#Editor to be used, i.e. "joe" or "nano" or "gedit" or ...
EDITOR=nano

#Additional files to be looked at:
NOTICE=Makefile

LDFLAGS=-lm -lws2812b_rpi -lws2811

#Uncomment to include wiringPi (e.g. for input/output via GPIO pins):
#LDFLAGS+=-lwiringPi

$(TARGET): $(NAME).o $(NOTICE)
	gcc $< $(LDFLAGS) -o $@
$(NAME).o: $(NAME).c
	gcc -I/usr/local/include/ws2811 -I/usr/local/include/ws2812b_rpi -c $<
run: $(TARGET)
	sudo ./$(TARGET) -g 21 -c
clean:
	$(RM) *.o *~ $(TARGET)
edit:
	$(EDITOR) $(NAME).c
make:
	$(EDITOR) Makefile
.PHONY: run clean edit make
.SILENT:
