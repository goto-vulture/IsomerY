##### ##### ##### ##### ###### Makefile Skript fuer IsomerY ##### ##### ##### ##### #####
# Das Programm kann sowohl ueber das Buildsystem "make" als auch ueber das Buildsystem "scons" erstellt werden
# Das "Makefile" fuer scons ist: SConstruct

CC = gcc
RM = rm

# Flags, die sowohl im Debug- als auch im Release-Build, verwendet werden
CCFLAGS = -std=c11 -pedantic -Wall -Wextra -Wconversion -fmessage-length=0

# Weitere hilfreiche Compilerflags
# Programmabbruch bei Ueberlauf von vorzeichenbehafteten Integers
# CCFLAGS += -ftrapv => Funktioniert leider nicht wie erhofft :(
# Warnung, wenn Gleitkommazahlen auf Gleichheit mittels == getestet werden
CCFLAGS += -Wfloat-equal
# Warnung, wenn Variablen ueberdeckt werden
CCFLAGS += -Wshadow
# Warnung, wenn irgendwas von der Groesse einer Funktion oder des void-Typs abhaengt
CCFLAGS += -Wpointer-arith
# Fest codierte Strings bekommen den Typ const char*
CCFLAGS += -Wwrite-strings
# Warnung, wenn ein switch Ausdruck keinen default Pfad besitzt
CCFLAGS += -Wswitch-default
# Warnung, wenn bei einem switch Ausdruck ein enum Typ verwendet wird und nicht alle moeglichen Konstanten im switch Ausdruck verwendet werden
CCFLAGS += -Wswitch-enum
# Warnung, wenn Code existiert, der nie erreicht werden kann
CCFLAGS += -Wunreachable-code
# Warnung, wenn eine Funktion ohne Parameterangabe deklariert oder definiert wird
CCFLAGS += -Wstrict-prototypes
# Warnung, wenn Formatstrings dynamisch erzeugt werden und und nicht zur Kompilierungszeit bekannt sind
CCFLAGS += -Wformat-nonliteral
# Warnung, wenn ein Wert sich selbst zugewiesen wird
CCFLAGS += -Winit-self
# Einige weitere Moeglichkeiten den Code etwas sicherer zu machen
CCFLAGS += -fstack-protector -Wl,-z,relro -Wl,-z,now -Wformat-security


# Debug Build: Keine Optimierung und das hoechste Debug Level
DEBUG_FLAGS = -O0 -g3 -D_FORTIFY_SOURCE=2

# Release Build: Hoechste Optimierung und keine Debug Informationen
RELEASE_FLAGS = -O3
# Optimierungen fuer den Linker (Ich wusste vorher gar nicht, dass es so ein Flag auch fuer den Linker gibt ... :o)
RELEASE_FLAGS += -Wl,-O1
# Positionsunabhaengigen Code erzeugen
RELEASE_FLAGS += -fPIE



DEBUG = 0
RELEASE = 0

PROJECT_NAME = IsomerY
# addsuffix, welches einen String am Ende einer Variable anbringt, kann das Ergebnis NICHT einer Variablen zuweisen, wenn diese
# Variable im Aufruf von addsuffix vorhanden ist !
# D.h.: test += $(addsuffix _X_, $(test)) ist NICHT moeglich !
# Daher der Umweg ueber mehrere Variablen
TEMP_1 =
TARGET =

# Zusaetzliche Flags fuer Windows
ADDITIONAL_WINDOWS_FLAGS = -Wno-pedantic-ms-format

# Der Debug-Build ist die Standardvariante, wenn nichts anderes angegeben wurde
# Fuer den Release-Build muss die Variable "Release", "RELEASE" oder "release" auf 1 gesetzt werden
ifeq ($(Release), 1)
	CCFLAGS += $(RELEASE_FLAGS)
	TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
	RELEASE = 1
else
	ifeq ($(RELEASE), 1)
		CCFLAGS += $(RELEASE_FLAGS)
		TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
		RELEASE = 1
	else
		ifeq ($(release), 1)
			CCFLAGS += $(RELEASE_FLAGS)
			TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
			RELEASE = 1
		else
			CCFLAGS += $(DEBUG_FLAGS)
			TEMP_1 = $(addsuffix _Debug_, $(PROJECT_NAME))
			DEBUG = 1
		endif
	endif
endif


