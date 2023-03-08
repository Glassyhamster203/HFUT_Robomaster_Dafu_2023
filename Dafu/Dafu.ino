/******************头文件定义************************/
#include <FastLED.h> //加载灯带头文件
/******************头文件定义************************/
#include <Arduino.h>
/******************宏定义***********************/

#define NUM_LEDS 8 * 32 // 灯珠数量
#define LED_TYPE WS2812 // 灯带型号
#define COLOR_ORDER GRB // 输入颜色的顺序 根据手册决定
CRGB leds[NUM_LEDS];    // 构建灯珠对象

void nextPattern(void);
void test(void);
void Arrowhead(void);
void fillLED(void);

/******************宏定义***********************/

/****************外部电路引脚定义**********************/

#define DATA_PIN 5 // 灯带引脚定义

/****************外部电路引脚定义**********************/

/*********************变量数组等定义************************/

uint8_t max_bright = 200; // 亮度 
uint8_t n = 0;            // LED灯的首地址
uint8_t hitFlag = 0;

/*********************变量数组等定义************************/

/*****************函数数组定义*********************/
void fillLED(void)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(0, 255, 200);
  }
  FastLED.show();
}
void Arrowhead(void)
{
  uint8_t ar[8][8] = {
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 1, 0, 0, 0, 0, 1, 1},
      {1, 1, 1, 0, 0, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {0, 1, 1, 1, 1, 1, 1, 0},
      {0, 0, 1, 1, 1, 1, 0, 0},
      {0, 0, 0, 1, 1, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0}};
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        leds[k * 64 + i * 8 + j] = CHSV(0, 255, 200 * ar[(i + n) % 8][j]);
      }
    }
  }
  n = (n + 7) % 8;
  FastLED.show();
  vTaskDelay(49); // 临时Delay函数，删
}

void WS2812Init(void)
{
  delay(1000);                                                                                     // 上电延迟
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // 初始化灯带模板 无需变动复制粘贴即可
  FastLED.setBrightness(max_bright);                                                               // 初始化亮度
}

void LEDCtrl(void *Parameter)
{
  while (1)
  {
    switch (hitFlag)
    {
    case 0:
      Arrowhead();
      break;
    case 1:
      fillLED();
      break;
    }
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}
/********************主函数************************/

void setup()
{
  WS2812Init();
  xTaskCreate(LEDCtrl,   // 任务函数
              "LEDCtrl", // 任务名字
              1024,      // 任务堆栈大小
              NULL,      // 传递给任务函数的参数
              2,         // 任务优先级
              NULL);     // 任务句柄*/
}

void loop()
{
  // Arrowhead();
  // WS2812Run();
}