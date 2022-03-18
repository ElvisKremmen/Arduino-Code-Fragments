/*  Single line to flash the built-in LED without using a delay - by HackBoy.
    This uses the expression 'T Mod(2I) / I' to give a 50% duty cycle,
    where T is the uptime and I is the flash interval (both are in milliseconds).
    This resolves to either 0.x or 1.x which is then passed to the digitalWrite function; ignoing x.
*/
// #define LED_BUILTIN 21
const int ledPin =  LED_BUILTIN; // I/O pin used for LED (LED_BUILTIN could be used thoughtout)
const long ledFlash = 500;       // Flash interval (in milliseconds)

void setup() {
pinMode(ledPin, OUTPUT);         // Configure LED I/O pin for output
}

void loop()
{
  // Your code can be here.
  
  digitalWrite(ledPin, ( millis()%(ledFlash*2) / ledFlash )); // Flash built-in LED (place in main code; can be repeated anywhere)

  // More of your code can be here.

} 
