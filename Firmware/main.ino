/******************头文件定义************************/
#include <FastLED.h> //加载灯带头文件
/******************头文件定义************************/
#include <Arduino.h>
/******************宏定义***********************/

#define NUM_LEDS 8 * 32                            // 灯珠数量
#define LED_TYPE WS2812                            // 灯带型号
#define COLOR_ORDER GRB                            // 输入颜色的顺序 根据手册决定
CRGB leds[NUM_LEDS];                               // 构建灯珠对象
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0])) // 求数组长度 总长度/单个长度

void nextPattern(void);
void test(void);
void Arrowhead(void);
uint8_t n = 0;
/******************宏定义***********************/

/****************外部电路引脚定义**********************/

#define DATA_PIN 5 // 灯带引脚定义

/****************外部电路引脚定义**********************/
/*********************任务句柄等定义************************/

TaskHandle_t LED_Driver = NULL;
TaskHandle_t LED_Control = NULL;
TaskHandle_t HIT_Dectect = NULL;

/*********************变量数组等定义************************/

CHSV myHSV(0, 255, 200); // 定义色板 色调，饱和度，亮度

uint8_t gCurrentPatternNumber = 0; // 函数数组下标

uint8_t gHue = 0; // 颜色转换标志位CHSV myHSV(0,255,200);//色调，饱和度，亮度 All_colors所用

uint8_t max_bright = 200; // 亮度

/*********************变量数组等定义************************/

/*****************函数数组定义*********************/

typedef void (*SimplePatternList[])(); // 创建函数数组指针

// SimplePatternList gPatterns = {All_colors, Run_light, rainbow_run, confetti, juggle};
SimplePatternList gPatterns = {Arrowhead};
// 循环遍历的模式列表。每个模式定义为下面的一个单独的函数。

/*****************函数数组定义*********************/

void nextPattern(void) // 进入下一个函数
{
  gCurrentPatternNumber = ((gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns)); // 类似数组下标++   取余是为了进行下一周期循环
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
}

/*把WS2812Init放入setup函数*/

void WS2812Init(void)
{
  vTaskDelay(1000);                                                                                     // 上电延迟
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // 初始化灯带模板 无需变动复制粘贴即可
  FastLED.setBrightness(max_bright);                                                               // 初始化亮度
}
/*把WS2812Run放入loop函数*/

void WS2812Run(void)
{
  gPatterns[gCurrentPatternNumber]();      // 首先调用第一个函数
  FastLED.show();                          // show灯带
  vTaskDelay(50);                               // 运行下一个函数之间延迟
  EVERY_N_MILLISECONDS(10) { gHue += 10; } // 10毫秒执行一次gHue++
  EVERY_N_SECONDS(8) { nextPattern(); }    // 8秒调用nextPattern();函数一次
}

void driverLED(void* arg)
{

  while (1)
  {
    vTaskDelay(1000);
    // WS2812Run();
    Serial.println("REFRESH");
  }
  vTaskDelete(NULL);
}


// void controlLED(void)
// {
//   while (1)
//   {
//     vTaskDelay();
//   }

// }
// void detectHIT(void)
// {

//   while (1)
//   {
    
//   }
  
// }

/********************主函数************************/

void setup()
{
  Serial.begin(115200);

  WS2812Init();
  xTaskCreate(driverLED, "LED_Driver", 10000, NULL, 1, &LED_Driver);
  if(LED_Driver){
    Serial.print("LED_DRIVER task is created");
  }
  vTaskStartScheduler();
  // xTaskCreate(LED_Control,"LED_Control",1024,NULL,3,LED_Control);
}

void loop()
{
  
  vTaskDelay(100);

  
}