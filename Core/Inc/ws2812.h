/**
 * @file ws2812b.h
 * @author Auska Wang
 * @brief Header file of ws2812b.c
 * 	      This file contains
 * 	      - the functions necessary to communicate with WS2812b.
 * 	      - RGB struct to ensure intuitive data structure.
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Defines ------------------------------------------------------------------*/
#define NUM_LEDS 16

/* Structs ------------------------------------------------------------------*/
typedef struct {
	int R;
	int G;
	int B;
} RGB_Value;

/* Function prototypes ------------------------------------------------------------------*/
void intialize_led_strip(uint8_t led_count);
void send_data_color_value(uint8_t led, uint32_t color_value);
void send_data_hue(uint8_t led, uint16_t hue, float saturation, float value);
void send_data_rgb(uint8_t led, RGB_Value rgb);
void show_leds();
void show_red();

#endif /* INC_WS2812_H_ */
