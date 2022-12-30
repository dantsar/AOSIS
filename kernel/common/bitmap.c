// #include <common/bitmap.h>

// void bitmap_set(bitmap_t *bmap, size_t index, bool val)
// {
//     uint8_t *bm_index = bmap->index + ((size_t) index / 8);
//     uint8_t bit = 0b10000000 >> (index % 8);
//     if (val) { // set
//         *bm_index |= bit;
//     } else { // clear
//         *bm_index &= ~bit;
//     }
// }

// bool bitmap_get(bitmap_t *bmap, size_t index)
// {
//     uint8_t *bm_index = bmap->index + ((size_t) index / 8);
//     uint8_t bit = 0b10000000 >> (index % 8);
//     return (*bm_index & bit);
// }
