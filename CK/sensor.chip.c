#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t VCC;
  pin_t GND;
  pin_t A0;
  pin_t D0;
  float value;
  float threshold;
} chip_data_t;

void chip_timer_callback(void *data) {
  chip_data_t *chip_data = (chip_data_t*)data;
  if (pin_read(chip_data->VCC) && !pin_read(chip_data->GND))
  {
    float value = attr_read(chip_data->value);
    float threshold = attr_read(chip_data->threshold);
    float volts = 5.0 * (value / 1024.0);
    pin_dac_write(chip_data->A0, volts);
    pin_write(chip_data->D0, value >= threshold);
  }
  else
  {
    pin_dac_write(chip_data->A0, 0);
    pin_write(chip_data->D0, 0);
  }
}

void chip_init() {
  chip_data_t *chip_data = (chip_data_t*)malloc(sizeof(chip_data_t));
  chip_data->value = attr_init("value", 500.0);
  chip_data->threshold = attr_init("threshold", 500.0);
  chip_data->VCC = pin_init("VCC", INPUT);
  chip_data->GND = pin_init("GND", INPUT);
  chip_data->A0 = pin_init("A0", ANALOG);
  chip_data->D0 = pin_init("D0", ANALOG);

  const timer_config_t config = 
  {
    .callback = chip_timer_callback,
    .user_data = chip_data,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 10000, true);
}
