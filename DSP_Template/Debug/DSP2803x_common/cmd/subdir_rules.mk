################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DSP2803x_common/cmd/28035_RAM_lnk.exe: F:/MODBUS_3.0/DSP2803x_common/cmd/28035_RAM_lnk.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/bin/cl2000" -v28 -ml -g --define="_DEBUG" --define="LARGE_MODEL" --display_error_number --diag_warning=225 --diag_wrap=off --obj_directory="F:/MODBUS_3.0/Debug" -z -m"F:/MODBUS_3.0/DSP2803x_common/cmd/Debug/DSP_Template.map" --stack_size=0x200 --warn_sections -i"F:/MODBUS_3.0/DSP_Template" -i"F:/MODBUS_3.0" -i"F:/MODBUS_3.0/DSP2803x_headers/include" -i"F:/MODBUS_3.0/DSP2803x_common/cmd/DSP2803x_headers/include" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_15.12.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="MODBUS_linkInfo.xml" --entry_point=_c_int00 --rom_model -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


