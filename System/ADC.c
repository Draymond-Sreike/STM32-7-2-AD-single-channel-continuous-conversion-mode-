#include "stm32f10x.h"                  // Device header

void ADC_Init_t()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIOA_InitStructure;
	//配置IO口为模拟输入模式,该模式是ADC的专属模式
	//该模式下IO口是无效的，防止IO口的输入输出对模拟电压造成干扰
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	//以上代码配置完，IO口就被配置为模拟输入的引脚了
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
	//配置ADC时钟为RCC的6分频即12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//选择规则组的输入通道
		//选择通道0，排序第1，采用时间为55.5个ADCCLK周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		//若还要写入其他通道进序列，如下即可
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	//本次程序的代码用的是连续转换，非扫描模式
	//连续转换模式配置为连续转换
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//扫描模式配置，只有一个通道，不需扫描
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//通道数目配置，指定在【扫描模式】下总共需要扫描多少个通道（非扫描模式下，写任何值都只有序列1有效而已）
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	//数据位对齐，选择右对齐，即读取的12位数据在16位寄存器中占后12位,这种情况下转换出来的就是实际读取值
		//若选择左对齐，则相当于在右对齐的基础上左移4位，占寄存器的高12位
		//此时读取的数据是实际数值的2的4次方倍，即8倍
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//外部触发源选择，即选择触发转换的源头，本程序我们用软件触发，不需外部触发，填ADC_ExternalTrigConv_None
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//配置ADC为独立工作模式，即ADC1和ADC2各转换各自的，该参数的其他值则是双ADC模式配置相关的
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_Init(ADC1, &ADC_InitStructure);
	//至此ADC配置就完成了
	
	//若需中断和看门口则可以在下面继续配置
	
	//开启ADC电源
	ADC_Cmd(ADC1, ENABLE);
	
	//开启电源后，根据手册的建议，我们还需对ADC进行校准
	//复位校准函数，开始复位校准
	ADC_ResetCalibration(ADC1);
	//复位校准完成标志函数（完成校准返回0，未完成校准时为1）
		//如果没有校准完成则在循环中一直等待直到校准完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	//开始校准函数，启动校准
	ADC_StartCalibration(ADC1);
	//等待校准完成函数
		//while循环原理同上个while
	while(ADC_GetCalibrationStatus(ADC1));
	
	//软件触发转换器,此时只需触发一次，ADC便开始连续转换，故放在Init函数中
	//当函数调用完成后就开始AD的转换了
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//调用了这个函数即读取ADC数据寄存器的转换值
uint16_t ADC_GetValue()
{
	return ADC_GetConversionValue(ADC1);
}
