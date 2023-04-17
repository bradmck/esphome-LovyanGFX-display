#ifndef LOVYANGFX
#define LOVYANGFX

#define LGFX_USE_V1
#include <Wire.h>
#include <SPI.h>
#include <LovyanGFX.hpp>
#include "esphome.h"

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"

namespace esphome {
namespace lovyangfx {

static const char *const TAG = "LovyanGFX";
 

class LovyanGFX :   public lgfx::LGFX_Device,
                    public PollingComponent,
                    public display::DisplayBuffer
{
    //lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Panel_ST7789 _panel_instance; 
    lgfx::Bus_Parallel8 _bus_instance; 
    lgfx::Light_PWM     _light_instance;    
    
    public:

    LGFX_Sprite canvas;
    lgfx::LGFX_Device lcd;
      
    LovyanGFX(void)
    {
        {                                      // バス制御の設定を行います。
            auto cfg = _bus_instance.config(); // バス設定用の構造体を取得します。

            // 16位设置
           // cfg.i2s_port = I2S_NUM_0;  // 使用するI2Sポートを選択 (0 or 1) (ESP32のI2S LCDモードを使用します)
            cfg.freq_write = 20000000; // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
            cfg.pin_wr = 8;           // WR を接続しているピン番号
            cfg.pin_rd = 9;           // RD を接続しているピン番号
            cfg.pin_rs = 7;           // RS(D/C)を接続しているピン番号

            cfg.pin_d0 = 39;
            cfg.pin_d1 = 40;
            cfg.pin_d2 = 41;
            cfg.pin_d3 = 42;
            cfg.pin_d4 = 45;
            cfg.pin_d5 = 46;
            cfg.pin_d6 = 47;
            cfg.pin_d7 = 48;
            

            _bus_instance.config(cfg);              // 設定値をバスに反映します。
            _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
        }

        {                                        // 表示パネル制御の設定を行います。
            auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。

            cfg.pin_cs = 6;   // CS要拉低
            cfg.pin_rst = 5;  // RST和开发板RST相连
            cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)

            // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。

            cfg.memory_width = 240;   // ドライバICがサポートしている最大の幅
            cfg.memory_height = 320;  // ドライバICがサポートしている最大の高さ
            cfg.panel_width = 170;    // 実際に表示可能な幅
            cfg.panel_height = 320;  // 実際に表示可能な高さ
            cfg.offset_x = 35;         // パネルのX方向オフセット量
            cfg.offset_y = 0;         // パネルのY方向オフセット量
            cfg.offset_rotation = 0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
            cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
            cfg.dummy_read_bits = 1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
            cfg.readable = true;      // データ読出しが可能な場合 trueに設定
            cfg.invert = true;       // パネルの明暗が反転してしまう場合 trueに設定
            cfg.rgb_order =false;    // パネルの赤と青が入れ替わってしまう場合 trueに設定
            cfg.dlen_16bit = false;    // データ長を16bit単位で送信するパネルの場合 trueに設定
            cfg.bus_shared = false;    // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance); // 使用するパネルをセットします。
    
    { // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。

      cfg.pin_bl = 38;              // バックライトが接続されているピン番号
      cfg.invert = false;           // バックライトの輝度を反転させる場合 true
      cfg.freq   = 44100;           // バックライトのPWM周波数
      cfg.pwm_channel = 7;          // 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
    }
    }

    void setup() override {
        lcd.init();
        //lcd.setRotation(3);
        lcd.setBrightness(1);
        lcd.fillScreen(TFT_RED);
        delay(500);
    }

    void loop() override {
    }

    //////////
    // DisplayBuffer methods
    //////////
    void fill(Color color) override {
        lcd.fillScreen(display::ColorUtil::color_to_565(color));
    }

    int get_width_internal() override {
        return lcd.width();
    }

    int get_height_internal() override {
	    return lcd.height();
    }

    display::DisplayType get_display_type() override {
        return display::DisplayType::DISPLAY_TYPE_COLOR;
    }

    void draw_absolute_pixel_internal(int x, int y, Color color) override {
        lcd.drawPixel(x, y, display::ColorUtil::color_to_565(color));
    }

    /////////////
    // PollingComponent Methods
    /////////////
    void update() override {
        this->do_update_();
        //spr.pushSprite(0, 0);
    }

    void dump_config() {
        ESP_LOGCONFIG(TAG, "LovyanGFX:");
           
        //ESP_LOGCONFIG(TAG, "  TFT_eSPI Library Version: %s", tftSetup.version.c_str());
        ESP_LOGCONFIG(TAG, "  TFT Width: %d", lcd.width());
        ESP_LOGCONFIG(TAG, "  TFT Height: %d", lcd.height());


    }
};


}  // namespace tdisplays3
}  // namespace esphome

#endif
