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

#ifndef WarmTheSDS011_h
#define WarmTheSDS011_h

#include <Arduino.h>
#include <SDS011.h>

#define DEBUG_SDS011

class WarmTheSDS011{

public:

    WarmTheSDS011(uint8_t pin_rx, uint8_t pin_tx);
    void begin(unsigned long readingInterval, unsigned long warmupTime);
	void update();
    void requestPollution();
	void onIntervalElapsed(void(*callback)(float pm25, float pm10, bool valid)) {
		cb_onIntervalElapsed = callback;
	}

private:

    enum sensorState {
		sleeping,
        warmingup,
		reading
	};

    sensorState _currentState;
    unsigned long _lastReadingMillis;
    unsigned long _warmupTimeMillis;
    unsigned long _startWarmupMillis;
    unsigned long _sdsReadingInterval;

    void sleepSensor();
    void warmupSensor();
    void readSensor();

    void(*cb_onIntervalElapsed)(float pm25, float pm10, bool valid);
};

#endif