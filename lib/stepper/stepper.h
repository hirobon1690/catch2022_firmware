#include "gpio.h"
#include "Ticker.h"

class stepper {
   public:
    stepper(gpio&);
    void step(int step,bool dir);

   private:
    gpio pin;
};