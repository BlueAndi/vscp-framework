#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTC6 (number 78), U8[11]/SW2 */
#define BOARD_SW2_GPIO                                                     GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_SW2_PORT                                                     PORTC   /*!< PORT device name: PORTC */
#define BOARD_SW2_GPIO_PIN                                                    6U   /*!< PORTC pin index: 6 */
#define BOARD_SW2_PIN_NAME                                                  PTC6   /*!< Pin name */
#define BOARD_SW2_LABEL                                             "U8[11]/SW2"   /*!< Label */
#define BOARD_SW2_NAME                                                     "SW2"   /*!< Identifier name */
#define BOARD_SW2_DIRECTION                              kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTA4 (number 38), SW3 */
#define BOARD_SW3_GPIO                                                     GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_SW3_PORT                                                     PORTA   /*!< PORT device name: PORTA */
#define BOARD_SW3_GPIO_PIN                                                    4U   /*!< PORTA pin index: 4 */
#define BOARD_SW3_PIN_NAME                                                  PTA4   /*!< Pin name */
#define BOARD_SW3_LABEL                                                    "SW3"   /*!< Label */
#define BOARD_SW3_NAME                                                     "SW3"   /*!< Identifier name */
#define BOARD_SW3_DIRECTION                              kPIN_MUX_DirectionInput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtons(void);

/* PORTB21 (number 67), D12[3]/LEDRGB_BLUE */
#define BOARD_LED_BLUE_GPIO                                                GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_LED_BLUE_PORT                                                PORTB   /*!< PORT device name: PORTB */
#define BOARD_LED_BLUE_GPIO_PIN                                              21U   /*!< PORTB pin index: 21 */
#define BOARD_LED_BLUE_PIN_NAME                                            PTB21   /*!< Pin name */
#define BOARD_LED_BLUE_LABEL                                "D12[3]/LEDRGB_BLUE"   /*!< Label */
#define BOARD_LED_BLUE_NAME                                           "LED_BLUE"   /*!< Identifier name */
#define BOARD_LED_BLUE_DIRECTION                        kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB22 (number 68), D12[1]/LEDRGB_RED */
#define BOARD_LED_RED_GPIO                                                 GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_LED_RED_PORT                                                 PORTB   /*!< PORT device name: PORTB */
#define BOARD_LED_RED_GPIO_PIN                                               22U   /*!< PORTB pin index: 22 */
#define BOARD_LED_RED_PIN_NAME                                             PTB22   /*!< Pin name */
#define BOARD_LED_RED_LABEL                                  "D12[1]/LEDRGB_RED"   /*!< Label */
#define BOARD_LED_RED_NAME                                             "LED_RED"   /*!< Identifier name */
#define BOARD_LED_RED_DIRECTION                         kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE26 (number 33), J2[1]/D12[4]/LEDRGB_GREEN */
#define BOARD_LED_GREEN_GPIO                                               GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_LED_GREEN_PORT                                               PORTE   /*!< PORT device name: PORTE */
#define BOARD_LED_GREEN_GPIO_PIN                                             26U   /*!< PORTE pin index: 26 */
#define BOARD_LED_GREEN_PIN_NAME                                           PTE26   /*!< Pin name */
#define BOARD_LED_GREEN_LABEL                        "J2[1]/D12[4]/LEDRGB_GREEN"   /*!< Label */
#define BOARD_LED_GREEN_NAME                                         "LED_GREEN"   /*!< Identifier name */
#define BOARD_LED_GREEN_DIRECTION                       kPIN_MUX_DirectionOutput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

/* PORTB17 (number 63), U10[1]/UART0_TX */
#define BOARD_DEBUG_UART_TX_PERIPHERAL                                     UART0   /*!< Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL                                            TX   /*!< UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PIN_NAME                                    UART0_TX   /*!< Pin name */
#define BOARD_DEBUG_UART_TX_LABEL                              "U10[1]/UART0_TX"   /*!< Label */
#define BOARD_DEBUG_UART_TX_NAME                                 "DEBUG_UART_TX"   /*!< Identifier name */
#define BOARD_DEBUG_UART_TX_DIRECTION                   kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTB16 (number 62), U7[4]/UART0_RX */
#define BOARD_DEBUG_UART_RX_PERIPHERAL                                     UART0   /*!< Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL                                            RX   /*!< UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PIN_NAME                                    UART0_RX   /*!< Pin name */
#define BOARD_DEBUG_UART_RX_LABEL                               "U7[4]/UART0_RX"   /*!< Label */
#define BOARD_DEBUG_UART_RX_NAME                                 "DEBUG_UART_RX"   /*!< Identifier name */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UART(void);

