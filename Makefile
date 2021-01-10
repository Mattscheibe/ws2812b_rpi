compile: ws2812b_rpi.o
ws2812b_rpi.o: ws2812b_rpi.c ws2812b_rpi.h
	if [ ! -e /usr/local/include/ws2811/dma.h ]; then echo "ws281x library by J. Garff missing, please install it first"; fi; exit
	gcc -I/usr/local/include/ws2811 -c ws2812b_rpi.c
libws2812b_rpi.a: ws2812b_rpi.o
	ar rcs libws2812b_rpi.a ws2812b_rpi.o
	ranlib libws2812b_rpi.a
install: libws2812b_rpi.a ws2812b_rpi.h
	mkdir -p /usr/local/include/ws2812b_rpi; cp ws2812b_rpi.h /usr/local/include/ws2812b_rpi; mkdir -p /usr/local/lib; cp libws2812b_rpi.a /usr/local/lib
clean:
	$(RM) *~ *.o *.a
.PHONY: compile install
.SILENT:
