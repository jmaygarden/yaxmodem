BUILD_DIR = build

SRCS = \
       src/xmodem.c

INCS = \
       -Iinclude

CFLAGS = $(INCS) -Wall -Werror -MMD -MP -MF"$(@:%.o=%.d)"

OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))

all: $(OBJS) $(BUILD_DIR)/ptytest

$(BUILD_DIR)/ptytest: test/ptytest.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)

