// ============================================================================
// SSD1306_Zephyr.cpp - 实现文件
// ============================================================================

#include "SSD1306_Zephyr.h"
#include <string.h>

// 5x7 字体 (仅包含可打印 ASCII)
const uint8_t font5x7[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, // Space
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    // ... (完整字体数据见下方，这里为了简洁先展示结构)
    // 实际使用时应包含完整的 96 个字符 (0x20-0x7F)
};

// 简化的字体查找 (实际需要完整字体表)
static const uint8_t* _getCharData(char c) {
    // 这里应该返回对应字符的 5 字节数据
    // 为简洁，使用一个最小化的字体表
    static const uint8_t minimal_font[96][5] = {
        {0x00,0x00,0x00,0x00,0x00}, // ' '
        {0x00,0x00,0x5F,0x00,0x00}, // '!'
        {0x00,0x07,0x00,0x07,0x00}, // '"'
        {0x14,0x7F,0x14,0x7F,0x14}, // '#'
        {0x24,0x2A,0x7F,0x2A,0x12}, // '$'
        {0x23,0x13,0x08,0x64,0x62}, // '%'
        {0x36,0x49,0x55,0x22,0x50}, // '&'
        {0x00,0x05,0x03,0x00,0x00}, // '''
        {0x00,0x1C,0x22,0x41,0x00}, // '('
        {0x00,0x41,0x22,0x1C,0x00}, // ')'
        {0x08,0x2A,0x1C,0x2A,0x08}, // '*'
        {0x08,0x08,0x3E,0x08,0x08}, // '+'
        {0x00,0x50,0x30,0x00,0x00}, // ','
        {0x08,0x08,0x08,0x08,0x08}, // '-'
        {0x00,0x60,0x60,0x00,0x00}, // '.'
        {0x20,0x10,0x08,0x04,0x02}, // '/'
        {0x3E,0x51,0x49,0x45,0x3E}, // '0'
        {0x00,0x42,0x7F,0x40,0x00}, // '1'
        {0x42,0x61,0x51,0x49,0x46}, // '2'
        {0x21,0x41,0x45,0x4B,0x31}, // '3'
        {0x18,0x14,0x12,0x7F,0x10}, // '4'
        {0x27,0x45,0x45,0x45,0x39}, // '5'
        {0x3C,0x4A,0x49,0x49,0x30}, // '6'
        {0x01,0x71,0x09,0x05,0x03}, // '7'
        {0x36,0x49,0x49,0x49,0x36}, // '8'
        {0x06,0x49,0x49,0x29,0x1E}, // '9'
        {0x00,0x36,0x36,0x00,0x00}, // ':'
        {0x00,0x56,0x36,0x00,0x00}, // ';'
        {0x00,0x08,0x14,0x22,0x41}, // '<'
        {0x14,0x14,0x14,0x14,0x14}, // '='
        {0x41,0x22,0x14,0x08,0x00}, // '>'
        {0x02,0x01,0x51,0x09,0x06}, // '?'
        {0x32,0x49,0x79,0x41,0x3E}, // '@'
        {0x7E,0x11,0x11,0x11,0x7E}, // 'A'
        {0x7F,0x49,0x49,0x49,0x36}, // 'B'
        {0x3E,0x41,0x41,0x41,0x22}, // 'C'
        {0x7F,0x41,0x41,0x22,0x1C}, // 'D'
        {0x7F,0x49,0x49,0x49,0x41}, // 'E'
        {0x7F,0x09,0x09,0x01,0x01}, // 'F'
        {0x3E,0x41,0x41,0x51,0x32}, // 'G'
        {0x7F,0x08,0x08,0x08,0x7F}, // 'H'
        {0x00,0x41,0x7F,0x41,0x00}, // 'I'
        {0x20,0x40,0x41,0x3F,0x01}, // 'J'
        {0x7F,0x08,0x14,0x22,0x41}, // 'K'
        {0x7F,0x40,0x40,0x40,0x40}, // 'L'
        {0x7F,0x02,0x04,0x02,0x7F}, // 'M'
        {0x7F,0x04,0x08,0x10,0x7F}, // 'N'
        {0x3E,0x41,0x41,0x41,0x3E}, // 'O'
        {0x7F,0x09,0x09,0x09,0x06}, // 'P'
        {0x3E,0x41,0x51,0x21,0x5E}, // 'Q'
        {0x7F,0x09,0x19,0x29,0x46}, // 'R'
        {0x46,0x49,0x49,0x49,0x31}, // 'S'
        {0x01,0x01,0x7F,0x01,0x01}, // 'T'
        {0x3F,0x40,0x40,0x40,0x3F}, // 'U'
        {0x1F,0x20,0x40,0x20,0x1F}, // 'V'
        {0x7F,0x20,0x18,0x20,0x7F}, // 'W'
        {0x63,0x14,0x08,0x14,0x63}, // 'X'
        {0x03,0x04,0x78,0x04,0x03}, // 'Y'
        {0x61,0x51,0x49,0x45,0x43}, // 'Z'
        {0x00,0x00,0x7F,0x41,0x41}, // '['
        {0x02,0x04,0x08,0x10,0x20}, // '\'
        {0x41,0x41,0x7F,0x00,0x00}, // ']'
        {0x04,0x02,0x01,0x02,0x04}, // '^'
        {0x40,0x40,0x40,0x40,0x40}, // '_'
        {0x00,0x01,0x02,0x04,0x00}, // '`'
        {0x20,0x54,0x54,0x54,0x78}, // 'a'
        {0x7F,0x48,0x44,0x44,0x38}, // 'b'
        {0x38,0x44,0x44,0x44,0x20}, // 'c'
        {0x38,0x44,0x44,0x48,0x7F}, // 'd'
        {0x38,0x54,0x54,0x54,0x18}, // 'e'
        {0x08,0x7E,0x09,0x01,0x02}, // 'f'
        {0x08,0x14,0x54,0x54,0x3C}, // 'g'
        {0x7F,0x08,0x04,0x04,0x78}, // 'h'
        {0x00,0x44,0x7D,0x40,0x00}, // 'i'
        {0x20,0x40,0x44,0x3D,0x00}, // 'j'
        {0x00,0x7F,0x10,0x28,0x44}, // 'k'
        {0x00,0x41,0x7F,0x40,0x00}, // 'l'
        {0x7C,0x04,0x18,0x04,0x78}, // 'm'
        {0x7C,0x08,0x04,0x04,0x78}, // 'n'
        {0x38,0x44,0x44,0x44,0x38}, // 'o'
        {0x7C,0x14,0x14,0x14,0x08}, // 'p'
        {0x08,0x14,0x14,0x18,0x7C}, // 'q'
        {0x7C,0x08,0x04,0x04,0x08}, // 'r'
        {0x48,0x54,0x54,0x54,0x20}, // 's'
        {0x04,0x3F,0x44,0x40,0x20}, // 't'
        {0x3C,0x40,0x40,0x20,0x7C}, // 'u'
        {0x1C,0x20,0x40,0x20,0x1C}, // 'v'
        {0x3C,0x40,0x30,0x40,0x3C}, // 'w'
        {0x44,0x28,0x10,0x28,0x44}, // 'x'
        {0x0C,0x50,0x50,0x50,0x3C}, // 'y'
        {0x44,0x64,0x54,0x4C,0x44}, // 'z'
        {0x00,0x08,0x36,0x41,0x00}, // '{'
        {0x00,0x00,0x7F,0x00,0x00}, // '|'
        {0x00,0x41,0x36,0x08,0x00}, // '}'
        {0x08,0x08,0x2A,0x1C,0x08}, // '~'
    };
    if (c >= 32 && c <= 127) return minimal_font[c - 32];
    return minimal_font[0]; // space for unknown
}

