/****************************
* Project: pwmFreq 
* Description: Function to measure the frequency of a signal applied to a pin
* Creation date: 11/3/2024
* Author: Michael Cowlbeck
*****************************/
#define project "pwmFreq"
#define cDAte "11/03/2024"
#define by "Michael Cowlbeck"
const uint8_t ver_hi = 1;
const uint8_t ver_lo = 2;
/****************************/
const int ledPin =  LED_BUILTIN;      // I/O pin used for LED (LED_BUILTIN could be used thoughtout)
const long ledFlash = 500;            // Flash interval (in milliseconds)

const uint8_t signalPin = 3;          // Specify the signal pin

float pwmData = 0;

void splash(){
  Serial.print("Project: ");
  Serial.println(project);
  Serial.print("Version: ");
  Serial.print(ver_hi);
  Serial.print(".");
  Serial.println(ver_lo);
  Serial.print("Date: ");
  Serial.println(cDAte);
  Serial.println(by);
}

void setup() {
  Serial.begin(115200);                         // Set serial comms speed to 115200bps <<<
  splash();                                     // Send project details to serial port
  pinMode(ledPin, OUTPUT);                      // Configure LED I/O pin for output
}

void loop() {
  digitalWrite(ledPin, (millis()%(ledFlash*2) / ledFlash)); // Flash built-in LED (place in main code; can be repeated anywhere)
 
  pwmData = pwmFreq(signalPin);                 // Call the pwmFreq function to measure the signal frequency
  PrintData();                                  // Call the PrintData function to send pwmData to the serial port
}

float pwmFreq(uint8_t pinNo){                   // pwmFreq with watchdog timeout
  pinMode (pinNo, INPUT_PULLUP);                // Enable internal pullup resistor on signal pin
  const uint8_t periods = 50;                   // Waveform periods to measure <<<< INCREASE THIS VALUE TO 
  const uint16_t timeout = 5000;                // Watchdog timeout
  uint16_t watchdog = 0;                        // Watchdog timer
  uint8_t pCount = 0;                           // Period count
  uint32_t tRise1 = 0;                          // Time at rise
  uint32_t tRise2 = 0;                          // Time at rise
  float Frequency = 0;                          // Signal frequncy
  float tPeriod = 0;                            // Time Period

  while (digitalRead(pinNo)){                   // Wait for Low to get full pulse
    if(watchdog++ > timeout) return (0);        // Timeout with zero if no signal
  }
  
  for (pCount = 0; pCount < periods; pCount++){ // Sample averaging loop
    watchdog = 0;                               // Reset watchdog timer for loop
    // First pulse
    while (!digitalRead(pinNo)){                // Wait for High
      if(watchdog++ > timeout) return (0);      // Timeout with zero if no signal
    }
    tRise1 = micros();                          // Mark rise 1
    while (digitalRead(pinNo)){                 // Wait for Low
      if(watchdog++ > timeout) return (0);      // Timeout with zero if no signal
    }
    // Second pulse
    while (!digitalRead(pinNo)){                // Wait for High
      if(watchdog++ > timeout) return (0);      // Timeout with zero if no signal
    }
    tRise2 = micros();                          // Mark rise 2
    while (digitalRead(pinNo)){                 // Wait for Low
      if(watchdog++ > timeout) return (0);      // Timeout with zero if no signal
    }
    tPeriod += (tRise2 - tRise1);               // record time period sum
  }
  tPeriod /= pCount;                            // Average time period
  Frequency = (1/(tPeriod/1000000));            // Convert tPeriod to seconds and calc recipricol

  return (Frequency);
}

void PrintData(){                               // Send pwm frequency to the serial port
  Serial.print("Frequncy: ");
  Serial.print(pwmData);
  Serial.println("Hz");
}
