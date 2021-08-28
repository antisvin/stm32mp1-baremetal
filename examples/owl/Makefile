BINARYNAME = main
BUILDDIR = build

OPTFLAG = -O3
#OPTFLAG = -g3

EXTLIBDIR = ../../third-party
SHAREDDIR = ../shared
STM32HALDIR = $(EXTLIBDIR)/STM32MP1xx_HAL_Driver
OWLDIR = $(EXTLIBDIR)/OwlProgram
PATCHSOURCE ?= $(OWLDIR)/PatchSource
LIBSOURCE ?= $(OWLDIR)/LibSource
DSPINC ?= $(OWLDIR)/Libraries/CMSIS/DSP/Include
DSPLIB ?= $(OWLDIR)/Libraries/CMSIS/DSP/Source
DAISYSPDIR = $(EXTLIBDIR)/OwlProgram/Libraries/DaisySP/Source

ifdef FAUST
# options for FAUST compilation
PATCHNAME   ?= $(FAUST)
PATCHCLASS  ?= FaustPatch
PATCHFILE   ?= FaustPatch.hpp
DEPS        += faust
else ifdef HEAVY
# options for Heavy PD compilation
PATCHNAME   ?= $(HEAVY)
PATCHCLASS  ?= HeavyPatch
PATCHFILE   ?= HeavyPatch.hpp
DEPS        += heavy
else ifdef GEN
# options for Max/MSP Gen compilation
PATCHNAME   ?= $(GEN)
PATCHCLASS  ?= GenPatch
PATCHFILE   ?= GenPatch.hpp
DEPS        += gen
else ifdef MAXIMILIAN
# options for Maximilian compilation
PATCHNAME   ?= $(MAXIMILIAN)
PATCHCLASS  ?= MaximilianPatch
PATCHFILE   ?= MaximilianPatch.hpp
DEPS        += maximilian
else ifdef SOUL
# options for SOUL patch compilation
PATCHNAME   ?= $(SOUL)
PATCHCLASS  ?= SoulPatch
PATCHFILE   ?= SoulPatch.hpp
SOULCLASS   ?= $(SOUL)
SOULFILE    ?= $(SOUL).soulpatch
SOULHPP     ?= $(SOUL).hpp
DEPS        += soul
else ifdef TEST
PATCHNAME   = $(TEST)
PATCHCLASS  = $(PATCHNAME)Patch
PATCHFILE   = $(PATCHNAME)Patch.hpp
else
# options for C++ compilation
PATCHNAME   ?= "Template"
PATCHCLASS  ?= $(PATCHNAME)Patch
PATCHFILE   ?= $(PATCHNAME)Patch.hpp
endif

PATCHIN     ?= 2
PATCHOUT    ?= 2
SLOT        ?= 0
OWLDEVICE   ?= OWL-*
BUILD       ?= $(BUILDROOT)/Build
PATCHSOURCE ?= $(BUILDROOT)/PatchSource

mdrivlibdir = lib/mdrivlib

SOURCES = main.cc \
		  mmu_ca7.c \
		  startup.s \
		  lib/util/math_tables.cc \
		  $(mdrivlibdir)/drivers/i2c.cc \
		  $(mdrivlibdir)/drivers/pin.cc \
		  $(mdrivlibdir)/drivers/codec_CS42L51.cc \
		  $(mdrivlibdir)/target/stm32mp1/drivers/sai_tdm.cc \
		  $(mdrivlibdir)/target/stm32mp1_ca7/drivers/cycle_counter.cc \
		  $(mdrivlibdir)/target/stm32mp1_ca7/drivers/hal_handlers.cc \
		  $(mdrivlibdir)/target/stm32mp1_ca7/drivers/interrupt_handler.cc \
		  $(mdrivlibdir)/target/stm32mp1_ca7/boot/irq_ctrl.c \
		  $(SHAREDDIR)/system/system_ca7.c \
		  $(SHAREDDIR)/system/libc_stub.c \
		  $(SHAREDDIR)/system/libcpp_stub.cc \
		  $(SHAREDDIR)/system/new.cc \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_cortex.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_dma.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_dma_ex.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_i2c.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_i2c_ex.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_sai.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_rcc.c \
		  $(STM32HALDIR)/Src/stm32mp1xx_hal_rcc_ex.c \
		  $(OWLDIR)/Source/PatchProcessor.cpp \
		  $(OWLDIR)/Source/Patch.cpp \
		  $(OWLDIR)/Source/system_tables.cpp \
		  $(OWLDIR)/LibSource/FloatArray.cpp \
		  $(OWLDIR)/LibSource/basicmaths.c

