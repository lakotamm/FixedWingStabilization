/*
 * MASTER_UART_TX_msg.h
 *
 *  Created on: Nov 7, 2017
 *      Author: das
 */

#pragma once

//message ID
typedef enum
{
	TX_MOTOR_ON					= 0U,
	TX_MOTOR_OFF				= 1U,

	TX_ABS_POS					= 2U,
	TX_REL_POS					= 3U,
	TX_ZERO_POS					= 4U,
	TX_MIN_POS					= 5U,
	TX_MAX_POS					= 6U,

	TX_SETTINGS					= 10U,
	TX_WRT_SETTINGS			= 11U,
	TX_USR_STNG					= 12U,
	TX_WRT_USR_STNG			= 13U,
	TX_RST_WNG_ERR			= 14U,

	TX_REQ_FEEDBACK			= 20U,
	TX_REQ_SERVICE_DATA	= 21U,
	TX_REQ_SETTINGS			= 22U,
	TX_REQ_ERRORS				= 23U,
	TX_REQ_DEBUG				= 24U,

	TX_FACTORY_RST			= 30U,
	TX_BOOTLOADER				= 40U,
	TX_SERVIS_MODE 		  = 50U

} TXMessageIDTypeDef;

typedef enum
{
    TX_COMMAND_LENGTH			= 6U,
    TX_POS_LENGTH					= 8U,
    TX_SETTINGS_LENGTH		= 18U,
    TX_USR_STNG_LENGTH		= 26U,
    TX_REQ_MESSAGE_LENGTH	= 8U

} TXMessageLengthTypeDef;

#define TX_HEADER_LENGTH 4
#define TX_USER_STRING_LENGTH 20

#define TX_DEFAULT_ID 0
#define TX_BROADCAST_ID 15

#define MODE_SINGLE_SERVO 0
#define MODE_MULTIPLE_SERVOS 1
//Header
struct TX_Header {
	// 2 bytes
	uint8_t key[2];
	// 1 byte
	unsigned int message_type :8;
	// 1 byte
	unsigned int servo_ID :4;
	unsigned int group_flag :4;

}__attribute__((packed, aligned(2)));

//Write settings to eeprom, Write user string to eeprom, Reset warnings and errors, Enter bootloader, Motor ON, Motor OFF
struct TX_Command {
	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

//Request message - feedback, data, settings, errors, debug
struct TX_Request_Message {
	// 1 byte
	unsigned int unused0 :8;
	// 1 byte
	uint8_t interval;
	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

//Set absolute position, set relative position
struct TX_Position {
	// 2 bytes
	unsigned int unused0 :1;
	int position :15;
	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

//Set settings
struct TX_Settings {
	// 2 bytes
	uint16_t P_setting;
	// 2 bytes
	uint16_t I_setting;
	// 2 bytes
	uint16_t D_setting;
	// 2 bytes
	uint16_t CurLim_setting;
	// 2 bytes
	uint16_t VoltLim_setting;
	// 1 byte
	unsigned int servo_ID :4;
	unsigned int group_ID :4;
	// 1 byte
	unsigned int servo_mode :8;
	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

//Set user string
struct TX_User_String {
	// 20 bytes
	uint8_t User_String[TX_USER_STRING_LENGTH];

	// 2 bytes
	uint16_t checksum;

}__attribute__((packed, aligned(2)));

union TX_Data {
	struct TX_Command command;
	struct TX_Request_Message request_message;
	struct TX_Position position;
	struct TX_Settings settings;
	struct TX_User_String user_string;
} __attribute__((packed, aligned(2)));

struct TX_Messages {
	struct TX_Header header;
	union TX_Data data;
} __attribute__((packed, aligned(2)));

typedef union {
	uint8_t buffer[30];
	struct TX_Messages message;
} TX_transmit_space_t;
