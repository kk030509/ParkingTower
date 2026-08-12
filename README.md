# 🅿️ STM32 자동 주차 타워 (Automatic Parking Tower)

## 📌 프로젝트 소개

STM32F411 마이크로컨트롤러를 기반으로 하는 RFID 기반 자동 주차 시스템입니다.
RFID 카드 인식, 자동 차량 입출차, 주차 시간 추적, 실시간 상태 표시 기능을 제공합니다.
프로젝트는 기능별 모듈화(Modular Design)를 적용하여 유지보수성과 재사용성을 높였습니다.

## ⚙️ 개발 환경

- **Language**: C (HAL 라이브러리)
- **MCU Board**: STM32F411 Discovery
- **MCU**: STM32F411RE (ARM Cortex-M4, 100MHz)
- **Tool**: STM32CubeIDE

## ✨ 주요 기능

- 🏷️ RFID 자동 인식 (MFRC522)
- 🚗 자동 입출차 제어 (스텝 모터)
- ⏱️ 주차 시간 자동 추적
- 💾 8개 슬롯 관리
- 📺 실시간 상태 표시 (LCD)
- 🎯 포토센서 기반 슬롯 감지
- 💡 LED 상태 표시
- 🔘 YES/NO 버튼 입력

## 🖥️ 시스템 구조

```
RFID Card Detection
      │
      ▼
MFRC522 Module (SPI)
      │
      ├─────────────┐
      ▼             ▼
Car Found?      Empty Slot?
      │             │
   YES/NO?    Assign to Slot
      │             │
      ▼             ▼
State Machine  Stepper Motor
      │             │
      ├─────┬───────┘
      ▼     ▼
  LCD UI    Photo Sensor
      │        │
      └───┬────┘
          ▼
       LED Status
```

## 📂 프로젝트 구조

```
Core/
├── main.c                    # 메인 루프, 상태 머신
├── stm32f4xx_it.c            # 인터럽트 핸들러
├── stm32f4xx_hal_msp.c       # HAL MSP
└── system_stm32f4xx.c        # 시스템 초기화

Peripheral/
├── gpio.c                    # GPIO 설정
├── spi.c                     # SPI1 초기화 (RFID)
├── i2c.c                     # I2C1 초기화 (LCD)
├── usart.c                   # USART2 (디버그)
├── adc.c                     # ADC1 (포토센서)
└── button.c                  # 버튼 입력 (디바운싱)

Devices/
├── RC522.c                   # RFID 리더 드라이버
├── HD44780_LCD.c             # LCD 드라이버
└── stepper.c                 # 스텝 모터 제어

Application/
├── parking.c                 # 주차 데이터 관리
├── tower.c                   # 타워 이동 로직
├── led.c                     # LED 상태 업데이트
├── lcd_ui.c                  # LCD 화면 표시
└── button.c                  # 버튼 디바운싱
```

## 🧩 모듈 구성

| 모듈 | 파일명 | 기능 |
|------|--------|------|
| **RFID Reader** | RC522.c | MFRC522 SPI 통신, UID 인식 |
| **LCD Display** | HD44780_LCD.c | I2C 백팩 HD44780 제어 |
| **Stepper Motor** | stepper.c | 28BYJ-48 스텝 모터, 슬롯 이동 |
| **Photo Sensor** | stepper.c | ADC 포토센서 감지 |
| **Parking Manager** | parking.c | 차량 데이터베이스, 슬롯 관리 |
| **Tower Control** | tower.c | 최단 경로 이동 계산 |
| **LED Control** | led.c | 8개 슬롯 상태 LED |
| **UI Display** | lcd_ui.c | LCD 화면 업데이트 |
| **Button Input** | button.c | 디바운싱 기반 버튼 처리 |

## 🎮 스위치 기능

| 핀 | 기능 |
|----|------|
| PC8 | YES 버튼 (입출차 확인) |
| PC9 | NO 버튼 (작업 취소) |

## 🎮 버튼 기능

### YES 버튼 (PC8)
- 입차 또는 출차 작업 승인
- 디바운싱: 20ms

### NO 버튼 (PC9)
- 입차 또는 출차 작업 취소
- 디바운싱: 20ms

## 🚗 주차 관리 시스템

### 입차 프로세스
```
1. RFID 카드 감지
2. 데이터베이스 조회 (이미 주차된 차량인가?)
   ├─ YES → 출차 모드로 진행
   └─ NO  → 빈 슬롯 탐색
3. 빈 슬롯이 있으면 사용자 확인 요청
4. YES: 해당 슬롯으로 이동, 입차 완료
   NO: 작업 취소, 대기 상태로 복귀
5. 입차 시간 기록 (slotEntryTick)
```

### 출차 프로세스
```
1. RFID 카드 감지
2. 데이터베이스 조회 (주차된 차량 찾기)
3. 차량 발견 시 슬롯 위치 확인
4. 사용자 확인 요청
5. YES: 해당 슬롯으로 이동, 출차 완료
   NO: 작업 취소, 대기 상태로 복귀
6. 주차 시간 계산 및 감사 로그 출력
```

