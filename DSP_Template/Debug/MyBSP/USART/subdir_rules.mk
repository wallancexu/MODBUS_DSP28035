################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F:/MODBUS_3.0/Debug/USART.obj: F:/MODBUS_3.0/MyBSP/USART/USART.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/bin/cl2000" -v28 -ml --include_path="F:/MODBUS_3.0/DSP2803x_headers/include" --include_path="F:/MODBUS_3.0/DSP2803x_common/include" --include_path="F:/MODBUS_3.0/MyBSP" --include_path="F:/MODBUS_3.0/DSP_Template/MyBSP/USART" --include_path="F:/MODBUS_3.0/DSP_Template/MyBSP/CRC" --include_path="F:/MODBUS_3.0/OnChip_Driver/Delay" --include_path="/packages/ti/xdais" --include_path="F:/MODBUS_3.0/DSP_Template/OnChip_Driver/TIMER" --include_path="F:/MODBUS_3.0/DSP_Template/MyBSP/LEDS" --include_path="F:/MODBUS_3.0/DSP_Template/MyBSP/MODBUS" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/include" -g --define="_DEBUG" --define="LARGE_MODEL" --display_error_number --diag_warning=225 --diag_wrap=off --obj_directory="F:/MODBUS_3.0/Debug" --preproc_with_compile --preproc_dependency="MyBSP/USART/USART.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


