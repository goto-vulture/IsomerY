/**
 * @file DMMC.h
 *
 * @brief Dynamischer Makro Memory Container
 *
 * Ein einfaches Containerkonzept, welches ueber Makros versucht einen allgemeinen dynamischen Container fuer
 * alle Datentypen bereitzustellen.
 *
 * Dabei wird ein Makroparameter fuer den Typ, der im Container gespeichert werden soll, verwendet. Dies entspricht
 * einem sehr rudimentaeren Template-Konzept wie in C++.
 *
 * @date 16.01.2022
 * @author am1
 */

#ifndef SRC_DMMC_H
#define SRC_DMMC_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <string.h>
#include "Error_Handling/Dynamic_Memory.h"
#include "Error_Handling/Assert_Msg.h"



#ifndef ALLOCATION_STEP_SIZE
#define ALLOCATION_STEP_SIZE 10 ///< Allokationsschritt
#else
#error "The macro \"ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* ALLOCATION_STEP_SIZE */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Container mit allen Funktionen sowie die notwendige Strukturdefinition erzeugen.
 */
#ifndef CREATE_CONTAINER
#define CREATE_CONTAINER(object_type, object_type_name)                     \
    CREATE_CONTAINER_STRUCT(object_type, object_type_name)                  \
    CREATE_CREATE_CONTAINER_FUNCTION(object_type, object_type_name)         \
    CREATE_DELETE_CONTAINER_FUNCTION(object_type_name)                      \
    CREATE_GET_ELEMENT_CONTAINER_FUNCTION(object_type, object_type_name)    \
    CREATE_SET_ELEMENT_CONTAINER_FUNCTION(object_type, object_type_name)    \
    CREATE_APPEND_ELEMENT_CONTAINER_FUNCTION(object_type, object_type_name)
#else
#error "The macro \"CREATE_CONTAINER\" is already defined !"
#endif /* CREATE_CONTAINER */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Strukturdefinition fuer den dynamischen Container erzeugen. Hierbei ist der Objekt-Typ ein Makroparameter.
 */
#ifndef CREATE_CONTAINER_STRUCT
#define CREATE_CONTAINER_STRUCT(object_type, object_type_name)  \
    struct object_type_name ## _container                       \
{                                                               \
    object_type* data;                                          \
                                                                \
    size_t one_object_size;                                     \
    uint_fast64_t container_size;                               \
    uint_fast64_t next_free_element;                            \
};
#else
#error "The macro \"CREATE_CONTAINER_STRUCT\" is already defined !"
#endif /* CREATE_CONTAINER_STRUCT */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Erzeugungsfunktion fuer den dynamischen Container mittels eines Makros erzeugen.
 */
#ifndef CREATE_CREATE_CONTAINER_FUNCTION
#define CREATE_CREATE_CONTAINER_FUNCTION(object_type, object_type_name)                                             \
    struct object_type_name ## _container* Create_ ## object_type_name ## _Container(void);                         \
    struct object_type_name ## _container* Create_ ## object_type_name ## _Container(void)                          \
    {                                                                                                               \
        struct object_type_name ## _container* new_container =                                                      \
                (struct object_type_name ## _container*) CALLOC (1, sizeof(struct object_type_name ## _container)); \
        if (new_container == NULL) { return NULL; }                                                                 \
                                                                                                                    \
        new_container->one_object_size = sizeof(object_type);                                                       \
                                                                                                                    \
        new_container->data = (object_type*) CALLOC (ALLOCATION_STEP_SIZE, sizeof(object_type));                    \
        ASSERT_ALLOC(new_container->data, "Try to create a " #object_type_name " container.",                       \
                ALLOCATION_STEP_SIZE * sizeof(object_type))                                                         \
        new_container->container_size = ALLOCATION_STEP_SIZE;                                                       \
        return new_container;                                                                                       \
    }
#else
#error "The macro \"CREATE_CREATE_CONTAINER_FUNCTION\" is already defined !"
#endif /* CREATE_CREATE_CONTAINER_FUNCTION */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Loeschfunktion fuer den dynamischen Container erzeugen. Hierbei wird ein Praeprozessormakro verwendet.
 */
#ifndef CREATE_DELETE_CONTAINER_FUNCTION
#define CREATE_DELETE_CONTAINER_FUNCTION(object_type_name)                                              \
    void Delete_ ## object_type_name ## _Container(struct object_type_name ## _container* container);   \
    void Delete_ ## object_type_name ## _Container(struct object_type_name ## _container* container)    \
    {                                                                                                   \
        if (container == NULL) { return; }                                                              \
        if (container->data != NULL) { FREE_AND_SET_TO_NULL (container->data); }                        \
                                                                                                        \
        FREE_AND_SET_TO_NULL (container);                                                               \
        return;                                                                                         \
    }
#else
#error "The macro \"CREATE_DELETE_CONTAINER_FUNCTION\" is already defined !"
#endif /* CREATE_DELETE_CONTAINER_FUNCTION */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get-Funktion fuer den dynamischen Container mittels Makro erzeugen.
 *
 * Die Get-Funktion des Containers gibt das Objekt an der Stelle des uebergebenen Indexes zurueck.
 */
#ifndef CREATE_GET_ELEMENT_CONTAINER_FUNCTION
#define CREATE_GET_ELEMENT_CONTAINER_FUNCTION(object_type, object_type_name)                                        \
    object_type Get_ ## object_type_name ## _Container_Element(struct object_type_name ## _container* container,    \
            uint_fast64_t index);                                                                                   \
    object_type Get_ ## object_type_name ## _Container_Element(struct object_type_name ## _container* container,    \
    uint_fast64_t index)                                                                                            \
    {                                                                                                               \
        if (container == NULL) { return 0; }                                                                        \
        if (container->container_size <= index) { return 0; }                                                       \
        return container->data[index];                                                                              \
    }
