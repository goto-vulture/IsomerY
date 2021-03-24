##### ##### ##### ##### ###### SConstruct Skript fuer IsomerY ##### ##### ##### ##### #####
##### ##### ##### BEGINN Compilereinstellungen ##### ##### #####
debug_c = Environment(
    CC="gcc",

    CCFLAGS=[
        # C Standard (C11)
        "-std=c11",
        # Optimierung
        "-O0",
        # Warnungen
        "-pedantic",
        "-Wall",
        "-Wextra",
        "-Wconversion"]
)
debug_cpp = Environment(
    CC="g++",

    CCFLAGS=[
        # C++ Standard (C++11)
        "-std=c++0x",
        # Optimierung
        "-O0",
        # Warnungen
        "-pedantic",
        "-Wall",
        "-Wextra",
        "-Wconversion"]
)

release_c = Environment(
    CC="gcc",

    CCFLAGS=[
        # C Standard (C11)
        "-std=c11",
        # Optimierung
        "-O3",
        # Warnungen
        "-pedantic",
        "-Wall",
        "-Wextra",
        "-Wconversion"]
)
release_cpp = Environment(
    CC="g++",

    CCFLAGS=[
        # C++ Standard (C++11)
        "-std=c++0x",
        # Optimierung
        "-O3",
        # Warnungen
        "-pedantic",
        "-Wall",
        "-Wextra",
        "-Wconversion"]
)
##### ##### ##### ENDE Compilereinstellungen ##### ##### #####

##### ##### ##### BEGINN Liste der Quellcodedateien ##### ##### #####
##### Liste der vorhandenen C-Dateien #####
c_file_list = [
    "src/main.c",

    # Dateien fuer das Error Handling
    "src/Error_Handling/Dynamic_Memory.c",

    # Dateien fuer die Erstellung der Alkane
    "src/Alkane/Alkane_Info_Constitutional_Isomer.c",
    "src/Alkane/Alkane_Create_Constitutional_Isomer.c",
    "src/Alkane/Alkane_Branch.c",
    "src/Alkane/Alkane_Branch_Container.c",
    "src/Alkane/Alkane.c",
    "src/Alkane/Alkane_Container.c",
    "src/Alkane/Alkane_To_IUPAC_Name.c",
    "src/Alkane/Alkane_Chain_To_IUPAC_Name.c",

    # Dateien fuer das Testen
    "src/Tests/Alkane_Tests.c",
    "src/Tests/tinytest.c",

    # str2int Konverter
    "src/str2int.c"
]

##### Liste der vorhandenen Cpp-Dateien #####
cpp_file_list = [
]
##### ##### ##### ENDE Liste der Quellcodedateien ##### ##### #####



# Mit dem MD5-Hash ueber ein Neubau entscheiden
Decider("MD5")

# Infos ueber die CLI-Parameter ausgeben
print ""
print "debug=0|1:            Compile the debug version (default)"
print "release=0|1:          Compile the release version"
print "system=Windows|Linux: Available target systems (default: Linux)"

# CLI-Parameter abfragen
debug = ARGUMENTS.get("debug", 0)
release = ARGUMENTS.get("release", 0)
system = ARGUMENTS.get("system", 0)
debug_int = 0
release_int = 0
target_system = ""

print ""



##### ##### ##### BEGINN CLI Parameter verarbeiten ##### ##### #####
# Wenn kein Parameter fuer die Buildvarianten angegeben wurde, dann wird der Debug-Build verwendet
if (int(debug) == 0 and int(release) == 0):
    print "No build configuration selected ! Using default build configuration (Debug)"
    debug_int = 1

# Wahl des Zielsystems (Standard-System ist Linux)
if (system == "Windows" or system == "windows" or system == "Win" or system == "win"):
    target_system = "windows"
elif (system == "Linux" or system == "linux"):
    target_system = "linux"
else:
    print "No target system selected ! Using default system (Linux)"
    target_system = "linux"

