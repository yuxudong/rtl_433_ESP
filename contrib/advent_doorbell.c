/** @file
    Decoder for the Advent wireless doorbell on 433.92 MHz.

    Verified rtl_433 flex equivalent:

        modulation = OOK_PWM
        short      = 50
        long       = 150
        reset      = 400
        bits       = 15
        match      = {15}d5ea
*/

#include "decoder.h"

static int advent_doorbell_callback(r_device *decoder, bitbuffer_t *bitbuffer)
{
    for (unsigned row = 0; row < bitbuffer->num_rows; ++row) {
        if (bitbuffer->bits_per_row[row] != 15) {
            continue;
        }

        uint8_t const *b = bitbuffer->bb[row];

        // rtl_433 stores partial bytes left-aligned, so {15}d5ea is D5 EA.
        if (b[0] != 0xD5 || b[1] != 0xEA) {
            continue;
        }

        // Convert the left-aligned bitbuffer value D5 EA to the 15-bit ID 6AF5.
        uint16_t code = (((uint16_t)b[0] << 8) | b[1]) >> 1;
        char code_str[5];
        snprintf(code_str, sizeof(code_str), "%04x", code);

        /* clang-format off */
        data_t *data = data_make(
                "model", "", DATA_STRING, "Advent Doorbell",
                "code",  "", DATA_STRING, code_str,
                "event", "", DATA_STRING, "button_pressed",
                NULL);
        /* clang-format on */

        decoder_output_data(decoder, data);
        return 1;
    }

    return DECODE_FAIL_SANITY;
}

static char const *const output_fields[] = {
        "model",
        "code",
        "event",
        NULL,
};

r_device const advent_doorbell = {
        .name        = "Advent Doorbell",
        .modulation  = OOK_PULSE_PWM,
        .short_width = 50,
        .long_width  = 150,
        .sync_width  = 0,
        .gap_limit   = 0,
        .reset_limit = 400,
        .tolerance   = 0,
        .disabled    = 0,
        .decode_fn   = &advent_doorbell_callback,
        .fields      = output_fields,
};