#else
#error "The macro \"CREATE_GET_ELEMENT_CONTAINER_FUNCTION\" is already defined !"
#endif /* CREATE_GET_ELEMENT_CONTAINER_FUNCTION */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Set-Funktion fuer den dynamischen Container mithilfe eines Makros erzeugen.
 *
 * Die erzeugte Funktion uebernimmt einen Index, wo das neue Objekt im Speicherbereich des Containers gesichert werden
 * soll.
 *
 * Bei der Verwendung des Containers sollte NUR die GET-Funktion ODER die Append-Funktion verwendet werden, da der
 * Zaehler der Append-Funktion durch die Set-Funktion nicht angepasst wird.
 */
#ifndef CREATE_SET_ELEMENT_CONTAINER_FUNCTION
#define CREATE_SET_ELEMENT_CONTAINER_FUNCTION(object_type, object_type_name)                                                    \
    void Set_ ## object_type_name ## _Container_Element(struct object_type_name ## _container* container,                       \
            object_type element, uint_fast64_t index);                                                                          \
    void Set_ ## object_type_name ## _Container_Element(struct object_type_name ## _container* container,                       \
            object_type element, uint_fast64_t index)                                                                           \
    {                                                                                                                           \
        if (container->next_free_element >= container->container_size)                                                          \
        {                                                                                                                       \
            container->container_size += ALLOCATION_STEP_SIZE;                                                                  \
            container->data = (object_type*) REALLOC (container->data, container->container_size * container->one_object_size); \
            ASSERT_ALLOC(container->data, "Try to reallocate the memory for a/an " #object_type_name " container.",             \
                    container->container_size * container->one_object_size)                                                     \
            memset(container->data + (container->container_size - ALLOCATION_STEP_SIZE), '\0',                                  \
                    ALLOCATION_STEP_SIZE * container->one_object_size);                                                         \
        }                                                                                                                       \
        container->data [index] = element;                                                                                      \
        return;                                                                                                                 \
    }
#else
#error "The macro \"CREATE_SET_ELEMENT_CONTAINER_FUNCTION\" is already defined !"
#endif /* CREATE_SET_ELEMENT_CONTAINER_FUNCTION */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Append-Funktion fuer den dynamischen Container mittels eines Makros erzeugen.
 *
 * Die Append-Funktion fuegt das uebergebene Objekt an der naechsten freien Stelle im Container ab. Dabei wird NICHT der
 * Container nach freien Elementen durchsucht. Das uebergebene Objekt wird einfach an der naechsten freien Stelle, die
 * anhand eines einfachen Zaehlers ermittelt wird, gesichert.
 *
 * Bei der Verwendung des Containers sollte NUR die GET-Funktion ODER die Append-Funktion verwendet werden, da der
 * Zaehler der Append-Funktion durch die Set-Funktion nicht angepasst wird.
 */
#ifndef CREATE_APPEND_ELEMENT_CONTAINER_FUNCTION
#define CREATE_APPEND_ELEMENT_CONTAINER_FUNCTION(object_type, object_type_name)                                                 \
    void Append_ ## object_type_name ## _Container_Element(struct object_type_name ## _container* container,                    \
            object_type element);                                                                                               \
    void Append_ ## object_type_name ## _Container_Element(struct object_type_name ## _container* container,                    \
            object_type element)                                                                                                \
    {                                                                                                                           \
        if (container->next_free_element >= container->container_size)                                                          \
        {                                                                                                                       \
            container->container_size += ALLOCATION_STEP_SIZE;                                                                  \
            container->data = (object_type*) REALLOC (container->data, container->container_size * container->one_object_size); \
            ASSERT_ALLOC(container->data, "Try to reallocate the memory for a/an " #object_type_name " container.",             \
                    container->container_size * container->one_object_size)                                                     \
            memset(container->data + container->next_free_element, '\0',                                                        \
                    ALLOCATION_STEP_SIZE * container->one_object_size);                                                         \
        }                                                                                                                       \
        container->data [container->next_free_element] = element;                                                               \
        container->next_free_element ++;                                                                                        \
        return;                                                                                                                 \
    }
#else
#error "The macro \"CREATE_APPEND_ELEMENT_CONTAINER_FUNCTION\" is already defined !"
#endif /* CREATE_APPEND_ELEMENT_CONTAINER_FUNCTION */

//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SRC_DMMC_H */
