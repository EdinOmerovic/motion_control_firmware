################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
drivers/src/control.obj: ../drivers/src/control.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e/drivers/inc" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/include" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e/hal/inc" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/utilities/tools/f2837xd" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e/drivers/inc" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e/hal/inc" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xd/driverlib" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include/F2837xD_dac.h" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include/F2837xD_device.h" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/include/IQmathLib.h" --advice:performance=all --define=CPU1 --define=_DUAL_HEADERS --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="drivers/src/$(basename $(<F)).d_raw" --obj_directory="drivers/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

drivers/src/%.obj: ../drivers/src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/utilities/tools/f2837xd" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e/drivers/inc" --include_path="C:/Users/Master/Desktop/ccs_extraction/!qepm_e/hal/inc" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include/F2837xD_dac.h" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/device_support/f2837xd/headers/include/F2837xD_device.h" --preinclude="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/math/IQmath/c28/include/IQmathLib.h" --advice:performance=all --define=CPU1 --define=_DUAL_HEADERS --define=_FLASH --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="drivers/src/$(basename $(<F)).d_raw" --obj_directory="drivers/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


