##### ##### ##### ##### ###### Makefile Skript fuer IsomerY ##### ##### ##### ##### #####
# Das Programm kann sowohl ueber das Buildsystem "make" als auch ueber das Buildsystem "scons" erstellt werden
# Das "Makefile" fuer scons ist: SConstruct

CC = gcc
RM = rm

# Flags, die sowohl im Debug- als auch im Release-Build, verwendet werden
CCFLAGS = -std=c11 -pedantic -Wall -Wextra -Wconversion -fmessage-length=0
# Debug Build: Keine Optimierung und das hoechste Debug Level
DEBUG_FLAGS = -O0 -g3
# Release Build: Hoechste Optimierung und keine Debug Informationen
RELEASE_FLAGS = -O3

DEBUG = 0
RELEASE = 0

ADDITIONAL_WINDOWS_FLAGS = -Wno-pedantic-ms-format

# Der Debug-Build ist die Standardvariante, wenn nichts anderes angegeben wurde
# Fuer den Release-Build muss die Variable "Release", "RELEASE" oder "release" auf 1 gesetzt werden
ifeq ($(Release), 1)
	CCFLAGS += $(RELEASE_FLAGS)
	RELEASE = 1
else
	ifeq ($(RELEASE), 1)
		CCFLAGS += $(RELEASE_FLAGS)
		RELEASE = 1
	else
		ifeq ($(release), 1)
			CCFLAGS += $(RELEASE_FLAGS)
			RELEASE = 1
		else
			CCFLAGS += $(DEBUG_FLAGS)
			DEBUG = 1
		endif
	endif
endif


# Unter Windows wird das Flag "-Wno-pedantic-ms-format" benoetigt, da die MinGW Implementierung nicht standardkonforme
# Formatstrings verwendet. Mit diesem Compilerflag wird die Warnung, dass die MinGW Formatstrings nicht dem Standard
# entsprechen, entfernt
ifeq ($(OS), Windows_NT)
	CCFLAGS += $(ADDITIONAL_WINDOWS_FLAGS)
endif

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

ALKANE_TO_IUPAC_NAME_H = ./src/Alkane/Alkane_To_IUPAC_Name.h
ALKANE_TO_IUPAC_NAME_C = ./src/Alkane/Alkane_To_IUPAC_Name.c

TINYTEST_H = ./src/Tests/tinytest.h
TINYTEST_C = ./src/Tests/tinytest.c

ALKANE_TESTS_H = ./src/Tests/Alkane_Tests.h
ALKANE_TESTS_C = ./src/Tests/Alkane_Tests.c

# Hiervon gibt es nur eine Headerdatei !
ASSERT_MSG_H = ./src/Error_Handling/Assert_Msg.h
##### ##### ##### ENDE Uebersetzungseinheiten ##### ##### #####



# Komplettes Projekt erstellen
all: $(TARGET)
	@echo
	@echo IsomerY build completed !

$(TARGET): main.o str2int.o Dynamic_Memory.o Alkane_Branch_Container.o Alkane_Branch.o Alkane_Container.o Alkane.o Alkane_Info_Constitutional_Isomer.o Alkane_Create_Constitutional_Isomer.o tinytest.o Alkane_Tests.o
	@echo
	@echo Linking object files ...
	@echo
	$(CC) $(CCFLAGS) -o $(TARGET) main.o str2int.o Dynamic_Memory.o Alkane_Branch_Container.o Alkane_Branch.o Alkane_Container.o Alkane.o Alkane_Info_Constitutional_Isomer.o Alkane_Create_Constitutional_Isomer.o tinytest.o Alkane_Tests.o

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

# Alles wieder aufraeumen
clean:
	@echo Clean IsomerY build.
	@echo
	$(RM) $(TARGET) *.o ./src/Alkane/*.gch ./src/Error_Handling/*.gch 2>> /dev/null
	@echo
	@echo IsomerY build cleaned.
