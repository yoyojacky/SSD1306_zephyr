// ============================================================================
// SSD1306_Zephyr.h - 纯 Zephyr 兼容的 SSD1306 I2C 驱动
// 不依赖 Adafruit_GFX，不依赖 portOutputRegister 等宏
// 专为 Arduino Zephyr Core 0.55.0 (STM32U585) 设计
// ============================================================================

#ifndef SSD1306_ZEPHYR_H
#define SSD1306_ZEPHYR_H

#include <Arduino.h>
#include <Wire.h>

// SSD1306 I2C 地址
#ifndef SSD1306_I2C_ADDRESS
#define SSD1306_I2C_ADDRESS 0x3C
#endif

// SSD1306 命令
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_EXTERNALVCC         0x01
#define SSD1306_SWITCHCAPVCC        0x02

// 颜色定义
#define SSD1306_BLACK               0
#define SSD1306_WHITE               1
#define SSD1306_INVERSE             2

class SSD1306_Zephyr {
public:
    SSD1306_Zephyr(uint8_t width, uint8_t height, TwoWire *wire = &Wire, 
                   int8_t rst_pin = -1, uint8_t i2c_addr = SSD1306_I2C_ADDRESS);
    
    bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2c_addr = 0);
    void display(void);
    void clearDisplay(void);
    void invertDisplay(bool i);
    void dim(bool dim);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
                      int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
                      int16_t x2, int16_t y2, uint16_t color);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setCursor(int16_t x, int16_t y);
    void println(const String &s);
    void print(const String &s);
    void print(char c);
    void print(int n);
    void print(float f, int digits = 2);
    void write(char c);
    
    uint8_t width(void) const { return _width; }
    uint8_t height(void) const { return _height; }
    bool isInitialized(void) const { return _initialized; }

private:
    TwoWire *_wire;
    uint8_t _i2c_addr;
    int8_t _rst_pin;
    uint8_t _width, _height;
    uint8_t _text_size;
    uint16_t _text_color;
    int16_t _cursor_x, _cursor_y;
    bool _initialized;
    
    // 显示缓冲区: 128x64 / 8 = 1024 bytes
    uint8_t *_buffer;
    uint16_t _buffer_size;
    
    void _sendCommand(uint8_t cmd);
    void _sendCommands(const uint8_t *cmds, uint8_t len);
    void _sendData(uint8_t *data, uint16_t len);
    void _drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint8_t size);
    
    // 辅助函数声明
    void _swap(int16_t &a, int16_t &b) { int16_t t = a; a = b; b = t; }
    int16_t _abs(int16_t x) { return x < 0 ? -x : x; }
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                          uint8_t corners, int16_t delta, uint16_t color);
};

// 5x7 字体数据 (ASCII 32-127)
extern const uint8_t font5x7[];

#endif // SSD1306_ZEPHYR_H
