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

    LGFX(void)
    {
        {
        auto cfg = _bus_instance.config();
        cfg.spi_mode   = 0;
        cfg.freq_write = SPI_FREQ_W;
        cfg.freq_read  = SPI_FREQ_R;
        cfg.pin_sclk   = TFT_SCK;
        cfg.pin_mosi   = TFT_MOSI;
        cfg.pin_miso   = TFT_MISO;
        cfg.pin_dc     = TFT_DC;
        //cfg.spi_host   = -1;
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
        }
        {
        auto cfg = _panel_instance.config();
        cfg.pin_cs           = TFT_CS;
        cfg.pin_rst          = TFT_RST;
        cfg.pin_busy         =    -1;
        cfg.memory_width     =   132;
        cfg.memory_height    =   132; // 162 or 160 or 132
        cfg.panel_width      =   TFT_WIDTH;
        cfg.panel_height     =   TFT_HEIGHT;
        cfg.offset_x         =     2;
        cfg.offset_y         =     2;
        cfg.offset_rotation  =     0;
        cfg.dummy_read_pixel =     8;
        cfg.dummy_read_bits  =     1;
        cfg.readable         = true;
        cfg.invert           = false;
        cfg.rgb_order        = false;
        cfg.dlen_16bit       = false;
        cfg.bus_shared       = true;
        _panel_instance.config(cfg);
        }
        setPanel(&_panel_instance);
    }


    static LGFX lcd;


    void setup() override {
        lcd.init();
        //lcd.setRotation(3);
        lcd.setBrightness(1);
        lcd.fillScreen(TFT_BLACK);
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
        spr.drawPixel(x, y, display::ColorUtil::color_to_565(color));
    }

    /////////////
    // PollingComponent Methods
    /////////////
    void update() override {
        this->do_update_();
        spr.pushSprite(0, 0);
    }

    void dump_config() {
        ESP_LOGCONFIG(TAG, "LovyanGFX:");
           
        //ESP_LOGCONFIG(TAG, "  TFT_eSPI Library Version: %s", tftSetup.version.c_str());
        ESP_LOGCONFIG(TAG, "  TFT Width: %d", lcd.width());
        ESP_LOGCONFIG(TAG, "  TFT Height: %d", lcd.height());


    }    
};


}  // namespace lovyangfx
}  // namespace esphome

#endif
