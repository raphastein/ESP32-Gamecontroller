  // ESP32 Joystick Controller for Minecraft and Warframe
  // This code allows you to control Minecraft and Warframe using a joystick and buttons.
  // It uses the ESP32 platform with USB HID capabilities to emulate a keyboard and mouse.
  // The joystick controls movement, while buttons are used for actions like jumping, sneaking, and auto-clicking.
  // The code also includes an auto-clicker feature that can be adjusted for speed.
  // The Adafruit NeoPixel library is used to provide visual feedback through LEDs.

  #include <Arduino.h>

  // Include the Adafruit NeoPixel library
  #include <Adafruit_NeoPixel.h>
  #define PIN 35
  #define NUMPIXELS 8
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

  // Include the USB and HID libraries
  #include <USB.h>
  #include <USBHIDKeyboard.h>
  #include <USBHIDMouse.h>
  USBHIDMouse Mouse;
  USBHIDKeyboard Keyboard;

  //Pin and Values for Joystick
  int SchwellwertXMin = 1000;
  int SchwellwertXMax = 2900;
  int SchwellwertYMin = 1000;
  int SchwellwertYMax = 3000;
  int JoystickX = 4;
  int JoystickY = 5;
  int ValueX;
  int ValueY;
  int JoystickButton = 6;
  int InventoryValue;

  //Sneak and Jump
  int ButtonJump = 46;
  int ButtonSneak = 3;
  int ValueJump;
  int ValueSneak;

  // Autocklicker
  int ButtonMOUSE = 10; 
  int MOUSEValue = LOW;
  int AutocklickerDelay = 1;
  int AutocklickerUP = 1;
  int AutocklickerDOWN = 37;
  int ValueUP;
  int ValueDOWN;
  int AutocklickerSpeed;
  int currentMillis;
  int previousMillis;
  int intervall = 500;
  int OldMillis;

  // For choosing the Game
  int GameSwitch = 2;
  int Game;
  int ButtonOld;

  void Minecraft()
  {
    // Autocklicker
    if(currentMillis - previousMillis >= intervall)
    {
      previousMillis = currentMillis;
      if(ValueUP == HIGH)
      {
        AutocklickerSpeed=AutocklickerSpeed+1;
      }
      if(ValueDOWN == HIGH)
      {
        AutocklickerSpeed=AutocklickerSpeed-1;
      }
    }
    if(AutocklickerSpeed <= -1)
    {
      AutocklickerSpeed = 0;
    }
    if(MOUSEValue == HIGH && Mouse.isPressed(MOUSE_LEFT))
    {
      if (currentMillis - OldMillis >= AutocklickerDelay)
      {
        OldMillis = currentMillis;
        digitalWrite(13, HIGH);
        Mouse.release(MOUSE_LEFT);
        pixels.show();
      }
    }
    if(MOUSEValue == HIGH && !Mouse.isPressed(MOUSE_LEFT))
    {
      if (currentMillis - OldMillis >= AutocklickerDelay)
      {
        Mouse.press(MOUSE_LEFT);
        OldMillis = currentMillis;
      }
    }
    if(MOUSEValue == LOW)
    {
      Mouse.release(MOUSE_LEFT);
      digitalWrite(13, LOW);
      pixels.clear();
      pixels.setPixelColor(AutocklickerSpeed, pixels.Color(255,0,0));
      pixels.show();
    }

    // Joystick Control
    if (ValueY > SchwellwertYMax) 
    {
      Keyboard.press('a');
      Keyboard.release('d');
      digitalWrite(13, HIGH);
      pixels.setPixelColor(0, pixels.Color(255,0,0));
    }
    if (ValueY < SchwellwertYMin)
    {
      Keyboard.press('d');
      Keyboard.release('a');
      digitalWrite(13, HIGH);
      pixels.setPixelColor(0, pixels.Color(255,0,0));
    }
    if (ValueY > SchwellwertYMin && ValueY < SchwellwertYMax)
    {
      Keyboard.release('a');
      Keyboard.release('d');
      digitalWrite(13, LOW);
      pixels.setPixelColor(0, pixels.Color(0,0,0));
    }
    if (ValueX > SchwellwertXMax) 
    {
      Keyboard.press('s');
      Keyboard.release('w');
      digitalWrite(13, HIGH);
      pixels.setPixelColor(0, pixels.Color(0,255,0));
    }
    if (ValueX < SchwellwertXMin)
    {
      Keyboard.press('w');
      Keyboard.release('s');
      digitalWrite(13, HIGH);
      pixels.setPixelColor(0, pixels.Color(0,255,0));
    }
    if (ValueX > SchwellwertXMin && ValueX < SchwellwertXMax)
    {
      Keyboard.release('w');
      Keyboard.release('s');
      digitalWrite(13, LOW);
      pixels.setPixelColor(0, pixels.Color(0,0,0));
    }
    if (InventoryValue == LOW && ButtonOld == LOW)
    {
      Keyboard.press('e');
      pixels.setPixelColor(0, pixels.Color(255,255,255));
      ButtonOld = HIGH;
    }
    if (InventoryValue == HIGH)
    {
      Keyboard.release('e');
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      ButtonOld = LOW;
    }
    
    // Jump and Sneak
    if (ValueJump == HIGH)
    {
      Keyboard.press('l');
      Keyboard.release('s');
      Keyboard.release('a');
      digitalWrite(13, HIGH);
      pixels.setPixelColor(0, pixels.Color(0,0,255));
    }
    else
    {
      Keyboard.release('l');
      pixels.setPixelColor(0, pixels.Color(0,0,0));
    }
    if (ValueSneak == HIGH)
    {
      Keyboard.press('k');
      Keyboard.release('s');
      Keyboard.release('a');
      digitalWrite(13, LOW);
      pixels.setPixelColor(0, pixels.Color(0,0,255));
    }
    else
    {
      Keyboard.release('k');
      pixels.setPixelColor(0, pixels.Color(0,0,0));
    }
  }

  void Warframe()
  {
    // Joystick Control
    if (ValueY > SchwellwertYMax) 
    {
      Keyboard.press('a');
      Keyboard.release('d');
      digitalWrite(13, HIGH);
    }
    if (ValueY < SchwellwertYMin) 
    {
      Keyboard.press('d');
      Keyboard.release('a');
      digitalWrite(13, HIGH);
    }
    if (ValueY > SchwellwertYMin && ValueY < SchwellwertYMax)
    {
      Keyboard.release('a');
      Keyboard.release('d');
      digitalWrite(13, LOW);
    }
    if (ValueX > SchwellwertXMax) 
    {
      Keyboard.press('s');
      Keyboard.release('w');
      digitalWrite(13, HIGH);
    }
    if (ValueX < SchwellwertXMin)
    {
      Keyboard.press('w');
      Keyboard.release('s');
      digitalWrite(13, HIGH);
    }
    if (ValueX > SchwellwertXMin && ValueX < SchwellwertXMax)
    {
      Keyboard.release('w');
      Keyboard.release('s');
      digitalWrite(13, LOW);
    }
    if (JoystickButton == 0)
    {
      Keyboard.press('x');
    }
    else 
    {
      Keyboard.release('x');
    }
    // Jump and Sneak
    if (ValueJump == HIGH)
    {
      Keyboard.press(KEY_RIGHT_SHIFT);
      Keyboard.release('s');
      Keyboard.release('a');
      digitalWrite(13, HIGH);
    }
    else
    {
      Keyboard.release(KEY_RIGHT_SHIFT);
    }
    if (ValueSneak == HIGH)
    {
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.release('s');
      Keyboard.release('a');
      digitalWrite(13, LOW);
    }
    else
    {
      Keyboard.release(KEY_LEFT_SHIFT);
    }
    if (MOUSEValue == HIGH)
    {
      Keyboard.press('e');
    }
    else
    {
    Keyboard.release('e');
    }
  }

  void setup() 
  {
    // Initialize Serial, Keyboard, Mouse, USB, and NeoPixel libraries
    Serial.begin(9600);
    Keyboard.begin();
    Mouse.begin();
    USB.begin();
    pixels.begin();
    pixels.begin();

    delay(1000);

    // Define the pins for the joystick and buttons
    pinMode(13, OUTPUT);
    pinMode(JoystickX, INPUT);
    pinMode(JoystickY, INPUT);
    pinMode(JoystickButton, INPUT_PULLUP);
    pinMode(ButtonMOUSE, INPUT_PULLDOWN);
    pinMode(AutocklickerUP, INPUT_PULLDOWN);
    pinMode(AutocklickerDOWN, INPUT_PULLDOWN);
    pinMode(ValueJump, INPUT_PULLDOWN);
    pinMode(ValueSneak, INPUT_PULLDOWN);
    pinMode(GameSwitch, INPUT_PULLDOWN);
  }

  void loop() 
  {
    // Read all joystick values and buttons
    ValueUP = digitalRead(AutocklickerUP);
    ValueDOWN = digitalRead(AutocklickerDOWN);
    ValueJump = digitalRead(ButtonJump);
    ValueSneak = digitalRead(ButtonSneak);
    currentMillis = millis();
    MOUSEValue = digitalRead(ButtonMOUSE);
    InventoryValue = digitalRead(JoystickButton);
    ValueX = analogRead(JoystickX);
    ValueY = analogRead(JoystickY);
    ValueJump = digitalRead(ButtonJump);
    ValueSneak = digitalRead(ButtonSneak);
    Game = digitalRead(GameSwitch);

    //Caalculations for the Autocklicker
    double CPS = pow(2, AutocklickerSpeed);
    AutocklickerDelay = 1000/CPS;

    //Choose the Game
    if(Game == LOW)
    {
      Minecraft();
    }
    else
    {
      Warframe();
    }
    // Show the pixels
    pixels.show();
  }
