# 🚀 AVR & C Firmware 실습 기록

부트캠프(서울기술교육센터 AI 시스템 반도체 과정)에서 진행한 **C 언어 기초 문법** 실습과 **ATmega123A(AVR)** 보드를 활용한 마이크로컨트롤러(MCU) 제어 소스코드를 기록하는 저장소입니다.

---

## 📁 폴더 구조

- **`avr/`** : ATmega123A 하드웨어 제어 실습 폴더
  - `ai2/ATMEGA123A/01.LED_CONTROL/` : LED 점등, 쉬프트 제어 및 `button.h`를 활용한 스위치 입력 제어 실습
- **`c_practice/`** : C 언어 기초 문법 및 알고리즘 실습 폴더 (과제 및 예제 소스 코드)

---

## 🛠️ 개발 및 실습 환경

- **OS** : Windows 10 / 11
- **MCU** : ATmega123A (AVR 8-bit Microcontroller)
- **IDE / 컴파일러** : Microchip Studio (구 Atmel Studio) / GCC Compiler
- **Language** : Embedded C / C++

---

## 📝 주요 실습 내용

### 1. AVR (ATmega123A) 하드웨어 제어
- [x] GPIO 입출력 제어 (DDR, PORT, PIN 레지스터 활용)
- [x] LED 점등 및 Shift 연산자를 이용한 순차 점등
- [x] 스위치(Button) 입력 인터럽트 디바운싱 및 제어 (`button.h`)
- [ ] Timer/Counter를 이용한 LED 제어 (예정)
- [ ] PWM 제어 및 세그먼트 표시 (예정)

### 2. C 프로그래밍 기초
- [x] C 언어 기본 연산자 및 제어문 (if, switch, for, while)
- [ ] 포인터(Pointer)와 배열 구조 이해 (예정)
- [ ] 구조체(Structure) 및 동적 할당 (예정)

---

## 🏃‍♂️ 실행 및 빌드 방법

1. **Microchip Studio**를 실행하여 `.atsln` 솔루션 파일을 엽니다.
2. `F7` 또는 `Build Solution`을 눌러 컴파일을 진행합니다.
3. 빌드가 완료되면 생성된 `Debug/01.LED_CONTROL.hex` 파일을 아트메가 보드에 다운로드하여 동작을 확인합니다.
