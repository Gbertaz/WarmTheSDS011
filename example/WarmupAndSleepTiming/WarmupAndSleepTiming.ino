//======================================================================
//======================================================================
//  Program: WarmupAndSleepTiming.ino
//
//  Description: warms up the SDS011 dust sensor, reads the values and
//               sets the sleep mode to extend the sensor's life.
//               https://github.com/Gbertaz/WarmTheSDS011
//
//
//  License:
//
//  Copyright(c) 2021 Giovanni Bertazzoni <nottheworstdev@gmail.com>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//======================================================================
//======================================================================

#include <WarmTheSDS011.h>

#define READING_INTERVAL  15*60*1000       //Sensor reading interval [milliseconds]
#define WARMUP_TIME       30*1000          //Sensor warmup time [milliseconds]
#define SDS_RX D1                          //SDS011 RX Pin
#define SDS_TX D2                          //SDS011 TX Pin

WarmTheSDS011 sds011(SDS_RX, SDS_TX);


void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  
  sds011.begin(READING_INTERVAL, WARMUP_TIME);
  sds011.onIntervalElapsed(OnSdsIntervalElapsed);
}

void loop() {
  sds011.update();
}


void OnSdsIntervalElapsed(float pm25, float pm10, bool valid){
  
}
