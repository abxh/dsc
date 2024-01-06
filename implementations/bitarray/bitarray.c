#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t ba_word;

typedef struct {
    size_t num_of_words;
    ba_word *words;
} Bitarray;

typedef ba_word (ba_update_word_f)(ba_word, size_t, size_t);
typedef int (ba_update_at_f)(int, size_t);

Bitarray *ba_new(size_t num_of_words) {
    Bitarray *bitarray = malloc(sizeof(Bitarray));
    if (bitarray == NULL) {
        return bitarray;
    }
    bitarray->words = calloc(num_of_words, sizeof(ba_word));
    bitarray->num_of_words = num_of_words;
    return bitarray;
}

int ba_get(Bitarray *bitarray_p, size_t index) {
    size_t n = index >> 3; // index / 8
    if (n >= bitarray_p->num_of_words) {
        return -1;
    }
    size_t m = ~index & 0b111; // 7 - (index % 8)
    return (bitarray_p->words[n] >> m) & 1;
}

int ba_set(Bitarray *bitarray_p, size_t index, int value) {
    size_t n = index >> 3; // index / 8
    if (n >= bitarray_p->num_of_words) {
        return -1;
    }
    size_t m = ~index & 0b111; // 7 - (index % 8)
    bitarray_p->words[n] &= ~(1 << m);
    bitarray_p->words[n] |= value << m;
    return 0;
}

int ba_update_word(Bitarray *bitarray_p, size_t index, ba_update_word_f update_func) {
    size_t n = index >> 3; // index / 8
    if (n >= bitarray_p->num_of_words) {
        return -1;
    }
    size_t m = ~index & 0b111; // 7 - (index % 8)
    bitarray_p->words[n] = update_func(bitarray_p->words[n], index, m);
    return 0;
}

int ba_update_at(Bitarray *bitarray_p, size_t index, ba_update_at_f update_func) {
    size_t n = index >> 3; // index / 8
    if (n >= bitarray_p->num_of_words) {
        return -1;
    }
    size_t m = ~index & 0b111; // 7 - (index % 8)
    int prev_value = (bitarray_p->words[n] >> m) & 1;
    bitarray_p->words[n] &= ~(1 << m);
    bitarray_p->words[n] |= update_func(prev_value, index) << m;
    return 0;
}

void ba_print(const Bitarray *bitarray_p) {
    /*
       ~sp              nl
        | 1 | 2 | 3 | 4 |
        | 5 | 6 | 7 | 8 |
        ...
    */
    size_t i;
    for (i = 0; i < bitarray_p->num_of_words; i++) {
        if (((i + 1) & 0b11) != 0b01) { // (i + 1) % 4 != 1
            putchar(' ');
        }
        putchar('0' + ((bitarray_p->words[i] >> 7) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 6) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 5) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 4) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 3) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 2) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 1) & 1));
        putchar('0' + ((bitarray_p->words[i] >> 0) & 1));
        if (((i + 1) & 0b111) == 0b100 || ((i + 1) & 0b111) == 0b000) { // (i + 1) % 4 == 0
            putchar('\n');
        }
    }
    if (((i + 1) & 0b11) != 0b01) { // (i + 1) % 4 != 1
        putchar('\n');
    }
}

void ba_free(Bitarray *bitarray_p) {
    free(bitarray_p->words);
    free(bitarray_p);
}
