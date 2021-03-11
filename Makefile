##### ##### ##### ##### ###### Makefile Skript fuer IsomerY ##### ##### ##### ##### #####
# Das Programm kann sowohl ueber das Buildsystem "make" als auch ueber das Buildsystem "scons" erstellt werden
# Das "Makefile" fuer scons ist: SConstruct

CC = gcc
RM = rm
ECHO = echo
CCFLAGS = -std=c11 -O0 -pedantic -Wall -Wextra -Wconversion

# Aktuell wird durch das Makefile nur die Debug-Variante fuer Linux erstellt
TARGET = IsomerY_Debug_Linux

##### ##### ##### BEGINN Uebersetzungseinheiten ##### ##### #####
MAIN_C = ./src/main.c

STR2INT_H = ./src/str2int.h
STR2INT_C = ./src/str2int.c

DYNAMIC_MEMORY_H = ./src/Error_Handling/Dynamic_Memory.h
DYNAMIC_MEMORY_C = ./src/Error_Handling/Dynamic_Memory.c

ALKANE_BRANCH_CONTAINER_H = ./src/Alkane/Alkane_Branch_Container.h
ALKANE_BRANCH_CONTAINER_C = ./src/Alkane/Alkane_Branch_Container.c

ALKANE_BRANCH_H = ./src/Alkane/Alkane_Branch.h
ALKANE_BRANCH_C = ./src/Alkane/Alkane_Branch.c

ALKANE_CONTAINER_H = ./src/Alkane/Alkane_Container.h
ALKANE_CONTAINER_C = ./src/Alkane/Alkane_Container.c

ALKANE_H = ./src/Alkane/Alkane.h
ALKANE_C = ./src/Alkane/Alkane.c

ALKANE_CREATE_CONSTITUTIONAL_ISOMER_H = ./src/Alkane/Alkane_Create_Constitutional_Isomer.h
ALKANE_CREATE_CONSTITUTIONAL_ISOMER_C = ./src/Alkane/Alkane_Create_Constitutional_Isomer.c

ALKANE_INFO_CONSTITUTIONAL_ISOMER_H = ./src/Alkane/Alkane_Info_Constitutional_Isomer.h
ALKANE_INFO_CONSTITUTIONAL_ISOMER_C = ./src/Alkane/Alkane_Info_Constitutional_Isomer.c

# Hiervon gibt es nur eine Headerdatei !
ASSERT_MSG_H = ./src/Error_Handling/Assert_Msg.h
##### ##### ##### ENDE Uebersetzungseinheiten ##### ##### #####



# Komplettes Projekt erstellen
all: $(TARGET)

$(TARGET): main.o str2int.o Dynamic_Memory.o Alkane_Branch_Container.o Alkane_Branch.o Alkane_Container.o Alkane.o Alkane_Info_Constitutional_Isomer.o Alkane_Create_Constitutional_Isomer.o
	$(CC) $(CCFLAGS) -o $(TARGET) main.o str2int.o Dynamic_Memory.o Alkane_Branch_Container.o Alkane_Branch.o Alkane_Container.o Alkane.o Alkane_Info_Constitutional_Isomer.o Alkane_Create_Constitutional_Isomer.o

main.o: $(MAIN_C)
	$(CC) $(CCFLAGS) -c $(MAIN_C) $(DYNAMIC_MEMORY_H) $(ALKANE_H)

str2int.o: $(STR2INT_C)
	$(CC) $(CCFLAGS) -c $(STR2INT_C)

Dynamic_Memory.o: $(DYNAMIC_MEMORY_C)
	$(CC) $(CCFLAGS) -c $(DYNAMIC_MEMORY_C)

Alkane_Branch_Container.o: $(ALKANE_BRANCH_CONTAINER_C) $(ASSERT_MSG_H) $(DYNAMIC_MEMORY_H) $(ALKANE_INFO_CONSTITUTIONAL_ISOMER_H)
	$(CC) $(CCFLAGS) -c $(ALKANE_BRANCH_CONTAINER_C)

Alkane_Branch.o: $(ALKANE_BRANCH_C) $(ASSERT_MSG_H) $(DYNAMIC_MEMORY_H) $(ALKANE_INFO_CONSTITUTIONAL_ISOMER_H)
	$(CC) $(CCFLAGS) -c $(ALKANE_BRANCH_C)

Alkane_Container.o: $(ALKANE_CONTAINER_C)
	$(CC) $(CCFLAGS) -c $(ALKANE_CONTAINER_C)

Alkane.o: $(ALKANE_C) $(DYNAMIC_MEMORY_H) $(ASSERT_MSG_H) $(ALKANE_INFO_CONSTITUTIONAL_ISOMER_H)
	$(CC) $(CCFLAGS) -c $(ALKANE_C)

Alkane_Info_Constitutional_Isomer.o: $(ALKANE_INFO_CONSTITUTIONAL_ISOMER_C)
	$(CC) $(CCFLAGS) -c $(ALKANE_INFO_CONSTITUTIONAL_ISOMER_C)

Alkane_Create_Constitutional_Isomer.o: $(ALKANE_CREATE_CONSTITUTIONAL_ISOMER_C)
	$(CC) $(CCFLAGS) -c $(ALKANE_CREATE_CONSTITUTIONAL_ISOMER_C)



# Alles wieder aufraeumen
clean:
	$(RM) $(TARGET) *.o ./src/Alkane/*.gch ./src/Error_Handling/*.gch
