#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "ADC.h"

int main()
{
	uint16_t ADC_Value;
	float Voltage;
	OLED_Init();
	ADC_Init_t();
	OLED_ShowString(1, 1, "ADC_Value:");
	OLED_ShowString(2, 1, "Voltage:0.00V");

	while(1)
	{
		ADC_Value = ADC_GetValue();
		Voltage = (ADC_Value / 4095.0) * 3.3;
		OLED_ShowNum(1, 11, ADC_Value,4);
		OLED_ShowNum(2, 9, Voltage,1);
		OLED_ShowNum(2, 11, Voltage*100, 2);
	}
}
