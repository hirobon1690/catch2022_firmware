#include <driver/ledc.h>
#include "PinNames.h"

class servo{
    public:
    servo(PinName pin, int min, int max);
    void write(float angle);

};