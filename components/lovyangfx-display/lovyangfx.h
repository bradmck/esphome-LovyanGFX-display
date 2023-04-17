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
    //static LovyanGFX lcd;    
    LGFX_Sprite canvas;
    lgfx lcd;
      
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


};


}  // namespace tdisplays3
}  // namespace esphome

#endif
