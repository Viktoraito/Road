all: server client_way client_obst client_rule client_move client_proc car draw road

server: server.o
	gcc -o server `xml2-config --cflags` server.o `xml2-config --libs`
client_way: client_way.o
	gcc -o client_way  `xml2-config --cflags` client_way.o  `xml2-config --libs` -lpthread -lm 
client_obst: client_obst.o
	gcc -o client_obst `xml2-config --cflags` client_obst.o `xml2-config --libs` -lpthread -lm
client_rule: client_rule.o
	gcc -o client_rule `xml2-config --cflags` client_rule.o `xml2-config --libs` -lpthread -lm
client_move: client_move.o
	gcc -o client_move `xml2-config --cflags` client_move.o `xml2-config --libs` -lpthread -lm -lX11 -lglut -lGL 
client_proc: client_proc.o
	gcc -o client_proc `xml2-config --cflags` client_proc.o `xml2-config --libs` -lpthread -lm
car: car.o
	gcc -o car `xml2-config --cflags` car.o `xml2-config --libs` -lpthread -lm
draw: draw.o
	gcc -o draw `xml2-config --cflags` draw.o `xml2-config --libs` -lpthread -lm -lX11 -lglut -lGL -lSOIL
road: road.o
	gcc -o road `xml2-config --cflags` road.o `xml2-config --libs` -lpthread -lm

server.o: server.c
	gcc -c -o server.o -I/usr/include/libxml2 server.c
client_way.o: client_way.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_way.c  -lpthread
client_obst.o: client_obst.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_obst.c -lpthread
client_rule.o: client_rule.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_rule.c -lpthread
client_move.o: client_move.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_move.c -lpthread -lX11 -lglut -lGL
client_proc.o: client_proc.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_proc.c -lpthread
car.o: car.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 car.c -lpthread
draw.o: draw.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 draw.c -lpthread -lX11 -lglut -lGL -lSOIL
road.o: road.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 road.c -lpthread

clean:
	rm -rf server client_way client_obst client_move client_proc client_rule car draw road *.o ask*.xml reader*.xml *?Data?.xml