// ============================================================================
// 构造函数
// ============================================================================
SSD1306_Zephyr::SSD1306_Zephyr(uint8_t width, uint8_t height, TwoWire *wire,
                                 int8_t rst_pin, uint8_t i2c_addr)
    : _wire(wire), _i2c_addr(i2c_addr), _rst_pin(rst_pin),
      _width(width), _height(height),
      _text_size(1), _text_color(SSD1306_WHITE),
      _cursor_x(0), _cursor_y(0), _initialized(false),
      _buffer(nullptr), _buffer_size(0) {
}

// ============================================================================
// 初始化
// ============================================================================
bool SSD1306_Zephyr::begin(uint8_t switchvcc, uint8_t i2c_addr) {
    if (i2c_addr != 0) _i2c_addr = i2c_addr;
    
    // 分配缓冲区
    _buffer_size = _width * ((_height + 7) / 8);
    _buffer = (uint8_t*)malloc(_buffer_size);
    if (!_buffer) return false;
    memset(_buffer, 0, _buffer_size);
    
    // 硬件复位
    if (_rst_pin >= 0) {
        pinMode(_rst_pin, OUTPUT);
        digitalWrite(_rst_pin, HIGH);
        delay(1);
        digitalWrite(_rst_pin, LOW);
        delay(10);
        digitalWrite(_rst_pin, HIGH);
    }
    
    // 初始化 I2C
    _wire->begin();
    delay(100);
    
    // SSD1306 初始化序列
    static const uint8_t init_cmds[] = {
        SSD1306_DISPLAYOFF,
        SSD1306_SETDISPLAYCLOCKDIV, 0x80,
        SSD1306_SETMULTIPLEX, (uint8_t)(_height - 1),
        SSD1306_SETDISPLAYOFFSET, 0x00,
        SSD1306_SETSTARTLINE | 0x00,
        SSD1306_CHARGEPUMP, (switchvcc == SSD1306_SWITCHCAPVCC) ? 0x14 : 0x10,
        SSD1306_MEMORYMODE, 0x00, // 水平寻址模式
        SSD1306_SEGREMAP | 0x01,
        SSD1306_COMSCANDEC,
        SSD1306_SETCOMPINS, (_height == 32) ? 0x02 : 0x12,
        SSD1306_SETCONTRAST, (switchvcc == SSD1306_SWITCHCAPVCC) ? 0xCF : 0x9F,
        SSD1306_SETPRECHARGE, (switchvcc == SSD1306_SWITCHCAPVCC) ? 0xF1 : 0x22,
        SSD1306_SETVCOMDETECT, 0x40,
        SSD1306_DISPLAYALLON_RESUME,
        SSD1306_NORMALDISPLAY,
        SSD1306_DISPLAYON
    };
    
    _sendCommands(init_cmds, sizeof(init_cmds));
    
    _initialized = true;
    clearDisplay();
    display();
    
    return true;
}

