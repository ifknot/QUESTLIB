/**
 * @file grayscale_converter.h
 * @brief Grayscale image to CP437 character conversion
 *
 * @defgroup text_rendering Text Rendering Utilities
 * @{
 */

#include <stdint.h>
#include <string.h>

/**
 * @brief Code Page 437 characters ordered by approximate darkness
 * @details Characters range from lightest (space) to darkest (block)
 *          Includes both ASCII and extended CP437 characters
 */static const char cp437_chars[] = {
    ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@',
    '░', '▒', '▓', '█', '≡', '≣', '⌂', '§', '±', '■'
};

/** Number of available characters in cp437_chars */
#define NUM_CHARS (sizeof(cp437_chars)/sizeof(cp437_chars[0]))

/**
 * @brief Function pointer type for interpolation methods
 * @param block 4x4 pixel block (16-byte array)
 * @return Computed grayscale value (0-255)
 */
typedef uint8_t (*interpolation_func)(const uint8_t* block);

// ---- Interpolation Methods ----

/**
 * @brief Mean value interpolation
 * @param block 4x4 pixel block
 * @return Average value of all pixels
 *
 * @details Simple arithmetic mean of all 16 pixels
 */
uint8_t mean_interpolation(const uint8_t* block) {
    int sum = 0;
    for (int i = 0; i < 16; i++) sum += block[i];
    return sum / 16;
}

/**
 * @brief Median value interpolation
 * @param block 4x4 pixel block
 * @return Median value of all pixels
 *
 * @details Uses average of two middle values after sorting
 *          More resistant to outliers than mean interpolation
 */
uint8_t median_interpolation(const uint8_t* block) {
    uint8_t sorted[16];
    memcpy(sorted, block, 16);

    // Simple bubble sort for small array
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15 - i; j++) {
            if (sorted[j] > sorted[j+1]) {
                uint8_t temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }
    return (sorted[7] + sorted[8]) / 2;
}

/**
 * @brief Error diffusion interpolation (stub)
 * @param block 4x4 pixel block
 * @return Base implementation returns simple mean
 *
 * @note Currently implements basic mean interpolation
 *       Should be enhanced with proper error diffusion
 * @todo Implement Floyd-Steinberg or other error diffusion
 */
uint8_t error_diffusion_interpolation(const uint8_t* block) {
    // Simple error diffusion (Floyd-Steinberg inspired)
    // For 4x4 blocks, we use a simplified approach
    int sum = 0;
    for (int i = 0; i < 16; i++) sum += block[i];
    return sum / 16; // Base implementation - see note above
}

// ---- Main Conversion Function ----

/**
 * @brief Converts grayscale image to CP437 character representation
 * @param[in] input 320x320 grayscale image (row-major order)
 * @param[out] output Buffer for 80x80 character output + newlines
 * @param[in] output_size Must be at least 80*81 bytes
 * @param[in] interpolate Function pointer to interpolation method
 *
 * @details Performs 4x4 pixel block conversion:
 *          1. Divides 320x320 image into 80x80 blocks (4x4 pixels each)
 *          2. Applies selected interpolation method to each block
 *          3. Maps resulting value to CP437 character by darkness
 *          4. Adds newlines for 80-character wide output
 *
 * @pre input != NULL
 * @pre output != NULL
 * @pre output_size >= 80*81
 * @pre interpolate != NULL
 *
 * @warning No bounds checking on input array
 * @note Output includes 80 newlines and null-terminator
 */
void grayscale_to_cp437(
    const uint8_t* input,
    char* output,
    size_t output_size,
    interpolation_func interpolate
) {
    if (output_size < 80*81) return;

    uint8_t block[16];

    for (int y = 0; y < 80; y++) {
        for (int x = 0; x < 80; x++) {
            // Extract 4x4 block
            for (int dy = 0, idx = 0; dy < 4; dy++) {
                for (int dx = 0; dx < 4; dx++) {
                    block[idx++] = input[(y * 4 + dy) * 320 + (x * 4 + dx)];
                }
            }

            // Use selected interpolation method
            uint8_t value = interpolate(block);

            // Map to character
            int index = (value * (NUM_CHARS - 1)) / 255;
            output[y * 81 + x] = cp437_chars[index];
        }
        output[y * 81 + 80] = '\n';
    }
    output[80*81] = '\0';
}

/** @} */ // end of text_rendering group

/*
// ---- Main Function with Method Selection ----

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s [mean|median|diffusion]\n", argv[0]);
        return 1;
    }

    // Select interpolation method
    interpolation_func method = NULL;

    if (strcmp(argv[1], "mean") == 0) {
        method = mean_interpolation;
    }
    else if (strcmp(argv[1], "median") == 0) {
        method = median_interpolation;
    }
    else if (strcmp(argv[1], "diffusion") == 0) {
        method = error_diffusion_interpolation;
    }
    else {
        printf("Unknown method: %s\n", argv[1]);
        return 1;
    }

    // Example usage
    uint8_t image[320*320];
    // ... load your image data here ...

    char text_output[80*81 + 1];
    grayscale_to_cp437(image, text_output, sizeof(text_output), method);

    printf("%s", text_output);
    return 0;
}
*/