# $(EXTLIBDIR)/DaisySP/Source/Control/adenv.cpp \
		  # $(EXTLIBDIR)/DaisySP/Source/Effects/reverbsc.cpp \
		  # $(EXTLIBDIR)/DaisySP/Source/Synthesis/oscillator.cpp \
		  # $(EXTLIBDIR)/DaisySP/Source/Utility/metro.cpp \
		  # $(STM32HALDIR)/Src/stm32mp1xx_ll_gpio.c \
		  # $(mdrivlibdir)/target/stm32mp1_ca7/boot/startup_ca7.s \

INCLUDES = -I. \
		   -I.. \
		   -Ilib \
		   -Iutil \
		   -I$(mdrivlibdir) \
		   -I$(mdrivlibdir)/drivers \
		   -I$(mdrivlibdir)/target/stm32mp1 \
		   -I$(mdrivlibdir)/target/stm32mp1_ca7 \
		   -I$(mdrivlibdir)/target/stm32mp1/drivers \
		   -I$(mdrivlibdir)/target/stm32mp1_ca7/boot \
		   -I$(mdrivlibdir)/target/stm32mp1_ca7/drivers \
		   -I$(STM32HALDIR)/Inc \
		   -I$(EXTLIBDIR)/gcem/include \
		   -I$(EXTLIBDIR)/CMSIS/Core_A/Include \
		   -I$(EXTLIBDIR)/CMSIS/Device/ST/STM32MP1xx/Include \
		   -I$(OWLDIR)/Libraries \
		   -I$(OWLDIR)/Source \
		   -I$(BUILDDIR) \
		   -I$(PATCHSOURCE) \
		   -I$(LIBSOURCE) \
		   -I$(DSPINC) \
		   -I$(DAISYSP) \
		   -I$(DAISYSP)/Control \
		   -I$(DAISYSP)/Drums \
		   -I$(DAISYSP)/Dynamics \
		   -I$(DAISYSP)/Effects \
		   -I$(DAISYSP)/Filters \
		   -I$(DAISYSP)/Noise \
		   -I$(DAISYSP)/PhysicalModeling \
		   -I$(DAISYSP)/Synthesis \
		   -I$(DAISYSP)/Utility

EXTRACFLAGS = -DUSE_HAL_DRIVER \
			  -DUSE_FULL_LL_DRIVER \
			  -DSTM32MP157Cxx \
			  -DSTM32MP1 \
			  -DCORE_CA7 \
				-mfpu=neon-vfpv4 \
				-mfloat-abi=hard \
				-mthumb-interwork \
				-mtune=cortex-a7 \
				-mno-unaligned-access \
				-funsafe-math-optimizations \
				-mvectorize-with-neon-quad \
				-fsingle-precision-constant \
				-DARM_CORTEX \
				-DARM_MATH_NEON \
				-DEXTERNAL_SRAM

EXTRACXXFLAGS =  \

include $(OWLDIR)/libs.mk
SOURCES += $(C_SRC)
SOURCES += $(S_SRC)

ifdef HEAVY
EXTRACXXFLAGS += -D__unix__ -DHV_SIMD_NONE
endif
ifdef FAUST
ifneq (,$(findstring soundfile,$(FAUSTOPTS)))
EXTRACXXFLAGS += -DSOUNDFILE
endif
endif

.FORCE:
	@mkdir -p $(BUILDDIR)/Source
	@mkdir -p $(BUILDDIR)/web
	@mkdir -p $(BUILDDIR)/Test

$(BUILDDIR)/registerpatch.cpp: .FORCE
	@echo Building patch $(PATCHNAME)
	@echo "#include \"$(PATCHFILE)\"" > $(@:.cpp=.h)
	@echo "REGISTER_PATCH($(PATCHCLASS), \"$(PATCHNAME)\", $(PATCHIN), $(PATCHOUT));" > $@


include ../shared/makefile-common.mk

DEPS += $(BUILDDIR)/registerpatch.h