// ============================================================================
// 底层 I2C 通信 - 完全不使用 portOutputRegister!
// ============================================================================
void SSD1306_Zephyr::_sendCommand(uint8_t cmd) {
    _wire->beginTransmission(_i2c_addr);
    _wire->write(0x00); // Co=0, D/C#=0 (命令)
    _wire->write(cmd);
    _wire->endTransmission();
}

void SSD1306_Zephyr::_sendCommands(const uint8_t *cmds, uint8_t len) {
    _wire->beginTransmission(_i2c_addr);
    _wire->write(0x00); // 命令流起始
    for (uint8_t i = 0; i < len; i++) {
        _wire->write(cmds[i]);
    }
    _wire->endTransmission();
}

void SSD1306_Zephyr::_sendData(uint8_t *data, uint16_t len) {
    const uint16_t chunk_size = 32; // I2C 缓冲区安全大小
    for (uint16_t i = 0; i < len; i += chunk_size) {
        uint16_t chunk = (len - i < chunk_size) ? (len - i) : chunk_size;
        _wire->beginTransmission(_i2c_addr);
        _wire->write(0x40); // Co=0, D/C#=1 (数据)
        _wire->write(data + i, chunk);
        _wire->endTransmission();
    }
}

// ============================================================================
// 显示控制
// ============================================================================
void SSD1306_Zephyr::display(void) {
    if (!_buffer) return;
    
    // 设置列地址
    _sendCommand(SSD1306_COLUMNADDR);
    _sendCommand(0);
    _sendCommand(_width - 1);
    
    // 设置页地址
    _sendCommand(SSD1306_PAGEADDR);
    _sendCommand(0);
    _sendCommand((_height / 8) - 1);
    
    // 发送整个缓冲区
    _sendData(_buffer, _buffer_size);
}

