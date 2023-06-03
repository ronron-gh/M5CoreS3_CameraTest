#include <Arduino.h>

#include <M5Unified.h>
#include <esp_camera.h>

#define LCD_WIDTH (320)
#define LCD_HEIGHT  (240)
#define LCD_BUF_SIZE (LCD_WIDTH*LCD_HEIGHT*2)



static camera_config_t camera_config = {
    .pin_pwdn     = -1,
    .pin_reset    = -1,
    .pin_xclk     = 2,
    .pin_sscb_sda = 12,
    .pin_sscb_scl = 11,

    .pin_d7 = 47,
    .pin_d6 = 48,
    .pin_d5 = 16,
    .pin_d4 = 15,
    .pin_d3 = 42,
    .pin_d2 = 41,
    .pin_d1 = 40,
    .pin_d0 = 39,

    .pin_vsync = 46,
    .pin_href  = 38,
    .pin_pclk  = 45,

    .xclk_freq_hz = 20000000,
    .ledc_timer   = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565,
    .frame_size   = FRAMESIZE_QVGA,   // QVGA(320x240)
    .jpeg_quality = 0,
    .fb_count     = 2,
    .fb_location  = CAMERA_FB_IN_PSRAM,
    .grab_mode    = CAMERA_GRAB_WHEN_EMPTY,
};

esp_err_t camera_init(){

    //initialize the camera
    M5.In_I2C.release();
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        //Serial.println("Camera Init Failed");
        M5.Display.println("Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

esp_err_t camera_capture(){
  //acquire a frame
  M5.In_I2C.release();
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    //Serial.println("Camera Capture Failed");
    M5.Display.println("Camera Capture Failed");
    return ESP_FAIL;
  }

  //replace this with your own function
  //process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);
  M5.Display.startWrite();
  M5.Display.setAddrWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
  M5.Display.writePixels((uint16_t*)fb->buf, int(fb->len / 2));
  M5.Display.endWrite();

  //return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);
  return ESP_OK;
}

void setup() {
  auto cfg = M5.config();
  cfg.output_power = true;
  M5.begin(cfg);

  M5.Display.setFont(&fonts::efontJA_24);
  M5.Display.println("HelloWorld");

#if 0   // Display test
  uint16_t *lcdBuf;
  lcdBuf = (uint16_t*)malloc(LCD_BUF_SIZE);
  if(lcdBuf==NULL){
    M5.Display.println("malloc() failed");    
  }

  for(int i=0; i<LCD_WIDTH*LCD_HEIGHT; i++){
    lcdBuf[i] = GREEN;
  }

  delay(1000);    // delay [ms]

  for(int x=0; x<LCD_WIDTH; x++){
    for(int y=0; y<LCD_HEIGHT; y++){
      //M5.Lcd.drawPixel(x, y, RED);
      M5.Lcd.drawPixel(x, y, lcdBuf[x*LCD_HEIGHT + y]);
      
    }
  }
#endif

  camera_init();

}

void loop() {
  //Serial.println("Hello World");  // Print text on the serial port.在串口输出文本

#if 0   // Check free size of heap memory
  Serial.printf("===============================================================\n");
  Serial.printf("Mem Test\n");
  Serial.printf("===============================================================\n");
  Serial.printf("esp_get_free_heap_size()                              : %6d\n", esp_get_free_heap_size() );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DMA)               : %6d\n", heap_caps_get_free_size(MALLOC_CAP_DMA) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_SPIRAM)            : %6d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_INTERNAL)          : %6d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DEFAULT)           : %6d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );
  delay(1000);          // Delay [ms]
#endif

  camera_capture();

}
