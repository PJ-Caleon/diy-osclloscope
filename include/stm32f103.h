#ifndef STM32F103_H
#define STM32F103_H
#include <stdint.h>
#include <stddef.h>

#define BIT(n) (1u << (n))
#define SYSCLK_HZ 72000000u

/* ---------- base addresses ---------- */
#define GPIOC_BASE   0x40011000u 
#define RCC_BASE     0x40021000u 
#define GPIOA_BASE   0x40010800u
#define GPIOB_BASE   0x40010C00u
#define TIM2_BASE    0x40000000u
#define TIM3_BASE    0x40000400u
#define USART2_BASE  0x40004400u
#define I2C1_BASE    0x40005400u
#define ADC1_BASE    0x40012400u
#define USART1_BASE  0x40013800u
#define DMA1_BASE    0x40020000u
#define FLASH_BASE_R 0x40022000u
#define SYSTICK_BASE 0xE000E010u

/* ---------- GPIO ---------- */
typedef struct {
    volatile uint32_t CRL;   /* 0x00 pins 0-7  */
    volatile uint32_t CRH;   /* 0x04 pins 8-15 */
    volatile uint32_t IDR;   /* 0x08 */
    volatile uint32_t ODR;   /* 0x0C */
    volatile uint32_t BSRR;  /* 0x10 */
    volatile uint32_t BRR;   /* 0x14 */
    volatile uint32_t LCKR;  /* 0x18 */
} GPIO_TypeDef;

/* ---------- RCC ---------- */
typedef struct {
    volatile uint32_t CR;        /* 0x00 */
    volatile uint32_t CFGR;      /* 0x04 */
    volatile uint32_t CIR;       /* 0x08 */
    volatile uint32_t APB2RSTR;  /* 0x0C */
    volatile uint32_t APB1RSTR;  /* 0x10 */
    volatile uint32_t AHBENR;    /* 0x14 */
    volatile uint32_t APB2ENR;   /* 0x18 */
    volatile uint32_t APB1ENR;   /* 0x1C */
    volatile uint32_t BDCR;      /* 0x20 */
    volatile uint32_t CSR;       /* 0x24 */
} RCC_TypeDef;

/* ---------- other peripherals (given; check each offset against its issue cheat sheet) ---------- */
typedef struct { volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR; } USART_TypeDef;   /* 0x00..0x18 */
typedef struct {
    volatile uint32_t SR, CR1, CR2, SMPR1, SMPR2;          /* 0x00..0x10 */
    volatile uint32_t JOFR[4];                             /* 0x14..0x20 */
    volatile uint32_t HTR, LTR, SQR1, SQR2, SQR3, JSQR;    /* 0x24..0x38 */
    volatile uint32_t JDR[4];                              /* 0x3C..0x48 */
    volatile uint32_t DR;                                  /* 0x4C */
} ADC_TypeDef;
typedef struct {
    volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER;  /* 0x00..0x20 */
    volatile uint32_t CNT, PSC, ARR, RES1, CCR1, CCR2, CCR3, CCR4;        /* 0x24..0x40 */
    volatile uint32_t RES2, DCR, DMAR;                                    /* 0x44..0x4C */
} TIM_TypeDef;
typedef struct { volatile uint32_t CCR, CNDTR, CPAR, CMAR, RESERVED; } DMA_Channel_TypeDef; /* 0x14 bytes */
typedef struct {
    volatile uint32_t ISR, IFCR;          /* 0x00, 0x04 */
    DMA_Channel_TypeDef ch[7];            /* channel 1 starts at 0x08 */
} DMA_TypeDef;
typedef struct { volatile uint32_t CR1, CR2, OAR1, OAR2, DR, SR1, SR2, CCR, TRISE; } I2C_TypeDef; /* 0x00..0x20 */
typedef struct { volatile uint32_t CTRL, LOAD, VAL, CALIB; } SysTick_TypeDef;
typedef struct { volatile uint32_t ACR, KEYR, OPTKEYR, SR, CR, AR; } FLASH_TypeDef;

/* ---------- peripheral pointers ---------- */
#ifdef UNIT_TEST
extern GPIO_TypeDef fake_gpioc;   /* host tests: a fake chip made of plain RAM */
extern RCC_TypeDef  fake_rcc;
#define GPIOC (&fake_gpioc)
#define RCC   (&fake_rcc)
#else
#define GPIOC ((GPIO_TypeDef *)GPIOC_BASE)
#define RCC   ((RCC_TypeDef *)RCC_BASE)
#endif
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define USART1  ((USART_TypeDef *)USART1_BASE)
#define USART2  ((USART_TypeDef *)USART2_BASE)
#define ADC1    ((ADC_TypeDef *)ADC1_BASE)
#define TIM2    ((TIM_TypeDef *)TIM2_BASE)
#define TIM3    ((TIM_TypeDef *)TIM3_BASE)
#define DMA1    ((DMA_TypeDef *)DMA1_BASE)
#define I2C1    ((I2C_TypeDef *)I2C1_BASE)
#define SYSTICK ((SysTick_TypeDef *)SYSTICK_BASE)
#define FLASH   ((FLASH_TypeDef *)FLASH_BASE_R)