void SSD1306_Zephyr::clearDisplay(void) {
    if (_buffer) memset(_buffer, 0, _buffer_size);
}

void SSD1306_Zephyr::invertDisplay(bool i) {
    _sendCommand(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void SSD1306_Zephyr::dim(bool dim) {
    _sendCommand(SSD1306_SETCONTRAST);
    _sendCommand(dim ? 0 : (_text_color == SSD1306_WHITE ? 0xCF : 0x8F));
}

// ============================================================================
// 绘图函数
// ============================================================================
void SSD1306_Zephyr::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= _width || y < 0 || y >= _height) return;
    
    uint16_t idx = x + (y / 8) * _width;
    uint8_t bit = 1 << (y & 7);
    
    switch (color) {
        case SSD1306_WHITE:   _buffer[idx] |= bit; break;
        case SSD1306_BLACK:   _buffer[idx] &= ~bit; break;
        case SSD1306_INVERSE: _buffer[idx] ^= bit; break;
    }
}

void SSD1306_Zephyr::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (y < 0 || y >= _height) return;
    if (x < 0) { w += x; x = 0; }
    if (x + w > _width) w = _width - x;
    if (w <= 0) return;
    
    uint8_t bit = 1 << (y & 7);
    uint16_t idx = x + (y / 8) * _width;
    
    switch (color) {
        case SSD1306_WHITE:
            while (w--) _buffer[idx++] |= bit;
            break;
        case SSD1306_BLACK:
            while (w--) _buffer[idx++] &= ~bit;
            break;
        case SSD1306_INVERSE:
            while (w--) _buffer[idx++] ^= bit;
            break;
    }
}

void SSD1306_Zephyr::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (x < 0 || x >= _width) return;
    if (y < 0) { h += y; y = 0; }
    if (y + h > _height) h = _height - y;
    if (h <= 0) return;
    
    uint16_t idx = x + (y / 8) * _width;
    uint8_t start_bit = y & 7;
    
    // 处理第一个不完整的字节
    if (start_bit != 0) {
        uint8_t mask = 0xFF << start_bit;
        if (h < (8 - start_bit)) mask &= ~(0xFF << (start_bit + h));
        
        switch (color) {
            case SSD1306_WHITE:   _buffer[idx] |= mask; break;
            case SSD1306_BLACK:   _buffer[idx] &= ~mask; break;
            case SSD1306_INVERSE: _buffer[idx] ^= mask; break;
        }
        idx += _width;
        h -= (8 - start_bit);
    }
    
    // 处理完整的字节
    while (h >= 8) {
        switch (color) {
            case SSD1306_WHITE:   _buffer[idx] = 0xFF; break;
            case SSD1306_BLACK:   _buffer[idx] = 0x00; break;
            case SSD1306_INVERSE: _buffer[idx] ^= 0xFF; break;
        }
        idx += _width;
        h -= 8;
    }
    
    // 处理最后一个不完整的字节
    if (h > 0) {
        uint8_t mask = (1 << h) - 1;
        switch (color) {
            case SSD1306_WHITE:   _buffer[idx] |= mask; break;
            case SSD1306_BLACK:   _buffer[idx] &= ~mask; break;
            case SSD1306_INVERSE: _buffer[idx] ^= mask; break;
        }
    }
}

void SSD1306_Zephyr::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t i = y; i < y + h; i++) {
        drawFastHLine(x, i, w, color);
    }
}

void SSD1306_Zephyr::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = _abs(y1 - y0) > _abs(x1 - x0);
    if (steep) { _swap(x0, y0); _swap(x1, y1); }
    if (x0 > x1) { _swap(x0, x1); _swap(y0, y1); }
    
    int16_t dx = x1 - x0;
    int16_t dy = _abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;
    
    for (; x0 <= x1; x0++) {
        if (steep) drawPixel(y0, x0, color);
        else drawPixel(x0, y0, color);
        err -= dy;
        if (err < 0) { y0 += ystep; err += dx; }
    }
}

