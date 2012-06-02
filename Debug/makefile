################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Change this to suit your Boost installation:
BOOST_PATH := /home/mike/boost/boost_1_49_0

RM := rm -rf

# All of the sources participating in the build are defined here
CPP_SRCS += \
../SctpServer.cpp \
../server1.cpp 

OBJS += \
./SctpServer.o \
./server1.o 

CPP_DEPS += \
./SctpServer.d \
./server1.d 

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: sctp_asio

# Tool invocations
sctp_asio: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L$(BOOST_PATH)/stage/lib -o"sctp_asio" $(OBJS) -lpthread -lsctp -lboost_thread -lboost_date_time -lboost_system
	@echo 'Finished building target: $@'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I$(BOOST_PATH) -I.. -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) sctp_asio
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
