# List of the files need for the servo control
SERVO_CONTROL = ${APPS}/servo_control

CPPSRC += ${SERVO_CONTROL}/servo_control.cpp \
  				${SERVO_CONTROL}/UART_servo/UART_hardware.cpp \
				 	${SERVO_CONTROL}/UART_servo/UART_process_RX.cpp \
					${SERVO_CONTROL}/UART_servo/UART_process_TX.cpp

INCDIR += ${SERVO_CONTROL}\
					${SERVO_CONTROL}/UART_servo
