/*
 * ws2812.c
 *
 *  Created on: Mar 18, 2025
 *      Author: auska
 */
#include "ws2812.h"
#include "main.h"
#define RIGHTMOST_BIT_MASK 0x1U
#define RGB_DATA_SIZE 24
extern TIM_HandleTypeDef htim3;

uint32_t pwm_data[RGB_DATA_SIZE * NUM_LEDS + 1];
uint8_t dma_in_progress = 0;

/**
 * @brief Sends data to light the specified LED using HSV.
 *
 * @param led Value from 0 to NUM_LEDS - 1
 * @param hue Hue value 0 to 255
 * @param saturation Saturation value 0 to 1.0
 * @param value Value value 0 to 1.0
 *
 * @retval None
 */
void send_data_hue(uint8_t led, uint16_t hue, float saturation, float value) {
	if (NUM_LEDS - 1 < led) {
		Error_Handler();
	}

	float max_RGB = value;
	float rgb_range = saturation * value;
	float min_RGB = max_RGB - rgb_range;

	RGB_Value rgb = {0,0,0};

	if (hue >= 0 && hue < 60) {
	    rgb.R = 255 * max_RGB;
	    rgb.G = ((max_RGB - min_RGB) / 60 * hue + min_RGB) * 255;
	    rgb.B = min_RGB * 255;
	}
	else if (hue >= 60 && hue < 120) {
	    rgb.R = ((max_RGB - min_RGB) / 60 * (120.0f - hue) + min_RGB) * 255;
	    rgb.G = 255 * max_RGB;
	    rgb.B = min_RGB * 255;
	}
	else if (hue >= 120 && hue < 180) {
	    rgb.R = min_RGB * 255;
	    rgb.G = 255 * max_RGB;
	    rgb.B = ((max_RGB - min_RGB) / 60 * (hue - 120.0f) + min_RGB) * 255;
	}
	else if (hue >= 180 && hue < 240) {
	    rgb.R = min_RGB * 255;
	    rgb.G = ((max_RGB - min_RGB) / 60 * (240.0f - hue) + min_RGB) * 255;
	    rgb.B = 255 * max_RGB;
	}
	else if (hue >= 240 && hue < 300) {
	    rgb.R = ((max_RGB - min_RGB) / 60 * (hue - 240.0f) + min_RGB) * 255;
	    rgb.G = min_RGB * 255;
	    rgb.B = 255 * max_RGB;
	}
	else if (hue >= 300 && hue < 360) {
	    rgb.R = 255 * max_RGB;
	    rgb.G = min_RGB * 255;
	    rgb.B = ((max_RGB - min_RGB) / 60 * (360.0f - hue) + min_RGB) * 255;
	}

	send_data_rgb(led, rgb);
}

/**
 * @brief Populates color value data to PWM array.
 *
 * @param led Value from 0 to NUM_LEDS - 1
 * @param color_value Color data 24 bits comprising of leftmost 8 bits not used, then 8 bits R value, 8 bits G value, then 8 bits B value
 *
 * @retval None
 */
void send_data_color_value(uint8_t led, uint32_t color_value)
{
	if (NUM_LEDS - 1 < led) {
			Error_Handler();
	}

	for (int i = 0; i < 24; i++) {
		char bit = color_value & 0x1;
		if (bit == 1) {
			pwm_data[23 + led * 24 - i] = 40;
		}
		else {
			pwm_data[23 + led * 24 - i] = 20;
		}
		color_value = color_value >> 1;
	}
	pwm_data[RGB_DATA_SIZE * NUM_LEDS] = 0;
}

/**
 * @brief Transfers the PWM data through DMA to TIM connected to WS2812b strip.
 *
 * @param None
 *
 * @retval None
 */
void show_leds() {
	while (dma_in_progress) {};
	dma_in_progress = 1;
	if (HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, pwm_data, sizeof(pwm_data)/sizeof(uint32_t) + 1) != HAL_OK) {
			Error_Handler();
	}
	HAL_Delay(1);
}

/**
 * @brief Sends color data in the RGB format
 *
 * @param led Value from 0 to NUM_LEDS - 1
 * @param rgb RGB struct that contains RGB value
 *
 * @retval None
 */
void send_data_rgb(uint8_t led, RGB_Value rgb)
{
	if (NUM_LEDS - 1 < led) {
			Error_Handler();
	}

	//GRB format
	uint32_t color_value = rgb.G << 16 | rgb.R << 8 | rgb.B;
	send_data_color_value(led, color_value);

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
  if (htim->hdma[TIM_DMA_ID_CC1]->State == HAL_DMA_STATE_READY)
  {
      __HAL_UNLOCK(htim->hdma[TIM_DMA_ID_CC1]);
      htim->hdma[TIM_DMA_ID_CC1]->State = HAL_DMA_STATE_READY;
  }
  dma_in_progress = 0;
}
