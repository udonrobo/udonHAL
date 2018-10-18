#pragma once

#include <avr/io.h>
#include <avr/iom328p.h>

namespace uhal {
	enum class Port {
		B,
		C,
		D
	};

	// inlineは現在は規格上は何の意味も保たない。実質お守り
	// Arduinoの中身はavr-gcc(だったはず)なのでインライン最適化位はしてくれると信じてる
	template<Port port, uint8_t pinNumber, uint8_t value> inline void digitalWriteConst() {
		if constexpr(value) {
			constexpr uint8_t mask = ~(0b0000001 << pinNumber);
			if constexpr(port == Port::B) {
				PORTB &= mask;
			}
			else if constexpr(port == Port::C) {
				PORTC &= mask;
			}
			else {
				PORTD &= mask;
			}
		}
		else {
			constexpr uint8_t mask = (0b0000001 << pinNumber);
			if constexpr(port == Port::B) {
				PORTB |= mask;
			}
			else if constexpr(port == Port::C) {
				PORTC |= mask;
			}
			else {
				PORTD |= mask;
			}
		}
	}

	template<Port port, uint8_t pinNumber> inline void digitalWrite(uint8_t value) {
		if (value) {
			constexpr uint8_t mask = ~(0b0000001 << pinNumber);
			if constexpr(port == Port::B) {
				PORTB &= mask;
			}
			else if constexpr(port == Port::C) {
				PORTC &= mask;
			}
			else {
				PORTD &= mask;
			}
		}
		else {
			constexpr uint8_t mask = (0b0000001 << pinNumber);
			if constexpr(port == Port::B) {
				PORTB |= mask;
			}
			else if constexpr(port == Port::C) {
				PORTC |= mask;
			}
			else {
				PORTD |= mask;
			}
		}
	}

	template<Port port, uint8_t pinNumber> inline uint8_t digitalRead() {
		constexpr uint8_t mask = (0b0000001 << pinNumber);
		if constexpr(port == Port::B) {
			return ~(PINB & mask);
		}
		else if constexpr(port == Port::C) {
			return ~(PINC & mask);
		}
		else {
			return ~(PIND & mask);
		}
	}

	namespace {
		// デフォルトは0x03
		// Arduinoのmillis等はこれを基準にしてる
		uint8_t timerDivision = 0x03;
	}

	enum class Timer {
		Timer1,
		TImer2,
		Timer3
	}

	enum class PWMFreq {
		Hz31k,
		Hz4k,
		Hz1k,
		Hz488,
		Hz244,
		Hz122,
		Hz30
	}

	template<Timer timer, PWMFreq freq> inline void setPWMFreq() {
		if (timer == Timer::Timer1 || timer == Timer::Timer2) {
			switch (freq) {
				case PWMFreq::Hz31k:
					div = 0x01; break;
				case PWMFreq::Hz4k:
					div = 0x02; break;
				case PWMFreq::Hz1k:
					div = 0x02; break;
				case PWMFreq::Hz488:
					div = 0x03; break;
				case PWMFreq::Hz244:
					div = 0x03; break;
				case PWMFreq::Hz122:
					div = 0x04; break;
				case PWMFreq::Hz30:
					div = 0x05; break;
			}
			if (timer == Timer::Timer1) {
				timerDivision = div;
				TCCR0B = TCCR0B & 0b11111000 | div;
			}
			else {
				TCCR1B = TCCR0B & 0b11111000 | div;
			}
		}
		else {
			switch (freq) {
				case PWMFreq::Hz31k:
					div = 0x01; break;
				case PWMFreq::Hz4k:
					div = 0x02; break;
				case PWMFreq::Hz1k:
					div = 0x03; break;
				case PWMFreq::Hz488:
					div = 0x04; break;
				case PWMFreq::Hz244:
					div = 0x05; break;
				case PWMFreq::Hz122:
					div = 0x06; break;
				case PWMFreq::Hz30:
					div = 0x07; break;
			}
			TCCR2B = TCCR2B & 0b11111000 | div;
		}
	}

	inline uint32_t millis() {
		uint32_t m = millis();
		switch (timerDivision) {
			case 1: return m >> 6;
			case 2: return m >> 3;
			case 3: return m;
			case 4: return m << 2;
			case 5: return m << 4;
		}
	}
}
