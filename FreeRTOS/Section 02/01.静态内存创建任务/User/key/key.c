#include "key.h"
#include "delay.h"

//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE(); //开启GPIOE时钟

    GPIO_Initure.Pin = GPIO_PIN_0;        //PA0
    GPIO_Initure.Mode = GPIO_MODE_INPUT;  //输入
    GPIO_Initure.Pull = GPIO_PULLDOWN;    //下拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; //高速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4; //PE2,3,4
    GPIO_Initure.Mode = GPIO_MODE_INPUT;                     //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;                    //高速
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);
}

//按键处理函数,返回按键值
//mode:0,不支持连续按;1,支持连续按;
//return 0，没有任何按键按下
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up = 1; //按键松开标志
    if (mode == 1)
        key_up = 1; //支持连按
    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || WK_UP == 1))
    {
        Delay_ms(10);
        key_up = 0;
        if (KEY0 == 0)
            return KEY0_PRES;
        else if (KEY1 == 0)
            return KEY1_PRES;
        else if (KEY2 == 0)
            return KEY2_PRES;
        else if (WK_UP == 1)
            return WKUP_PRES;
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0)
        key_up = 1;
    return 0; //无按键按下
}
/*
* CLK Enable
	__HAL_RCC_GPIOA_CLK_ENABLE()
	__HAL_RCC_GPIOB_CLK_ENABLE()
	__HAL_RCC_GPIOC_CLK_ENABLE()
	__HAL_RCC_GPIOD_CLK_ENABLE()
	__HAL_RCC_GPIOE_CLK_ENABLE()
	__HAL_RCC_GPIOF_CLK_ENABLE()
	__HAL_RCC_GPIOG_CLK_ENABLE()
	__HAL_RCC_GPIOH_CLK_ENABLE()
	__HAL_RCC_GPIOI_CLK_ENABLE()
	
* GPIO_InitTypeDef.Pin
	GPIO_PIN_0		GPIO_PIN_7		GPIO_PIN_14
	GPIO_PIN_1		GPIO_PIN_8		GPIO_PIN_15
	GPIO_PIN_2		GPIO_PIN_9		GPIO_PIN_16
	GPIO_PIN_3		GPIO_PIN_10
	GPIO_PIN_4		GPIO_PIN_11
	GPIO_PIN_5		GPIO_PIN_12
	GPIO_PIN_6		GPIO_PIN_13

* GPIO_InitTypeDef.Mode
	GPIO_MODE_INPUT					// Input Mode
	GPIO_MODE_OUTPUT_PP				// Output Push Pull Mode 
	GPIO_MODE_OUTPUT_OD				// Output Open Drain Mode
	GPIO_MODE_AF_PP 				// Alternate Function Push Pull Mode
	GPIO_MODE_AF_OD 				// Alternate Function Open Drain Mode
	GPIO_MODE_ANALOG				// Analog Mode
	GPIO_MODE_IT_RISING 			// External Interrupt Mode with Rising edge trigger detection
	GPIO_MODE_IT_FALLING 			// External Interrupt Mode with Falling edge trigger detection
	GPIO_MODE_IT_RISING_FALLING		// External Interrupt Mode with Rising/Falling edge trigger detection
	GPIO_MODE_EVT_RISING			// External Event Mode with Rising edge trigger detection
	GPIO_MODE_EVT_FALLING 			// External Event Mode with Falling edge trigger detection
	GPIO_MODE_EVT_RISING_FALLING	// External Event Mode with Rising/Falling edge trigger detection
	
* GPIO_InitTypeDef.Pull
	GPIO_NOPULL        // No Pull-up or Pull-down activation  
	GPIO_PULLUP        // Pull-up activation                  
	GPIO_PULLDOWN      // Pull-down activation 
	
* GPIO_InitTypeDef.Speed
	GPIO_SPEED_FREQ_LOW         // IO works at 2 MHz, please refer to the product datasheet 
	GPIO_SPEED_FREQ_MEDIUM      // range 12,5 MHz to 50 MHz, please refer to the product datasheet 
	GPIO_SPEED_FREQ_HIGH        // range 25 MHz to 100 MHz, please refer to the product datasheet  
	GPIO_SPEED_FREQ_VERY_HIGH   // range 50 MHz to 200 MHz, please refer to the product datasheet 
	
* Initialization and de-initialization functions
	void HAL_GPIO_Init	(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
	void HAL_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
	
* IO operation functions
	GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
	void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	HAL_StatusTypeDef 	HAL_GPIO_LockPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
	void HAL_GPIO_EXTI_Callback	(uint16_t GPIO_Pin);
*/