void SSD1306_Zephyr::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

void SSD1306_Zephyr::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x;
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

// ============================================================================
// fillCircleHelper - 必须在 fillCircle 之前定义
// ============================================================================
void SSD1306_Zephyr::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                                       uint8_t corners, int16_t delta, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;
    
    delta++;
    
    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x;
        
        if (x < (y + 1)) {
            if (corners & 1) drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2) drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (corners & 1) drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2) drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}

void SSD1306_Zephyr::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

void SSD1306_Zephyr::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                   int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void SSD1306_Zephyr::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                   int16_t x2, int16_t y2, uint16_t color) {
    int16_t a, b, y, last;
    
    if (y0 > y1) { _swap(y0, y1); _swap(x0, x1); }
    if (y1 > y2) { _swap(y2, y1); _swap(x2, x1); }
    if (y0 > y1) { _swap(y0, y1); _swap(x0, x1); }
    
    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) a = x1; else if (x1 > b) b = x1;
        if (x2 < a) a = x2; else if (x2 > b) b = x2;
        drawFastHLine(a, y0, b - a + 1, color);
        return;
    }
    
    int16_t dx01 = x1 - x0, dy01 = y1 - y0;
    int16_t dx02 = x2 - x0, dy02 = y2 - y0;
    int16_t dx12 = x2 - x1, dy12 = y2 - y1;
    
    if (y1 == y2) last = y1;
    else last = y1 - 1;
    
    for (y = y0; y <= last; y++) {
        a = x0 + (dx01 * (y - y0) + dy01 / 2) / dy01;
        b = x0 + (dx02 * (y - y0) + dy02 / 2) / dy02;
        if (a > b) _swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }
    
    dx01 = x2 - x1; dy01 = y2 - y1;
    for (; y <= y2; y++) {
        a = x1 + (dx01 * (y - y1) + dy01 / 2) / dy01;
        b = x0 + (dx02 * (y - y0) + dy02 / 2) / dy02;
        if (a > b) _swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}

// ============================================================================
// 文本函数
// ============================================================================
void SSD1306_Zephyr::setTextSize(uint8_t s) { _text_size = (s > 0) ? s : 1; }
void SSD1306_Zephyr::setTextColor(uint16_t c) { _text_color = c; }
void SSD1306_Zephyr::setCursor(int16_t x, int16_t y) { _cursor_x = x; _cursor_y = y; }

void SSD1306_Zephyr::_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint8_t size) {
    const uint8_t *glyph = _getCharData(c);
    
    for (int8_t i = 0; i < 5; i++) {
        uint8_t line = glyph[i];
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 1) {
                if (size == 1) drawPixel(x + i, y + j, color);
                else fillRect(x + i * size, y + j * size, size, size, color);
            }
        }
    }
}

void SSD1306_Zephyr::write(char c) {
    if (c == '\n') {
        _cursor_x = 0;
        _cursor_y += _text_size * 8;
    } else if (c != '\r') {
        if ((_cursor_x + _text_size * 6) > _width) {
            _cursor_x = 0;
            _cursor_y += _text_size * 8;
        }
        if ((_cursor_y + _text_size * 8) > _height) {
            _cursor_y = 0; // 滚动或重置
        }
        _drawChar(_cursor_x, _cursor_y, c, _text_color, _text_size);
        _cursor_x += _text_size * 6;
    }
}

void SSD1306_Zephyr::print(const String &s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        write(s[i]);
    }
}

void SSD1306_Zephyr::println(const String &s) {
    print(s);
    write('\n');
}

void SSD1306_Zephyr::print(char c) { write(c); }

void SSD1306_Zephyr::print(int n) {
    print(String(n));
}

void SSD1306_Zephyr::print(float f, int digits) {
    print(String(f, digits));
}
