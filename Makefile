EXEC=messageTest

all: $(EXEC)

messageTest: messageTest.o messageParser.o
	$(CC) -o $@ $<

messageParser.o: messageParser.c
messageParser.c: messageParser.h

messageTest.c: messageParser.h
messageTest.o: messageTest.c


clean:
	rm -rf *.o $(EXEC)
