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


WarmTheSds011::WarmTheSds011(SDS011 *sds011){
    _sds011 = sds011;
}

void WarmTheSds011::begin(unsigned long readingInterval, unsigned long warmupTime){
    _currentState = sleeping;
    _warmupTimeMillis = warmupTime;
    _sdsReadingInterval = readingInterval;
    _lastReadingMillis = 0;
}


//==============================================================================================
//									PRIVATE
//==============================================================================================

void WarmTheSds011::sleepSensor(){
    if(_lastReadingMillis != 0 && (millis() - _lastReadingMillis < _sdsReadingInterval - warmupTime)) return;
    _sds011->wakeup();
    _startWarmupMillis = millis();
    _currentState = warmingup;
}

void WarmTheSds011::warmupSensor(){
    if(millis() - _startWarmupMillis < _warmupTimeMillis) return;
    _currentState = reading;
}

void WarmTheSds011::readSensor(){
    float p25, p10;
    int error = _sds011.read(&p25,&p10);
    _sds011->sleep();
    _lastReadingMillis = millis();
    _currentState = sleeping;
    if(cb_onIntervalElapsed)(*cb_onIntervalElapsed)(p25, p10, error == 0);
}

//==============================================================================================
//									PUBLIC
//==============================================================================================

void WarmTheSds011::update(){
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