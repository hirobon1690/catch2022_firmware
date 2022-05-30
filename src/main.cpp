#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp32/rom/ets_sys.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" {
void app_main(void);
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_RATE_MS);
}

int i2c_master_port = 0;
void i2c_init() {
    i2c_config_t conf;
    const int I2C_MASTER_SCL_IO = 22;        /*!< GPIO number used for I2C master clock */
    const int I2C_MASTER_SDA_IO = 21;        /*!< GPIO number used for I2C master data  */
    const int I2C_MASTER_NUM = 0;            /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
    const int I2C_MASTER_FREQ_HZ = 400000;   /*!< I2C master clock frequency */
    const int I2C_MASTER_TX_BUF_DISABLE = 0; /*!< I2C master doesn't need buffer */
    const int I2C_MASTER_RX_BUF_DISABLE = 0; /*!< I2C master doesn't need buffer */
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;  // select GPIO specific to your project
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;  // select GPIO specific to your project
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;  // select frequency specific to your project

    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void write() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x20 << 1 | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    uint8_t data1[] = {0b00000110, 0b00000000, 0b00000000};
    i2c_master_write_byte(cmd, 0b00000110, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0b00000000, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0b00000000, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_master_port, cmd, 1 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

void led() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x20 << 1 | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    uint8_t data2[] = {0b00000010, 0xFF, 0xFF};
    i2c_master_write_byte(cmd, 0b00000010, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0xFF, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0xFF, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_master_port, cmd, 1 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

void app_main() {
    i2c_init();
    write();
    while (1) {
        led();
        printf("%d\n", 1);
        delay_ms(500);
    }
}
