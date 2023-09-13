//**************** RGB Ring
#include <FastLED.h>
#define LED_PIN     6
#define NUM_LEDS    24
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

//*************** Touch Sensor 
#define sensorT 2

//************** Serial Data
String inputString = "";
String valor;
int k=0;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  pinMode(sensorT, INPUT);

}

void loop() {

if (digitalRead(sensorT)){
  //Serial.println(digitalRead(sensorT));
  SetupPurpleAndGreenPalette();             
  currentBlending = LINEARBLEND;
  while(digitalRead(sensorT)){
    digitalRead(sensorT);
  }
}

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void serialEvent(){
  while (Serial.available()) {              
    char inChar = (char)Serial.read();   // get the new byte:
    inputString += inChar; // add it to the inputString:
    if (inChar == '\n') {
      ValidateSerialData(inputString);
      inputString = "";
    }
  }
}

void ValidateSerialData(String data){
  //Serial.print("lo que llega ");
  Serial.println(data);
  valor = data.substring(1, data.length());
  //Serial.println("separando ");
  //Serial.println(ringFuntion);
  //Serial.println(" <> ");
  //Serial.println(valor);
  k = 0;

  

  if (data.substring(0, 1) == "X") {
    OffRing();
  }

  if (data.substring(0, 1) == "A") {
    currentPalette = PartyColors_p;           
    currentBlending = LINEARBLEND;      
  }

  if (data.substring(0, 1) == "B") {
    FillLEDsBlue();
  }

  if (data.substring(0, 1) == "R") {
    FillLEDsRed();
  }

  if (data.substring(0, 1) == "Y") {
    FillLEDsYellow();
  }

  if (data.substring(0, 1) == "G") {
    FillLEDsGreen();
  }

  if (data.substring(0, 1) == "P") {
    FillLEDsPurple();
  }

  if (data.substring(0, 1) == "W") {
    FillLEDsGray();
  }



  if (data.substring(0, 1) == "Z") {
    currentPalette = myRedWhiteBluePalette_p; 
    currentBlending = NOBLEND;     
  }

  if (data.substring(0, 1) == "C") {
    currentPalette = myRedWhiteBluePalette_p; 
    currentBlending = LINEARBLEND;     
  }

  if (data.substring(0, 1) == "T") {
    //FillLEDsRed3();
  }

  
  if (data.substring(0, 1) == "0") {
    FillLEDsRed3();
  }

  if (data.substring(0, 1) == "1") {
    SetupBlackAndWhiteStripedPalette();       
    currentBlending = NOBLEND;
  }

  if (data.substring(0, 1) == "2") {
    SetupPurpleAndGreenPalette();             
    currentBlending = LINEARBLEND;       
  }

  if (data.substring(0, 1) == "3") {
    SetupTotallyRandomPalette();              
    currentBlending = LINEARBLEND;       
  }

  if (data.substring(0, 1) == "4") {
    SetupBlackAndWhiteStripedPalette();       
    currentBlending = LINEARBLEND;      
  }

  if (data.substring(0, 1) == "5") {
    currentPalette = RainbowStripeColors_p;   
    currentBlending = NOBLEND;      
  }

  if (data.substring(0, 1) == "6") {
    currentPalette = RainbowColors_p;         
    currentBlending = LINEARBLEND;      
  }

 if (data.substring(0, 1) == "7") {
    currentPalette = RainbowStripeColors_p;   
    currentBlending = LINEARBLEND;      
  }

  if (data.substring(0, 1) == "8") {
    currentPalette = CloudColors_p;           
    currentBlending = LINEARBLEND;      
  }

  if (data.substring(0, 1) == "9") {
    currentPalette = PartyColors_p;           
    currentBlending = LINEARBLEND;      
  }




}


void StopRGB(){

  
}





void OffRing()
{
// 'black out' all apaga el Ring
    //fill_solid( currentPalette, 16, CRGB::Black);
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Black;
  }
}

void FillLEDsBlue()
{
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Blue;
  }
}

void FillLEDsYellow()
{
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Yellow;
  }
}

void FillLEDsGreen()
{
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Green;
  }
}

void FillLEDsGray()
{
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Gray;
  }
}

void FillLEDsRed()
{
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Red;
  }
}

void FillLEDsRed2()
{
// 'black out' all apaga el Ring
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to Red.
    currentPalette[0] = CRGB::Red;
    currentPalette[2] = CRGB::Red;
    currentPalette[4] = CRGB::Red;
    currentPalette[6] = CRGB::Red;
    currentPalette[8] = CRGB::Red;
    currentPalette[10] = CRGB::Red;
    currentPalette[12] = CRGB::Red;
    //currentPalette[14] = CRGB::Red;
    //currentPalette[16] = CRGB::Red;
}

void FillLEDsRed3()
{
    CRGB orange = CHSV( HUE_ORANGE, 255, 255);
    CRGB red  = CHSV( HUE_RED, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   red,  red,  black,  black,
                                   orange,red, black,  black,
                                   red,  red,  black,  black,
                                   red,red, black,  black );
}

void FillLEDsPurple()
{
  for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CRGB::Purple;
  }
}



void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
