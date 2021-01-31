

/*
	-Input data: 8  bytes (64 bits)  left(4 bytes) <---> right ( 4 bytes ) 
	-Key :		 16 bytes (128 bits) 4 (long)
*/

#include <stdint.h>
 
void encrypt (uint32_t* v, uint32_t* k);
void decrypt (uint32_t* v, uint32_t* k);
void tea_decrypt(uint8_t * data, uint32_t * len, uint32_t * key);
void tea_encrypt(uint8_t * data, uint32_t * len, uint32_t * key);