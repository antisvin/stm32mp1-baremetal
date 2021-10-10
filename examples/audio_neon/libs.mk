$(shell mkdir -p $(BUILDDIR))

RM ?= rm
LIBDIR ?= lib
LIBMATHNEON = $(LIBDIR)/math-neon
LIBNE10 = $(LIBDIR)/ne10

NEONLIBS := $(LIBDIR)/libmathneon.a $(LIBDIR)/libne10.a
MN_SOURCES := $(filter-out  $(LIBMATHNEON)/math_debug.c,$(wildcard $(LIBMATHNEON)/math_*.c))
MN_OBJS := $(addprefix $(OBJDIR)/,$(notdir $(MN_SOURCES:.c=.o)))
MN_DEPS := $(addprefix $(OBJDIR)/,$(notdir $(MN_SOURCES:.c=.d)))


NE_IGNORES := $(LIBNE10)/modules/dsp/NE10_fft_float32.neon.s \
	$(LIBNE10)/modules/dsp/NE10_fft_float32.neon.c \

# FFT - look mutually exclusive with intrinsics versions, intrinsics also implement RFFT functions

NE_CSOURCES := $(filter-out $(NE_IGNORES),$(wildcard $(LIBNE10)/modules/*/*.c))
NE_SSOURCES := $(filter-out $(NE_IGNORES),$(wildcard $(LIBNE10)/modules/*/*.s))
NE_CSOURCES += $(wildcard $(LIBNE10)/modules/*.c)
NE_SSOURCES += $(wildcard $(LIBNE10)/common/*.s)
NE_CSOURCES += $(wildcard $(LIBNE10)/common/*.c)
NE_OBJS := $(addprefix $(OBJDIR)/,$(notdir $(NE_CSOURCES:.c=.o)))
NE_OBJS += $(addprefix $(OBJDIR)/,$(notdir $(NE_SSOURCES:.s=.o)))
NE_DEPS := $(addprefix $(OBJDIR)/,$(notdir $(NE_CSOURCES:.c=.d)))

INCLUDES += -I$(LIBMATHNEON)
INCLUDES += -I$(LIBNE10)/inc
INCLUDES += -I$(LIBNE10)/common
INCLUDES += -I$(LIBNE10)/modules/math

vpath %.c $(sort $(dir $(MN_SOURCES)))
vpath %.c $(sort $(dir $(NE_CSOURCES)))
vpath %.s $(sort $(dir $(NE_SSOURCES)))

$(LIBDIR)/libmathneon.a: $(MN_OBJS)
	echo $(MN_OBJS)
	@nm -a $(MN_OBJS) | grep main && { echo "Error: there is a main() among the library objects" >&2 ; exit 1; } || exit 0
	ar rcs $@ $^

$(LIBDIR)/libne10.a: $(NE_OBJS)
	@nm -a $(NE_OBJS) | grep main && { echo "Error: there is a main() among the library objects" >&2 ; exit 1; } || exit 0
	ar rcs $@ $^

OPTFLAGS += -Wa,-mimplicit-it=thumb

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	$(CC) -c $(OPTFLAG) $(CFLAGS) -std=gnu99 $< -o $@
	$(CC) -MM -MT"$@" $(OPTFLAG) $(CFLAGS)  -std=gnu99  $< > $(@:.o=.d)

$(OBJDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	echo $(NE_SSOURCES)
	$(info Building $< at $(OPTFLAG))
	@$(AS) $(AFLAGS) $(INCLUDES) $< -o $@

libs: $(NEONLIBS)

clean_libs:
	$(RM) $(LIBDIR)/*.a

