/**
 * @file To_XPM.h
 *
 * @brief Ueberfuehrung einer textbasierten Zeichnung in eine XPM-Datei.
 *
 * Bei XPM-Dateien handelt es sich um Grafiken, die aus reinen Textdateien bestehen. Daher koennen diese Grafiken ohne
 * externe Bibliothek erstellt werden. Technisch wird ja nur eine Textdatei erstellt.
 *
 * Siehe: https://de.wikipedia.org/wiki/X_PixMap
 *
 * @date 16.04.2022
 * @author am1
 */

#ifndef TO_XPM_H
#define TO_XPM_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Text_Based_Alkane_Drawing.h"



enum Export_To_XPM_Settings
{
    EXPORT_XPM_CHAR_SIZE_32_32 = 1 << 1,
    EXPORT_XPM_CHAR_SIZE_64_64 = 1 << 2,

    EXPORT_XPM_CHAR_PER_PIXEL_1 = 1 << 3,
    EXPORT_XPM_CHAR_PER_PIXEL_2 = 1 << 4
};

extern void Export_Text_Based_Drawing_To_XPM
(
        const struct Text_Based_Alkane_Drawing* const restrict input,
        const enum Export_To_XPM_Settings export_settings
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TO_XPM_H */
