# List of the files need for the AIRPLANE_ORIENTATION_CONTROL
AIRPLANE_ORIENTATION_CONTROL = ${APPS}/airplane_orientation_control

CPPSRC += ${AIRPLANE_ORIENTATION_CONTROL}/airplane_orientation_control.cpp \
		  ${AIRPLANE_ORIENTATION_CONTROL}/input_output.cpp \
		  ${AIRPLANE_ORIENTATION_CONTROL}/angular_rate_control.cpp \
		  ${AIRPLANE_ORIENTATION_CONTROL}/orientation_control.cpp

INCDIR += ${AIRPLANE_ORIENTATION_CONTROL}/
