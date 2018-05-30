/*
 * MASTER_UART_RX_msg.h
 *
 *  Created on: Nov 7, 2017
 *      Author: das
 */
#pragma once

/**
  * RX message ID definition
  */

typedef enum
{
	RX_FEEDBACK				= 0U,
	RX_SERVICE_DATA		= 1U,
	RX_SETTINGS				= 2U,
	RX_ERRORS					= 3U,
	RX_DEBUG					= 4U
} RXMessageIDTypeDef;

/**
  * message length definition
  */
typedef enum
{
	RX_FEEDBACK_LENGTH			= 16U,
	RX_SERVICE_DATA_LENGTH	= 42U,
	RX_SETTINGS_LENGTH			= 30U,
	RX_ERRORS_LENGTH				= 14U,
	RX_DEBUG_LENGTH					= 28U
} RXMessageLengthTypeDef;

#define RX_HEADER_LENGTH 4
#define RX_USER_STRING_LENGTH 20

struct RX_Header {
	// 2 bytes
	uint8_t key[2];

	// 1 byte
	unsigned int message_type :8;
	// 1 byte
	unsigned int motor_state :1;
	unsigned int warnings :1;
	unsigned int errors :1;
	unsigned int permanent_errors :1;
	unsigned int servo_ID :4;

} __attribute__((packed, aligned(2)));

struct RX_Feedback {
	// 2 bytes
	unsigned int unused0 :1;
	int position :15;
	// 2 bytes
	unsigned int unused1 :1;
	int pos_error :15;
	// 2 bytes
	unsigned int unused2 :1;
	int torque :15;
	// 2 bytes
	unsigned int unused3 :1;
	int speed :15;
	// 2 bytes
	unsigned int unused4 :1;
	int temperature :15;

	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

struct RX_Service_data {
	// 4 bytes
	uint32_t EL_serial_number;
	// 20 bytes
	uint8_t User_String[RX_USER_STRING_LENGTH];
	// 4 bytes
	uint32_t ME_serial_number;
	// 4 bytes
	uint32_t run_time;
	// 2 bytes
	uint16_t sw_version;
	// 2 bytes
	uint16_t servo_wear;

	// 2 bytes
	uint16_t checksum;

} __attribute__((packed, aligned(2)));

struct RX_Settings {
	// 2 bytes
	uint16_t P_setting;
	// 2 bytes
	uint16_t I_setting;
	// 2 bytes
	uint16_t D_setting;
	// 2 bytes
	unsigned int unused0 :1;
	int zero_position :15;
	// 2 bytes
	unsigned int unused1 :1;
	int min_position :15;
	// 2 bytes
	unsigned int unused2 :1;
	int max_position :15;
	// 2 bytes
	uint16_t CurLim_setting;
	// 2 bytes
	uint16_t VoltLim_setting;
	// 1 byte
	unsigned int servo_ID :4;
	unsigned int group_ID :4;
	// 1 byte
	unsigned int servo_mode :8;
	// 1 byte
	uint8_t feedback_interval;
	// 1 byte
	uint8_t data_interval;
	// 1 byte
	uint8_t settings_interval;
	// 1 byte
  uint8_t	errors_interval;
	// 1 byte
	uint8_t debug_interval;
	// 1 byte
	unsigned int unused3 :8;
	// 2 bytes
	uint16_t checksum;

} __attribute__((packed, aligned(2)));

struct RX_Errors {
	// 1 byte
	uint8_t warnings;
	// 1 byte
	uint8_t errors;
	// 1 byte
	uint8_t permanent_errors;
	// 1 byte
	uint8_t service_intervals;
	// 2 bytes
	uint16_t eeprom_errors;
	// 2 bytes
	uint16_t uart_errors;

	// 2 bytes
	uint16_t checksum;

} __attribute__((packed, aligned(2)));

struct RX_Debug {
	// 2 bytes
	unsigned int unused0 :1;
	int raw_position :15;
	// 2 bytes
	unsigned int unused1 :1;
	int raw_setpoint :15;
	// 2 bytes
	uint16_t input_voltage;
	// 2 bytes
	uint16_t output_voltage;
	// 2 bytes
	unsigned int unused2 :1;
	int motor_voltage :15;
 	// 2 bytes
	unsigned int unused3 :1;
	int phase_current :15;
	// 2 bytes
	uint16_t output_power;
	// 2 bytes
	uint16_t current_consumption;
	// 2 bytes
	uint16_t power_consumption;
	// 2 bytes
	uint16_t consumed_current;
	// 2 bytes
	uint16_t consumed_power;

	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

union RX_Data {
	struct RX_Feedback feedback;
	struct RX_Service_data service_data;
	struct RX_Settings settings;
	struct RX_Errors errors;
	struct RX_Debug debug;
} __attribute__((packed, aligned(2)));

struct RX_Messages {
	struct RX_Header header;
	union RX_Data data;
} __attribute__((packed, aligned(2)));

typedef union {
	uint8_t buffer[50];
	struct RX_Messages message;
} RX_receive_space_t;
