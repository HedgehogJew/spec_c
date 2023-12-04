#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>

/*
CRC32 (dataStatus)  Ошибка 2.7
    /\ IF dataStatus == CORRECT
        THEN
            /\ _out (TRUE)
        ELSE
            /\ _out (FALSE)
*/

uint_least32_t crc32_calculate(uint8_t *buf, size_t len);

/*
CRC16 (state)  Ошибка 2.6
    /\ IF state == CORRECT
        THEN
            /\ _out (TRUE)
        ELSE
            /\ _out (FALSE)
*/
uint16_t crc16_calculate(uint8_t *pcBlock, uint16_t len);

#endif // CRC_H