### 데이터 구조
```c
typedef struct {
    uint8_t uid[5];       // RFID UID
    uint8_t parked;       // 1=주차, 0=비어있음
    uint8_t slot;         // 슬롯 번호 (0-7)
} ParkingInfo;
```

## 🏢 상태 머신 (AppState)

```
┌─────────────────────────────────────┐
│          APP_IDLE                   │
│    (RFID 대기, LCD 초기 화면)       │
└─────────────┬───────────────────────┘
              │ RFID 감지
              ▼
    ┌─────────────────────┐
    │ 신차? 기존차?      │
    └──┬──────────────┬──┘
       │ 신차          │ 기존차
       ▼               ▼
┌────────────────┐ ┌──────────────────┐
│APP_CONFIRM_IN  │ │APP_CONFIRM_OUT   │
│ (입차 확인)     │ │ (출차 확인)      │
└─┬─────────┬─┬──┘ └──┬────────────┬──┘
  │YES NO  │ │       │YES NO      │
  ▼   ▼   │ │       ▼   ▼        │
┌──────┐  │ │   ┌──────┐        │
│MOVE_ │  │ │   │MOVE_ │        │
│IN    │  │ │   │OUT   │        │
└──┬───┘  │ │   └──┬───┘        │
   │      │ │      │             │
   ▼      │ │      ▼             ▼
┌───────┐ │ │  ┌─────────┐   ┌────────┐
│IN_COM-│ │ │  │OUT_COM- │   │CANCEL  │
│PLETE  │ │ │  │PLETE    │   │        │
└───┬───┘ │ │  └────┬────┘   └───┬────┘
    │     │ │       │            │
    └─────┴─┴───────┴────────────┘
         (2-3초 후)
          ▼
     APP_IDLE (복귀)
```

## 🔌 하드웨어 핀 할당

### SPI (RC522 RFID)
| 신호 | 핀 | 기능 |
|------|-----|------|
| SCK | PA5 | SPI1 클록 |
| MISO | PA6 | SPI1 입력 |
| MOSI | PA7 | SPI1 출력 |
| CS | PA4 | 칩 선택 |

### I2C (LCD)
| 신호 | 핀 | 기능 |
|------|-----|------|
| SCL | PB6 | I2C 클록 |
| SDA | PB7 | I2C 데이터 |

### 모터 제어 (스텝 모터)
| 신호 | 핀 | 기능 |
|------|-----|------|
| IN1 | PB1 | 코일1 |
| IN2 | PB10 | 코일2 |
| IN3 | PB12 | 코일3 |
| IN4 | PB13 | 코일4 |

### LED (슬롯 상태)
| 슬롯 | 핀 | GPIO |
|------|-----|------|
| 0 | PB12 | GPIOB |
| 1 | PC7 | GPIOC |
| 2 | PA9 | GPIOA |
| 3 | PB10 | GPIOB |
| 4 | PB4 | GPIOB |
| 5 | PB5 | GPIOB |
| 6 | PB3 | GPIOB |
| 7 | PA10 | GPIOA |

### 버튼
| 버튼 | 핀 | 기능 |
|------|-----|------|
| YES | PC8 | PULLUP |
| NO | PC9 | PULLUP |

### ADC (포토센서)
| 신호 | 핀 | 기능 |
|------|-----|------|
| Photo | PA0 | ADC1_IN0 |

### USART (디버그)
| 신호 | 핀 | 기능 |
|------|-----|------|
| TX | PA2 | USART2_TX |
| RX | PA3 | USART2_RX |


```

## 📊 LCD 화면 표시

| 상태 | 1행 | 2행 |
|------|-----|-----|
| 대기 | "Parking Tower" | "Empty : X" |
| 입차 확인 | "Parking In?" | "Slot #X Y/N" |
| 입차 이동 | "Moving..." | "Slot #X" |
| 입차 완료 | "Complete" | "Slot #X In" |
| 입차 취소 | "Parking" | "Canceled" |
| 출차 확인 | "Parking Out?" | "Slot #X Y/N" |
| 출차 완료 | "Thank You" | "Time MM:SS" |
| 출차 취소 | "Exit" | "Canceled" |

## 🎯 동작 흐름

```
[시작]
  │
  ▼
[IDLE - RFID 대기]
  │
  ├─ RFID 카드 감지
  │
  ├─ UID 읽기 및 데이터베이스 조회
  │
  ├─ 신규 차량? ─── YES ──→ [입차 확인 화면]
  │                            │
  │                         YES/NO 버튼
  │                            │
  │                        ┌───┴───┐
  │                        ▼       ▼
  │                      YES      NO
  │                        │       │
  │                        ▼       ▼
  │                   [이동]   [취소]
  │                        │       │
  │                        ▼       ▼
  │                   [완료]   [대기]
  │                        │
  │ 기존 차량? ─── YES ──→ [출차 확인 화면]
  │                            │
  │                         YES/NO 버튼
  │                            │
  │                        ┌───┴───┐
  │                        ▼       ▼
  │                      YES      NO
  │                        │       │
  │                        ▼       ▼
  │                   [이동]   [취소]
  │                        │       │
  │                        ▼       ▼
  │                   [감사]   [대기]
  │
  └─────────────────────→ [대기]
```

