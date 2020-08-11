CC			= gcc
ARCHIVE			= ar

CC_FLAGS		= -g -c -Wall -Iinclude
ARCHIVE_FLAGS		= ru

LIBS			 = libmongoose.a libjson.a libsqlite3.a

%.o			: %.c
			  @echo [CC] $@
			  @$(CC) $(CC_FLAGS) $<

all			: $(LIBS)

libsqlite3.a		: sqlite3.o
			  @echo [AR] libsqlite3.a
			  @$(ARCHIVE) $(ARCHIVE_FLAGS) $@ $^

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

