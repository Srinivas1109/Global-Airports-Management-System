234.exe : client.o PES1UG20CS517.o
	gcc client.o PES1UG20CS517.o -o 234.exe
client.o : client.c header.h
	gcc -c client.c
PES1UG20CS517.o : PES1UG20CS517.c header.h
	gcc -c PES1UG20CS517.c