# Unter Windows wird das Flag "-Wno-pedantic-ms-format" benoetigt, da die MinGW Implementierung nicht standardkonforme
# Formatstrings verwendet. Mit diesem Compilerflag wird die Warnung, dass die MinGW Formatstrings nicht dem Standard
# entsprechen, entfernt
ifeq ($(OS), Windows_NT)
	CCFLAGS += $(ADDITIONAL_WINDOWS_FLAGS)
	TARGET = $(addsuffix Win, $(TEMP_1))
else
	TARGET = $(addsuffix Linux, $(TEMP_1))
endif

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

ALKANE_TO_IUPAC_NAME_H = ./src/Alkane/Alkane_To_IUPAC_Name.h
ALKANE_TO_IUPAC_NAME_C = ./src/Alkane/Alkane_To_IUPAC_Name.c

TINYTEST_H = ./src/Tests/tinytest.h
TINYTEST_C = ./src/Tests/tinytest.c

ALKANE_TESTS_H = ./src/Tests/Alkane_Tests.h
ALKANE_TESTS_C = ./src/Tests/Alkane_Tests.c

ALKANE_CHAIN_TO_IUPAC_NAME_H = ./src/Alkane/Alkane_Chain_To_IUPAC_Name.h
ALKANE_CHAIN_TO_IUPAC_NAME_C = ./src/Alkane/Alkane_Chain_To_IUPAC_Name.c

# Hiervon gibt es nur eine Headerdatei !
ASSERT_MSG_H = ./src/Error_Handling/Assert_Msg.h
##### ##### ##### ENDE Uebersetzungseinheiten ##### ##### #####



# Komplettes Projekt erstellen
all: $(TARGET)
	@echo
	@echo IsomerY build completed !

$(TARGET): main.o str2int.o Dynamic_Memory.o Alkane_Branch_Container.o Alkane_Branch.o Alkane_Container.o Alkane.o Alkane_Info_Constitutional_Isomer.o Alkane_Create_Constitutional_Isomer.o Alkane_To_IUPAC_Name.o tinytest.o Alkane_Tests.o Alkane_Chain_To_IUPAC_Name.o
	@echo
	@echo Linking object files ...
	@echo
	$(CC) $(CCFLAGS) -o $(TARGET) main.o str2int.o Dynamic_Memory.o Alkane_Branch_Container.o Alkane_Branch.o Alkane_Container.o Alkane.o Alkane_Info_Constitutional_Isomer.o Alkane_Create_Constitutional_Isomer.o Alkane_To_IUPAC_Name.o tinytest.o Alkane_Tests.o Alkane_Chain_To_IUPAC_Name.o

##### BEGINN Die einzelnen Uebersetzungseinheiten #####
main.o: $(MAIN_C)
	@echo Build target: $(TARGET).
	@echo
ifeq ($(RELEASE), 1)
	@echo Using RELEASE build.
else
	@echo Using DEBUG build.
endif
	@echo
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

Alkane_Create_Constitutional_Isomer.o: $(ALKANE_CREATE_CONSTITUTIONAL_ISOMER_C) $(ASSERT_MSG_H) $(ALKANE_BRANCH_H) $(ALKANE_BRANCH_CONTAINER_H)
	$(CC) $(CCFLAGS) -c $(ALKANE_CREATE_CONSTITUTIONAL_ISOMER_C)

Alkane_To_IUPAC_Name.o: $(ALKANE_TO_IUPAC_NAME_C)
	$(CC) $(CCFLAGS) -c $(ALKANE_TO_IUPAC_NAME_C)

tinytest.o: $(TINYTEST_C)
	$(CC) $(CCFLAGS) -c $(TINYTEST_C)

Alkane_Tests.o: $(ALKANE_TESTS_C)
	$(CC) $(CCFLAGS) -c $(ALKANE_TESTS_C)

Alkane_Chain_To_IUPAC_Name.o: $(ALKANE_CHAIN_TO_IUPAC_NAME_C)
	$(CC) $(CCFLAGS) -c $(ALKANE_CHAIN_TO_IUPAC_NAME_C)
##### ENDE Die einzelnen Uebersetzungseinheiten #####

# Alles wieder aufraeumen
clean:
	@echo Clean IsomerY build.
	@echo
	$(RM) -f $(PROJECT_NAME)* *.o ./src/Alkane/*.gch ./src/Error_Handling/*.gch gmon.out
	@echo
	@echo IsomerY build cleaned.
