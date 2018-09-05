#include <stm32f30x.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>

#define Buffer_Size	10

char readArray[Buffer_Size];

void WriteRingBuffer(char* string);
char *ReadRingBuff(void);
