#ifndef _CGMINER_MINER_H
#define _CGMINER_MINER_H

#include <stdint.h>

struct work {
	uint8_t *midstate;
	uint8_t *data;
};

#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif
#ifndef MIN
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#endif

typedef enum {
	false = 0,
	true = 1
} bool;
#endif