/* PORTA18 (number 50), U13[16]/RMII_RXCLK */
#define BOARD_EXTAL0_PERIPHERAL                                              OSC   /*!< Device name: OSC */
#define BOARD_EXTAL0_SIGNAL                                               EXTAL0   /*!< OSC signal: EXTAL0 */
#define BOARD_EXTAL0_PIN_NAME                                             EXTAL0   /*!< Pin name */
#define BOARD_EXTAL0_LABEL                                  "U13[16]/RMII_RXCLK"   /*!< Label */
#define BOARD_EXTAL0_NAME                                               "EXTAL0"   /*!< Identifier name */

/* EXTAL32 (number 29), Y3[2]/EXTAL32_RTC */
#define BOARD_ETAL32K_PERIPHERAL                                             RTC   /*!< Device name: RTC */
#define BOARD_ETAL32K_SIGNAL                                             EXTAL32   /*!< RTC signal: EXTAL32 */
#define BOARD_ETAL32K_PIN_NAME                                           EXTAL32   /*!< Pin name */
#define BOARD_ETAL32K_LABEL                                  "Y3[2]/EXTAL32_RTC"   /*!< Label */
#define BOARD_ETAL32K_NAME                                             "ETAL32K"   /*!< Identifier name */

/* XTAL32 (number 28), Y3[1]/XTAL32_RTC */
#define BOARD_XTAL32K_PERIPHERAL                                             RTC   /*!< Device name: RTC */
#define BOARD_XTAL32K_SIGNAL                                              XTAL32   /*!< RTC signal: XTAL32 */
#define BOARD_XTAL32K_PIN_NAME                                            XTAL32   /*!< Pin name */
#define BOARD_XTAL32K_LABEL                                   "Y3[1]/XTAL32_RTC"   /*!< Label */
#define BOARD_XTAL32K_NAME                                             "XTAL32K"   /*!< Identifier name */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSC(void);

/* PORTE25 (number 32), J2[18]/U8[6]/I2C0_SDA */
#define BOARD_ACCEL_SDA_PERIPHERAL                                          I2C0   /*!< Device name: I2C0 */
#define BOARD_ACCEL_SDA_SIGNAL                                               SDA   /*!< I2C0 signal: SDA */
#define BOARD_ACCEL_SDA_PIN_NAME                                        I2C0_SDA   /*!< Pin name */
#define BOARD_ACCEL_SDA_LABEL                            "J2[18]/U8[6]/I2C0_SDA"   /*!< Label */
#define BOARD_ACCEL_SDA_NAME                                         "ACCEL_SDA"   /*!< Identifier name */

/* PORTE24 (number 31), J2[20]/U8[4]/I2C0_SCL */
#define BOARD_ACCEL_SCL_PERIPHERAL                                          I2C0   /*!< Device name: I2C0 */
#define BOARD_ACCEL_SCL_SIGNAL                                               SCL   /*!< I2C0 signal: SCL */
#define BOARD_ACCEL_SCL_PIN_NAME                                        I2C0_SCL   /*!< Pin name */
#define BOARD_ACCEL_SCL_LABEL                            "J2[20]/U8[4]/I2C0_SCL"   /*!< Label */
#define BOARD_ACCEL_SCL_NAME                                         "ACCEL_SCL"   /*!< Identifier name */

/* PORTC6 (number 78), U8[11]/SW2 */
#define BOARD_ACCEL_INT1_GPIO                                              GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_ACCEL_INT1_PORT                                              PORTC   /*!< PORT device name: PORTC */
#define BOARD_ACCEL_INT1_GPIO_PIN                                             6U   /*!< PORTC pin index: 6 */
#define BOARD_ACCEL_INT1_PIN_NAME                                           PTC6   /*!< Pin name */
#define BOARD_ACCEL_INT1_LABEL                                      "U8[11]/SW2"   /*!< Label */
#define BOARD_ACCEL_INT1_NAME                                       "ACCEL_INT1"   /*!< Identifier name */
#define BOARD_ACCEL_INT1_DIRECTION                       kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC13 (number 85), U8[9] */
#define BOARD_ACCEL_INT2_GPIO                                              GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_ACCEL_INT2_PORT                                              PORTC   /*!< PORT device name: PORTC */
#define BOARD_ACCEL_INT2_GPIO_PIN                                            13U   /*!< PORTC pin index: 13 */
#define BOARD_ACCEL_INT2_PIN_NAME                                          PTC13   /*!< Pin name */
#define BOARD_ACCEL_INT2_LABEL                                           "U8[9]"   /*!< Label */
#define BOARD_ACCEL_INT2_NAME                                       "ACCEL_INT2"   /*!< Identifier name */
#define BOARD_ACCEL_INT2_DIRECTION                       kPIN_MUX_DirectionInput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCEL(void);

