# List of the files need for the FLIGHT_CONTROL
AIRPLANE_FLIGHT_CONTROL = ${APPS}/airplane_flight_control

CPPSRC += ${AIRPLANE_FLIGHT_CONTROL}/airplane_flight_control.cpp \
		  ${AIRPLANE_FLIGHT_CONTROL}/control_input.cpp \
		  ${AIRPLANE_FLIGHT_CONTROL}/control_output.cpp

INCDIR += ${AIRPLANE_FLIGHT_CONTROL}/