print ""

# "debug_int == 1", falls der Default-Fall verwendet wird
if (int(debug) == 1 or debug_int == 1):
    print "Build configuration: Debug-Build"
    debug_int = 1

if (int(release) == 1):
    print "Build configuration: Release-Build"
    release_int = 1

if (target_system == "linux"):
    print "Target system:       Linux"

if (target_system == "windows"):
    print "Target system:       Windows"

print ""
##### ##### ##### ENDE CLI Parameter verarbeiten ##### ##### #####



##### ##### ##### BEGINN C Dateien uebersetzen ##### ##### #####
c_object_list_debug = []
c_object_list_release = []

if (debug_int == 1):
    for i in range(len(c_file_list)):
        if (i < 9): # str(i + 1) !
            c_object_list_debug.append(debug_c.Object("debug_c_object_0" + str(i + 1), c_file_list[i]))
        else:
            c_object_list_debug.append(debug_c.Object("debug_c_object_" + str(i + 1), c_file_list[i]))

if (release_int == 1):
    for i in range(len(c_file_list)):
        if (i < 9): # str(i + 1) !
            c_object_list_release.append(release_c.Object("release_c_object_0" + str(i + 1), c_file_list[i]))
        else:
            c_object_list_release.append(release_c.Object("release_c_object_" + str(i + 1), c_file_list[i]))
##### ##### ##### ENDE C Dateien uebersetzen ##### ##### #####



##### ##### ##### BEGINN Cpp Dateien uebersetzen ##### ##### #####
cpp_object_list_debug = []
cpp_object_list_release = []

if (debug_int == 1):
    for i in range(len(cpp_file_list)):
        if (i < 9): # str(i + 1) !
            cpp_object_list_debug.append(debug_cpp.Object("debug_cpp_object_0" + str(i + 1), cpp_file_list[i]))
        else:
            cpp_object_list_debug.append(debug_cpp.Object("debug_cpp_object_" + str(i + 1), cpp_file_list[i]))

if (release_int == 1):
    for i in range(len(cpp_file_list)):
        if (i < 9): # str(i + 1) !
            cpp_object_list_release.append(release_cpp.Object("release_cpp_object_0" + str(i + 1), cpp_file_list[i]))
        else:
            cpp_object_list_release.append(release_cpp.Object("release_cpp_object_" + str(i + 1), cpp_file_list[i]))
##### ##### ##### ENDE Cpp Dateien uebersetzen ##### ##### #####



##### ##### ##### BEGINN Programm erzeugen ##### ##### #####
libs                    = []
target_lib_path         = []
cpp_defines             = []
linker_flags            = []
program_name_debug      = "IsomerY_Debug"
program_name_release    = "IsomerY_Release"

if (target_system == "linux"):
    cpp_defines.append("__linux__")

    program_name_debug += "_Linux"
    program_name_release += "_Linux"
elif (target_system == "windows"):
    cpp_defines.append("__win__")

    program_name_debug += "_Win"
    program_name_release += "_Win"
else:
    print "No target system specified !"
    exit(1)



if (debug_int == 1):
    debug_c.Program(program_name_debug,
        [
            # C-Objektdateien einbinden
            c_object_list_debug,

            # Cpp-Objektdateien einbinden
            #cpp_object_list_debug
        ],

        # CPP-Defines setzen
        #CPPDEFINES=cpp_defines,

        # Linkerflags setzen
        LINKFLAGS=linker_flags
    )

if (release_int == 1):
    release_c.Program(program_name_release,
        [
            # C-Objektdateien einbinden
            c_object_list_release,

            # Cpp-Objektdateien einbinden
            #cpp_object_list_release
        ],

        # CPP-Defines setzen
        #CPPDEFINES=cpp_defines,

        # Linkerflags setzen
        LINKFLAGS=linker_flags
    )
##### ##### ##### ENDE Programm erzeugen ##### ##### #####