/* PORTB1 (number 54), U13[11]/RMII0_MDC */
#define BOARD_RMII0_MDC_PERIPHERAL                                          ENET   /*!< Device name: ENET */
#define BOARD_RMII0_MDC_SIGNAL                                          RMII_MDC   /*!< ENET signal: RMII_MDC */
#define BOARD_RMII0_MDC_PIN_NAME                                       RMII0_MDC   /*!< Pin name */
#define BOARD_RMII0_MDC_LABEL                                "U13[11]/RMII0_MDC"   /*!< Label */
#define BOARD_RMII0_MDC_NAME                                         "RMII0_MDC"   /*!< Identifier name */

/* PORTB0 (number 53), U13[10]/RMII0_MDIO */
#define BOARD_RMII0_MDIO_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_MDIO_SIGNAL                                        RMII_MDIO   /*!< ENET signal: RMII_MDIO */
#define BOARD_RMII0_MDIO_PIN_NAME                                     RMII0_MDIO   /*!< Pin name */
#define BOARD_RMII0_MDIO_LABEL                              "U13[10]/RMII0_MDIO"   /*!< Label */
#define BOARD_RMII0_MDIO_NAME                                       "RMII0_MDIO"   /*!< Identifier name */

/* PORTA13 (number 43), U13[13]/RMII0_RXD_0 */
#define BOARD_RMII0_RXD0_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_RXD0_SIGNAL                                        RMII_RXD0   /*!< ENET signal: RMII_RXD0 */
#define BOARD_RMII0_RXD0_PIN_NAME                                     RMII0_RXD0   /*!< Pin name */
#define BOARD_RMII0_RXD0_LABEL                             "U13[13]/RMII0_RXD_0"   /*!< Label */
#define BOARD_RMII0_RXD0_NAME                                       "RMII0_RXD0"   /*!< Identifier name */

/* PORTA12 (number 42), U13[12]/RMII0_RXD_1 */
#define BOARD_RMII0_RXD1_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_RXD1_SIGNAL                                        RMII_RXD1   /*!< ENET signal: RMII_RXD1 */
#define BOARD_RMII0_RXD1_PIN_NAME                                     RMII0_RXD1   /*!< Pin name */
#define BOARD_RMII0_RXD1_LABEL                             "U13[12]/RMII0_RXD_1"   /*!< Label */
#define BOARD_RMII0_RXD1_NAME                                       "RMII0_RXD1"   /*!< Identifier name */

/* PORTA5 (number 39), U13[17]/RMII0_RXER */
#define BOARD_RMII0_RXER_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_RXER_SIGNAL                                        RMII_RXER   /*!< ENET signal: RMII_RXER */
#define BOARD_RMII0_RXER_PIN_NAME                                     RMII0_RXER   /*!< Pin name */
#define BOARD_RMII0_RXER_LABEL                              "U13[17]/RMII0_RXER"   /*!< Label */
#define BOARD_RMII0_RXER_NAME                                       "RMII0_RXER"   /*!< Identifier name */

/* PORTA16 (number 46), U13[20]/RMII0_TXD0 */
#define BOARD_RMII0_TXD0_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_TXD0_SIGNAL                                        RMII_TXD0   /*!< ENET signal: RMII_TXD0 */
#define BOARD_RMII0_TXD0_PIN_NAME                                     RMII0_TXD0   /*!< Pin name */
#define BOARD_RMII0_TXD0_LABEL                              "U13[20]/RMII0_TXD0"   /*!< Label */
#define BOARD_RMII0_TXD0_NAME                                       "RMII0_TXD0"   /*!< Identifier name */

