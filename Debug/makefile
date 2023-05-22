################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./helpers.obj" \
"./main.obj" \
"./bsp/F2837xD_Adc.obj" \
"./bsp/F2837xD_CodeStartBranch.obj" \
"./bsp/F2837xD_CpuTimers.obj" \
"./bsp/F2837xD_DefaultISR.obj" \
"./bsp/F2837xD_EPwm.obj" \
"./bsp/F2837xD_EQep.obj" \
"./bsp/F2837xD_GlobalVariableDefs.obj" \
"./bsp/F2837xD_Gpio.obj" \
"./bsp/F2837xD_Ipc.obj" \
"./bsp/F2837xD_PieCtrl.obj" \
"./bsp/F2837xD_PieVect.obj" \
"./bsp/F2837xD_SysCtrl.obj" \
"./bsp/F2837xD_usDelay.obj" \
"./bsp/sci.obj" \
"./bsp/scistdio.obj" \
"./bsp/sysctl.obj" \
"./drivers/src/control.obj" \
"./drivers/src/encoder.obj" \
"./drivers/src/lp_filter.obj" \
"./drivers/src/motor.obj" \
"./drivers/src/trajectory.obj" \
"./hal/src/adc_hal.obj" \
"./hal/src/dac_hal.obj" \
"./hal/src/encoder_hal.obj" \
"./hal/src/serial.obj" \
"../2837x_FLASH_lnk_cpu1.cmd" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/lib/IQmath_fpu32.lib" \
-l"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/lib/rts2800_fpu32.lib" \
-l"C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/cmd/F2837xD_Headers_nonBIOS_cpu1.cmd" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include bsp/subdir_vars.mk
-include drivers/src/subdir_vars.mk
-include hal/src/subdir_vars.mk
-include subdir_rules.mk
-include bsp/subdir_rules.mk
-include drivers/src/subdir_rules.mk
-include hal/src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
qepm_extraction.out \

EXE_OUTPUTS__QUOTED += \
"qepm_extraction.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "qepm_extraction.out"

# Tool invocations
qepm_extraction.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include/F2837xD_dac.h" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include/F2837xD_device.h" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/include/IQmathLib.h" --advice:performance=all --define=CPU1 --define=_DUAL_HEADERS --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"qepm_extraction.map" --stack_size=0x200 --warn_sections -i"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/lib" -i"C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/common/cmd" -i"C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/cmd" -i"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="qepm_extraction_linkInfo.xml" --rom_model -o "qepm_extraction.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "helpers.obj" "main.obj" "bsp\F2837xD_Adc.obj" "bsp\F2837xD_CodeStartBranch.obj" "bsp\F2837xD_CpuTimers.obj" "bsp\F2837xD_DefaultISR.obj" "bsp\F2837xD_EPwm.obj" "bsp\F2837xD_EQep.obj" "bsp\F2837xD_GlobalVariableDefs.obj" "bsp\F2837xD_Gpio.obj" "bsp\F2837xD_Ipc.obj" "bsp\F2837xD_PieCtrl.obj" "bsp\F2837xD_PieVect.obj" "bsp\F2837xD_SysCtrl.obj" "bsp\F2837xD_usDelay.obj" "bsp\sci.obj" "bsp\scistdio.obj" "bsp\sysctl.obj" "drivers\src\control.obj" "drivers\src\encoder.obj" "drivers\src\lp_filter.obj" "drivers\src\motor.obj" "drivers\src\trajectory.obj" "hal\src\adc_hal.obj" "hal\src\dac_hal.obj" "hal\src\encoder_hal.obj" "hal\src\serial.obj" 
	-$(RM) "helpers.d" "main.d" "bsp\F2837xD_Adc.d" "bsp\F2837xD_CpuTimers.d" "bsp\F2837xD_DefaultISR.d" "bsp\F2837xD_EPwm.d" "bsp\F2837xD_EQep.d" "bsp\F2837xD_GlobalVariableDefs.d" "bsp\F2837xD_Gpio.d" "bsp\F2837xD_Ipc.d" "bsp\F2837xD_PieCtrl.d" "bsp\F2837xD_PieVect.d" "bsp\F2837xD_SysCtrl.d" "bsp\sci.d" "bsp\scistdio.d" "bsp\sysctl.d" "drivers\src\control.d" "drivers\src\encoder.d" "drivers\src\lp_filter.d" "drivers\src\motor.d" "drivers\src\trajectory.d" "hal\src\adc_hal.d" "hal\src\dac_hal.d" "hal\src\encoder_hal.d" "hal\src\serial.d" 
	-$(RM) "bsp\F2837xD_CodeStartBranch.d" "bsp\F2837xD_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

