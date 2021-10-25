// MIT License
//
// Copyright(c) 2021 Giovanni Bertazzoni <nottheworstdev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "WarmTheSDS011.h"

SDS011 _sds011;

WarmTheSDS011::WarmTheSDS011(uint8_t pin_rx, uint8_t pin_tx){
    _sds011.begin(pin_rx, pin_tx);
    delay(2000);
}

void WarmTheSDS011::begin(unsigned long readingInterval, unsigned long warmupTime){
    _currentState = sleeping;
    _warmupTimeMillis = warmupTime;
    _sdsReadingInterval = readingInterval;
    _lastReadingMillis = 0;
}


//==============================================================================================
//									PRIVATE
//==============================================================================================

void WarmTheSDS011::sleepSensor(){
    if(_lastReadingMillis != 0 && (millis() - _lastReadingMillis < _sdsReadingInterval - _warmupTimeMillis)) return;
    requestPollution();
}

void WarmTheSDS011::warmupSensor(){
    if(millis() - _startWarmupMillis < _warmupTimeMillis) return;
    _currentState = reading;

    #ifdef DEBUG_SDS011
        Serial.println("SDS011: start new reading");
    #endif
}

void WarmTheSDS011::readSensor(){
    float p25, p10;
    int error = _sds011.read(&p25,&p10);
    
    #ifdef DEBUG_SDS011
        Serial.print("SDS011: pm2.5 ");
        Serial.print(p25);
        Serial.print(" pm10 ");
        Serial.println(p10);
        Serial.println("SDS011: sleeping");
    #endif

    delay(200);
    _sds011.sleep();
    _lastReadingMillis = millis();
    _currentState = sleeping;

    if(cb_onIntervalElapsed)(*cb_onIntervalElapsed)(p25, p10, error == 0);
}

//==============================================================================================
//									PUBLIC
//==============================================================================================

void WarmTheSDS011::update(){
    switch(_currentState){
        case sleeping:
            sleepSensor();
        break;
        case warmingup:
            warmupSensor();
        break;
        case reading:
            readSensor();
        break;
    }
}

bool WarmTheSDS011::isBusy(){
    return _currentState != sleeping;
}

void WarmTheSDS011::requestPollution(){
    _currentState = warmingup;
    _startWarmupMillis = millis();
    _sds011.wakeup();
    
    #ifdef DEBUG_SDS011
        Serial.println("SDS011: warming up");
    #endif
}