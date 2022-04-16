################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F:/MODBUS_3.0/Debug/DSP2803x_GlobalVariableDefs.obj: F:/MODBUS_3.0/DSP2803x_headers/source/DSP2803x_GlobalVariableDefs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/bin/cl2000" -v28 -ml --include_path="/packages/ti/xdais" --include_path="F:/MODBUS_3.0/DSP2803x_headers/include" --include_path="F:/MODBUS_3.0/DSP2803x_common/include" --include_path="F:/MODBUS_3.0/DSP2803x_headers/DSP2803x_headers/include" --include_path="F:/MODBUS_3.0/DSP2803x_headers/DSP2803x_common/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/include" -g --define="_DEBUG" --define="LARGE_MODEL" --display_error_number --diag_warning=225 --diag_wrap=off --obj_directory="F:/MODBUS_3.0/Debug" --preproc_with_compile --preproc_dependency="DSP2803x_headers/source/DSP2803x_GlobalVariableDefs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