/* PORTA17 (number 47), U13[21]/RMII0_TXD1 */
#define BOARD_RMII0_TXD1_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_TXD1_SIGNAL                                        RMII_TXD1   /*!< ENET signal: RMII_TXD1 */
#define BOARD_RMII0_TXD1_PIN_NAME                                     RMII0_TXD1   /*!< Pin name */
#define BOARD_RMII0_TXD1_LABEL                              "U13[21]/RMII0_TXD1"   /*!< Label */
#define BOARD_RMII0_TXD1_NAME                                       "RMII0_TXD1"   /*!< Identifier name */

/* PORTA15 (number 45), U13[19]/RMII0_TXEN */
#define BOARD_RMII0_TXEN_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII0_TXEN_SIGNAL                                        RMII_TXEN   /*!< ENET signal: RMII_TXEN */
#define BOARD_RMII0_TXEN_PIN_NAME                                     RMII0_TXEN   /*!< Pin name */
#define BOARD_RMII0_TXEN_LABEL                              "U13[19]/RMII0_TXEN"   /*!< Label */
#define BOARD_RMII0_TXEN_NAME                                       "RMII0_TXEN"   /*!< Identifier name */

/* PORTA14 (number 44), U13[15]/RMII0_CRS_DV */
#define BOARD_RMII0_CRS_DV_PERIPHERAL                                       ENET   /*!< Device name: ENET */
#define BOARD_RMII0_CRS_DV_SIGNAL                                    RMII_CRS_DV   /*!< ENET signal: RMII_CRS_DV */
#define BOARD_RMII0_CRS_DV_PIN_NAME                                 RMII0_CRS_DV   /*!< Pin name */
#define BOARD_RMII0_CRS_DV_LABEL                          "U13[15]/RMII0_CRS_DV"   /*!< Label */
#define BOARD_RMII0_CRS_DV_NAME                                   "RMII0_CRS_DV"   /*!< Identifier name */

/* PORTA18 (number 50), U13[16]/RMII_RXCLK */
#define BOARD_RMII_RXCLK_PERIPHERAL                                         ENET   /*!< Device name: ENET */
#define BOARD_RMII_RXCLK_SIGNAL                                       RMII_CLKIN   /*!< ENET signal: RMII_CLKIN */
#define BOARD_RMII_RXCLK_PIN_NAME                                         EXTAL0   /*!< Pin name */
#define BOARD_RMII_RXCLK_LABEL                              "U13[16]/RMII_RXCLK"   /*!< Label */
#define BOARD_RMII_RXCLK_NAME                                       "RMII_RXCLK"   /*!< Identifier name */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitENET(void);

/* PORTE0 (number 1), J15[P8]/SDHC0_D1 */
#define BOARD_SDHC0_D1_PERIPHERAL                                           SDHC   /*!< Device name: SDHC */
#define BOARD_SDHC0_D1_SIGNAL                                               DATA   /*!< SDHC signal: DATA */
#define BOARD_SDHC0_D1_CHANNEL                                                 1   /*!< SDHC DATA channel: 1 */
#define BOARD_SDHC0_D1_PIN_NAME                                         SDHC0_D1   /*!< Pin name */
#define BOARD_SDHC0_D1_LABEL                                  "J15[P8]/SDHC0_D1"   /*!< Label */
#define BOARD_SDHC0_D1_NAME                                           "SDHC0_D1"   /*!< Identifier name */

/* PORTE1 (number 2), J15[P7]/SDHC0_D0 */
#define BOARD_SDHC0_D0_PERIPHERAL                                           SDHC   /*!< Device name: SDHC */
#define BOARD_SDHC0_D0_SIGNAL                                               DATA   /*!< SDHC signal: DATA */
#define BOARD_SDHC0_D0_CHANNEL                                                 0   /*!< SDHC DATA channel: 0 */
#define BOARD_SDHC0_D0_PIN_NAME                                         SDHC0_D0   /*!< Pin name */
#define BOARD_SDHC0_D0_LABEL                                  "J15[P7]/SDHC0_D0"   /*!< Label */
#define BOARD_SDHC0_D0_NAME                                           "SDHC0_D0"   /*!< Identifier name */

/* PORTE2 (number 3), J15[P5]/SDHC0_DCLK */
#define BOARD_SDHC0_DCLK_PERIPHERAL                                         SDHC   /*!< Device name: SDHC */
#define BOARD_SDHC0_DCLK_SIGNAL                                             DCLK   /*!< SDHC signal: DCLK */
#define BOARD_SDHC0_DCLK_PIN_NAME                                     SDHC0_DCLK   /*!< Pin name */
#define BOARD_SDHC0_DCLK_LABEL                              "J15[P5]/SDHC0_DCLK"   /*!< Label */
#define BOARD_SDHC0_DCLK_NAME                                       "SDHC0_DCLK"   /*!< Identifier name */