/* ---------- bit names ---------- */
#define RCC_APB2ENR_IOPCEN BIT(4) 
#define LED_PIN 13u
/* RCC CR */
#define RCC_CR_HSEON     BIT(16)
#define RCC_CR_HSERDY    BIT(17)
#define RCC_CR_PLLON     BIT(24)
#define RCC_CR_PLLRDY    BIT(25)
/* RCC CFGR field positions */
#define RCC_CFGR_SW_Pos       0u   /* 2 bits: 2 = PLL */
#define RCC_CFGR_SWS_PLL      BIT(3)
#define RCC_CFGR_PPRE1_Pos    8u   /* 3 bits: 4 = /2  */
#define RCC_CFGR_ADCPRE_Pos   14u  /* 2 bits: 2 = /6  */
#define RCC_CFGR_PLLSRC_HSE   BIT(16)
#define RCC_CFGR_PLLMUL_Pos   18u  /* 4 bits: 7 = x9  */
/* RCC enable bits */
#define RCC_AHBENR_DMA1EN    BIT(0)
#define RCC_APB2ENR_AFIOEN   BIT(0)
#define RCC_APB2ENR_IOPAEN   BIT(2)
#define RCC_APB2ENR_IOPBEN   BIT(3)
#define RCC_APB2ENR_ADC1EN   BIT(9)
#define RCC_APB2ENR_USART1EN BIT(14)
#define RCC_APB1ENR_TIM2EN   BIT(0)
#define RCC_APB1ENR_TIM3EN   BIT(1)
#define RCC_APB1ENR_I2C1EN   BIT(21)
/* FLASH ACR */
#define FLASH_ACR_LATENCY_Pos 0u   /* 3 bits: 2 = two wait states */
#define FLASH_ACR_PRFTBE      BIT(4)
/* SysTick CTRL */
#define SYSTICK_ENABLE  BIT(0)
#define SYSTICK_TICKINT BIT(1)
#define SYSTICK_CLKSRC  BIT(2)
/* USART */
#define USART_SR_RXNE BIT(5)
#define USART_SR_TC   BIT(6)
#define USART_SR_TXE  BIT(7)
#define USART_CR1_RE  BIT(2)
#define USART_CR1_TE  BIT(3)
#define USART_CR1_UE  BIT(13)
/* ADC */
#define ADC_SR_EOC       BIT(1)
#define ADC_CR2_ADON     BIT(0)
#define ADC_CR2_CAL      BIT(2)
#define ADC_CR2_DMA      BIT(8)
#define ADC_CR2_EXTSEL_Pos 17u     /* 3 bits: 7 = SWSTART, 4 = TIM3 TRGO */
#define ADC_CR2_EXTTRIG  BIT(20)
#define ADC_CR2_SWSTART  BIT(22)
/* TIM */
#define TIM_CR1_CEN   BIT(0)
#define TIM_CR1_ARPE  BIT(7)
#define TIM_CR2_MMS_Pos 4u         /* 3 bits: 2 = update event -> TRGO */
#define TIM_EGR_UG    BIT(0)
#define TIM_CCMR1_OC2PE BIT(11)
#define TIM_CCMR1_OC2M_Pos 12u     /* 3 bits: 6 = PWM mode 1 */
#define TIM_CCER_CC2E BIT(4)
/* DMA */
#define DMA_CCR_EN    BIT(0)
#define DMA_CCR_MINC  BIT(7)
#define DMA_CCR_PSIZE_Pos 8u       /* 2 bits: 1 = 16-bit */
#define DMA_CCR_MSIZE_Pos 10u      /* 2 bits: 1 = 16-bit */
#define DMA_ISR_TCIF1 BIT(1)
/* I2C */
#define I2C_CR1_PE    BIT(0)
#define I2C_CR1_START BIT(8)
#define I2C_CR1_STOP  BIT(9)
#define I2C_SR1_SB    BIT(0)
#define I2C_SR1_ADDR  BIT(1)
#define I2C_SR1_BTF   BIT(2)
#define I2C_SR1_TXE   BIT(7)
#define I2C_SR1_AF    BIT(10)
#define I2C_SR2_BUSY  BIT(1)
#define I2C_CCR_FS    BIT(15)

#endif /* STM32F103_H */
