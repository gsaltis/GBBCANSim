CC			= gcc
ARCHIVE			= ar

CC_FLAGS		= -g -c -Iinclude
ARCHIVE_FLAGS		= r

LIBS			 = libmongoose.a libjson.a

%.o			: %.c
			  @echo [CC] $@
			  @$(CC) $(CC_FLAGS) $<

all			: $(LIBS)

libmongoose.a		: mongoose.o
			  @echo [AR] libmongoose.a
			  @$(ARCHIVE) $(ARCHIVE_FLAGS) $@ $^

libjson.a		: json.o
			  @echo [AR] libjson.a
			  @$(ARCHIVE) $(ARCHIVE_FLAGS) $@ $^


.PHONY			: junkclean
junkclean		: 
			  rm -rf $(wildcard *~)

.PHONY			: clean
clean			: junkclean
			  rm -rf $(wildcard *.o *.a)


