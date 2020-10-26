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


.PHONY			: clean
clean			: 
			  rm -rf $(wildcard *~)

.PHONY			: veryclean
veryclean		: clean
			  rm -rf $(wildcard *.o *.a)