/* PORTE3 (number 4), J15[P3]/SDHC0_CMD */
#define BOARD_SDHC0_CMD_PERIPHERAL                                          SDHC   /*!< Device name: SDHC */
#define BOARD_SDHC0_CMD_SIGNAL                                               CMD   /*!< SDHC signal: CMD */
#define BOARD_SDHC0_CMD_PIN_NAME                                       SDHC0_CMD   /*!< Pin name */
#define BOARD_SDHC0_CMD_LABEL                                "J15[P3]/SDHC0_CMD"   /*!< Label */
#define BOARD_SDHC0_CMD_NAME                                         "SDHC0_CMD"   /*!< Identifier name */

/* PORTE4 (number 5), J15[P2]/SDHC0_D3 */
#define BOARD_SDHC0_D3_PERIPHERAL                                           SDHC   /*!< Device name: SDHC */
#define BOARD_SDHC0_D3_SIGNAL                                               DATA   /*!< SDHC signal: DATA */
#define BOARD_SDHC0_D3_CHANNEL                                                 3   /*!< SDHC DATA channel: 3 */
#define BOARD_SDHC0_D3_PIN_NAME                                         SDHC0_D3   /*!< Pin name */
#define BOARD_SDHC0_D3_LABEL                                  "J15[P2]/SDHC0_D3"   /*!< Label */
#define BOARD_SDHC0_D3_NAME                                           "SDHC0_D3"   /*!< Identifier name */

/* PORTE5 (number 6), J15[P1]/SDHC0_D2 */
#define BOARD_SDHC0_D2_PERIPHERAL                                           SDHC   /*!< Device name: SDHC */
#define BOARD_SDHC0_D2_SIGNAL                                               DATA   /*!< SDHC signal: DATA */
#define BOARD_SDHC0_D2_CHANNEL                                                 2   /*!< SDHC DATA channel: 2 */
#define BOARD_SDHC0_D2_PIN_NAME                                         SDHC0_D2   /*!< Pin name */
#define BOARD_SDHC0_D2_LABEL                                  "J15[P1]/SDHC0_D2"   /*!< Label */
#define BOARD_SDHC0_D2_NAME                                           "SDHC0_D2"   /*!< Identifier name */

/* PORTE6 (number 7), J15[G1]/SD_CARD_DETECT */
#define BOARD_SDHC_CD_GPIO                                                 GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_SDHC_CD_PORT                                                 PORTE   /*!< PORT device name: PORTE */
#define BOARD_SDHC_CD_GPIO_PIN                                                6U   /*!< PORTE pin index: 6 */
#define BOARD_SDHC_CD_PIN_NAME                                              PTE6   /*!< Pin name */
#define BOARD_SDHC_CD_LABEL                             "J15[G1]/SD_CARD_DETECT"   /*!< Label */
#define BOARD_SDHC_CD_NAME                                             "SDHC_CD"   /*!< Identifier name */
#define BOARD_SDHC_CD_DIRECTION                          kPIN_MUX_DirectionInput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDHC(void);

/* USB0_DP (number 10), J22[3]/K64_MICRO_USB_DP */
#define BOARD_USB_DP_PERIPHERAL                                             USB0   /*!< Device name: USB0 */
#define BOARD_USB_DP_SIGNAL                                                   DP   /*!< USB0 signal: DP */
#define BOARD_USB_DP_PIN_NAME                                            USB0_DP   /*!< Pin name */
#define BOARD_USB_DP_LABEL                             "J22[3]/K64_MICRO_USB_DP"   /*!< Label */
#define BOARD_USB_DP_NAME                                               "USB_DP"   /*!< Identifier name */

/* USB0_DM (number 11), J22[2]/K64_MICRO_USB_DN */
#define BOARD_USB_DM_PERIPHERAL                                             USB0   /*!< Device name: USB0 */
#define BOARD_USB_DM_SIGNAL                                                   DM   /*!< USB0 signal: DM */
#define BOARD_USB_DM_PIN_NAME                                            USB0_DM   /*!< Pin name */
#define BOARD_USB_DM_LABEL                             "J22[2]/K64_MICRO_USB_DN"   /*!< Label */
#define BOARD_USB_DM_NAME                                               "USB_DM"   /*!< Identifier name */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSB(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
