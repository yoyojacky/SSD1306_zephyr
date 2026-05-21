# SSD1306_zephyr
It's a library for Arduino UNO Q to drive OLED display(128x64pixels). 
因为zephyr更新到0.55.0 后，原先可用的adafruit_ssd1306 的库不可用了。出现的问题可以查看看这个issue： [ https://github.com/adafruit/Adafruit_SSD1306/issues/301 ]
所以决定直接用zephyr提供的i2c的api直接驱动OLED屏幕，基本上是vibe coding出来的，不过实测好用。有需要的可以随意拿去用。

## 使用方法

sketch/
├── SSD1306_Zephyr.h      ← 添加这个头文件
├── SSD1306_Zephyr.cpp    ← 还有这个CPP文件
├── your_main.ino         ← 使用 #include "SSD1306_Zephyr.h"

## 添加初始化的操作：
```cpp
#include "SSD1306_Zephyr.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1   
#define OLED_I2C_ADDRESS 0x3C  
SSD1306_Zephyr display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, OLED_I2C_ADDRESS);
```

## 可调用的函数

以下是 `SSD1306_Zephyr` 驱动库的所有可调函数完整说明，按功能分类整理：

---

## 一、初始化与显示控制

### `begin()`
**功能**：初始化 SSD1306 OLED 屏幕
```cpp
bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2c_addr = 0);
```
| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `switchvcc` | `uint8_t` | `SSD1306_SWITCHCAPVCC` | 供电模式：`SSD1306_SWITCHCAPVCC` (0x02) 使用内部电荷泵升压；`SSD1306_EXTERNALVCC` (0x01) 外部供电 |
| `i2c_addr` | `uint8_t` | `0` (使用构造函数传入的地址) | I2C 设备地址，0x3C 或 0x3D |

**返回值**：`true` 初始化成功，`false` 失败（内存分配失败等）

**示例**：
```cpp
SSD1306_Zephyr display(128, 64, &Wire, -1, 0x3C);
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!");
}
```

---

### `display()`
**功能**：将内存缓冲区内容刷新到 OLED 屏幕（必须调用才能显示）
```cpp
void display(void);
```
**参数**：无

**示例**：
```cpp
display.clearDisplay();
display.setCursor(0, 0);
display.print("Hello");
display.display();  // 必须调用，否则屏幕不更新
```

---

### `clearDisplay()`
**功能**：清空显示缓冲区（变为全黑），**不自动刷新到屏幕**
```cpp
void clearDisplay(void);
```
**参数**：无

**注意**：调用后需再调用 `display()` 才能在屏幕上看到效果

---

### `invertDisplay()`
**功能**：反转屏幕显示颜色（黑变白，白变黑）
```cpp
void invertDisplay(bool i);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `i` | `bool` | `true` 反转显示，`false` 恢复正常 |

---

### `dim()`
**功能**：调整屏幕亮度（对比度）
```cpp
void dim(bool dim);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `dim` | `bool` | `true` 暗屏（低对比度），`false` 正常亮度 |

---

## 二、状态查询

### `width()` / `height()`
**功能**：获取屏幕尺寸
```cpp
uint8_t width(void) const;
uint8_t height(void) const;
```
**返回值**：屏幕宽度/高度（像素）

---

### `isInitialized()`
**功能**：检查是否初始化成功
```cpp
bool isInitialized(void) const;
```
**返回值**：`true` 已初始化

---

## 三、基础绘图函数

### `drawPixel()`
**功能**：绘制单个像素点
```cpp
void drawPixel(int16_t x, int16_t y, uint16_t color);
```
| 参数 | 类型 | 范围 | 说明 |
|------|------|------|------|
| `x` | `int16_t` | 0 ~ width-1 | X 坐标 |
| `y` | `int16_t` | 0 ~ height-1 | Y 坐标 |
| `color` | `uint16_t` | `SSD1306_BLACK` (0), `SSD1306_WHITE` (1), `SSD1306_INVERSE` (2) | 颜色 |

---

### `drawLine()`
**功能**：绘制直线（Bresenham 算法）
```cpp
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `x0, y0` | `int16_t` | 起点坐标 |
| `x1, y1` | `int16_t` | 终点坐标 |
| `color` | `uint16_t` | 颜色 |

**示例**：
```cpp
display.drawLine(0, 0, 127, 63, SSD1306_WHITE);  // 对角线
```

---

### `drawFastHLine()`
**功能**：快速绘制水平线（比 drawLine 更高效）
```cpp
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `x, y` | `int16_t` | 起点坐标 |
| `w` | `int16_t` | 线宽度（像素） |
| `color` | `uint16_t` | 颜色 |

---

### `drawFastVLine()`
**功能**：快速绘制垂直线
```cpp
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `x, y` | `int16_t` | 起点坐标 |
| `h` | `int16_t` | 线高度（像素） |
| `color` | `uint16_t` | 颜色 |

---

## 四、矩形与填充

### `drawRect()`
**功能**：绘制空心矩形
```cpp
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `x, y` | `int16_t` | 左上角坐标 |
| `w` | `int16_t` | 宽度 |
| `h` | `int16_t` | 高度 |
| `color` | `uint16_t` | 边框颜色 |

---

### `fillRect()`
**功能**：绘制实心矩形
```cpp
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
```
**参数**：同上

**示例**：
```cpp
display.fillRect(10, 10, 50, 20, SSD1306_WHITE);  // 白色实心矩形
display.fillRect(70, 10, 50, 20, SSD1306_BLACK);  // 黑色实心（擦除）
```

---

## 五、圆形

### `drawCircle()`
**功能**：绘制空心圆
```cpp
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `x0, y0` | `int16_t` | 圆心坐标 |
| `r` | `int16_t` | 半径 |
| `color` | `uint16_t` | 颜色 |

---

### `fillCircle()`
**功能**：绘制实心圆
```cpp
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
```
**参数**：同上

**示例**：
```cpp
display.drawCircle(64, 32, 20, SSD1306_WHITE);   // 空心圆
display.fillCircle(64, 32, 10, SSD1306_WHITE);   // 实心圆
```

---

## 六、三角形

### `drawTriangle()`
**功能**：绘制空心三角形
```cpp
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `x0, y0` | `int16_t` | 顶点1 |
| `x1, y1` | `int16_t` | 顶点2 |
| `x2, y2` | `int16_t` | 顶点3 |
| `color` | `uint16_t` | 颜色 |

---

### `fillTriangle()`
**功能**：绘制实心三角形
```cpp
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color);
```

**示例**：
```cpp
display.fillTriangle(64, 10, 40, 50, 88, 50, SSD1306_WHITE);  // 实心三角形
```

---

## 七、文本显示

### `setTextSize()`
**功能**：设置文本大小
```cpp
void setTextSize(uint8_t s);
```
| 参数 | 类型 | 范围 | 说明 |
|------|------|------|------|
| `s` | `uint8_t` | ≥1 | 缩放倍数：1=5x7像素，2=10x14像素，3=15x21像素 |

---

### `setTextColor()`
**功能**：设置文本颜色
```cpp
void setTextColor(uint16_t c);
```
| 参数 | 类型 | 说明 |
|------|------|------|
| `c` | `uint16_t` | `SSD1306_WHITE` 或 `SSD1306_BLACK` |

---

### `setCursor()`
**功能**：设置文本光标位置
```cpp
void setCursor(int16_t x, int16_t y);
```
| 参数 | 类型 | 范围 | 说明 |
|------|------|------|------|
| `x` | `int16_t` | 0~127 | 光标 X 坐标 |
| `y` | `int16_t` | 0~63 | 光标 Y 坐标 |

---

### `print()` / `println()`
**功能**：输出文本（支持多种数据类型）
```cpp
void print(const String &s);      // 字符串
void print(char c);               // 单个字符
void print(int n);                // 整数
void print(float f, int digits = 2);  // 浮点数，digits=小数位数
void println(const String &s);    // 输出字符串并换行
```

**示例**：
```cpp
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 0);
display.println("Temp: 25.5C");
display.print("ADC: ");
display.print(512);
display.display();
```

---

### `write()`
**功能**：输出单个字符（`print()` 的底层实现）
```cpp
void write(char c);
```
**特殊字符**：
- `'\n'`：换行（光标移到下一行开头）
- `'\r'`：忽略

---

## 八、颜色常量

| 常量 | 值 | 说明 |
|------|-----|------|
| `SSD1306_BLACK` | 0 | 黑色（熄灭） |
| `SSD1306_WHITE` | 1 | 白色（点亮） |
| `SSD1306_INVERSE` | 2 | 反色（与现有像素异或） |

---

## 九、完整使用示例

```cpp
#include "SSD1306_Zephyr.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1   
#define OLED_I2C_ADDRESS 0x3C  

SSD1306_Zephyr display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, OLED_I2C_ADDRESS);

void setup() {
    Serial.begin(115200);
    
    // 初始化 OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        Serial.println("OLED init failed!");
        return;
    }
    
    // 清屏
    display.clearDisplay();
    
    // 显示文本
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Hello World!");
    display.print("Temp: ");
    display.print(25.5, 1);
    display.println(" C");
    
    // 绘制图形
    display.drawRect(0, 40, 60, 20, SSD1306_WHITE);      // 空心矩形
    display.fillRect(70, 40, 50, 20, SSD1306_WHITE);     // 实心矩形
    display.drawCircle(100, 20, 10, SSD1306_WHITE);        // 圆
    display.drawLine(0, 35, 127, 35, SSD1306_WHITE);       // 分割线
    
    // 刷新到屏幕
    display.display();
}

void loop() {
    // 动态更新示例
    static int counter = 0;
    
    display.fillRect(0, 20, 40, 10, SSD1306_BLACK);  // 擦除旧数字区域
    display.setCursor(0, 20);
    display.print(counter++);
    display.display();
    
    delay(1000);
}
```

---

## 十、与 Adafruit_SSD1306 API 对比

| Adafruit 函数 | SSD1306_Zephyr 对应 | 兼容性 |
|---------------|---------------------|--------|
| `display.begin()` | `display.begin()` | ✅ 相同 |
| `display.display()` | `display.display()` | ✅ 相同 |
| `display.clearDisplay()` | `display.clearDisplay()` | ✅ 相同 |
| `display.setTextSize()` | `display.setTextSize()` | ✅ 相同 |
| `display.setTextColor()` | `display.setTextColor()` | ✅ 相同 |
| `display.setCursor()` | `display.setCursor()` | ✅ 相同 |
| `display.println()` | `display.println()` | ✅ 相同 |
| `display.drawPixel()` | `display.drawPixel()` | ✅ 相同 |
| `display.drawLine()` | `display.drawLine()` | ✅ 相同 |
| `display.drawRect()` | `display.drawRect()` | ✅ 相同 |
| `display.fillRect()` | `display.fillRect()` | ✅ 相同 |
| `display.drawCircle()` | `display.drawCircle()` | ✅ 相同 |
| `display.fillCircle()` | `display.fillCircle()` | ✅ 相同 |
| `display.drawTriangle()` | `display.drawTriangle()` | ✅ 相同 |
| `display.fillTriangle()` | `display.fillTriangle()` | ✅ 相同 |
| `display.width()` | `display.width()` | ✅ 相同 |
| `display.height()` | `display.height()` | ✅ 相同 |

**注意**：`SSD1306_Zephyr` 不包含 `Adafruit_GFX` 的复杂功能（如 `drawBitmap()`、`setRotation()` 等），如需这些功能需要额外实现，有需要的自己开发吧。

<img width="1191" height="899" alt="image" src="https://github.com/user-attachments/assets/8663360f-013b-45c3-89fb-8492157952b5" />
<img width="1066" height="753" alt="image" src="https://github.com/user-attachments/assets/baac5e0d-85a4-43f8-95f4-34c9774f0cca" />
