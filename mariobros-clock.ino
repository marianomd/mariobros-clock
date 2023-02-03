#include <PxMatrix.h>
#include <Ticker.h>

#include "CWDateTime.h"
#include "Clockface.h"
#include "WiFiConnect.h"

#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

PxMATRIX *dma_display = new PxMATRIX(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

// ISR for display refresh
void display_updater() {
  dma_display->display(20);
}

Ticker display_ticker;
WiFiConnect wifi;
CWDateTime cwDateTime;
Clockface *clockface;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

byte displayBright = 32;

void displaySetup() {
  //   HUB75_I2S_CFG mxconfig(
  //     64,   // module width
  //     64,   // module height
  //     1    // Chain length
  //   );

  //   mxconfig.gpio.e = 18;
  //   mxconfig.clkphase = false;

  // #if HUB75_BLUE_GREEN_SWAP
  //   // Swap Blue and Green pins because the panel is RBG instead of RGB.
  //   mxconfig.gpio.b1 = 26;
  //   mxconfig.gpio.b2 = 12;
  //   mxconfig.gpio.g1 = 27;
  //   mxconfig.gpio.g2 = 13;
  // #endif

  // Display Setup
  // dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin(32);
  dma_display->setFastUpdate(true);
  dma_display->setBrightness(100);
  dma_display->clearDisplay();
  dma_display->fillScreen(myBLACK);
  display_ticker.attach(0.004, display_updater);
  yield();
}
int reset = 0;

void setup() {
  Serial.begin(115200);

   //wifiManager.resetSettings();


  displaySetup();

  clockface = new Clockface(dma_display);

  dma_display->setTextSize(1);
  dma_display->setTextColor(myWHITE);
  dma_display->setCursor(5, 0);
  String tz = myTZ.getTimezoneName();
  dma_display->print("TZ: ");
  dma_display->println(tz);
  dma_display->setTextColor(myBLUE);
  dma_display->setCursor(0, 32);
  dma_display->print("connecting...");

  wifi.connect();
  cwDateTime.begin();

  clockface->setup(&cwDateTime);
}

void loop() {
  clockface->update();
}
