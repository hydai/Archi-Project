SRC	= main.c
HEADER	= main.h
TARGET	= pipeline
CFLAGS	= -Wall -O0 -g
EXE	= ./pipeline
RM	= -rm -rf
DIFF	= diff -q
OPEN_TEST	= ./open_testcase
HIDDEN_TEST	= ./hidden_testcase
VALID_TEST	= ./valid_testcase

all		: $(TARGET)

$(TARGET)	: $(SRC) $(HEADER)
	cc $(SRC) $(CFLAGS) -o $(TARGET)

test		: $(TARGET) open_testcase hidden_testcase valid_testcase

open_testcase	: error fib_r pipeline1 pipeline2 pipeline3

error		:
	@echo "Testing 'error'"
	@cp $(OPEN_TEST)/error/*.bin .
	@$(EXE)
	@-$(DIFF) $(OPEN_TEST)/error/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(OPEN_TEST)/error/error_dump.rpt error_dump.rpt

fib_r		:
	@echo "Testing 'fib_r'"
	@cp $(OPEN_TEST)/fib_r/*.bin .
	@$(EXE)
	@-$(DIFF) $(OPEN_TEST)/fib_r/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(OPEN_TEST)/fib_r/error_dump.rpt error_dump.rpt

pipeline1	:
	@echo "Testing 'pipeline1'"
	@cp $(OPEN_TEST)/pipeline1/*.bin .
	@$(EXE)
	@-$(DIFF) $(OPEN_TEST)/pipeline1/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(OPEN_TEST)/pipeline1/error_dump.rpt error_dump.rpt

pipeline2	:
	@echo "Testing 'pipeline2'"
	@cp $(OPEN_TEST)/pipeline2/*.bin .
	@$(EXE)
	@-$(DIFF) $(OPEN_TEST)/pipeline2/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(OPEN_TEST)/pipeline2/error_dump.rpt error_dump.rpt

pipeline3	:
	@echo "Testing 'pipeline3'"
	@cp $(OPEN_TEST)/pipeline3/*.bin .
	@$(EXE)
	@-$(DIFF) $(OPEN_TEST)/pipeline3/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(OPEN_TEST)/pipeline3/error_dump.rpt error_dump.rpt

hidden_testcase	: bubble_sort divide pipeline4 pipeline5

bubble_sort	:
	@echo "Testing 'bubble_sort'"
	@cp $(HIDDEN_TEST)/bubble_sort/*.bin .
	@$(EXE)
	@-$(DIFF) $(HIDDEN_TEST)/bubble_sort/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(HIDDEN_TEST)/bubble_sort/error_dump.rpt error_dump.rpt

divide		:
	@echo "Testing 'divide'"
	@cp $(HIDDEN_TEST)/divide/*.bin .
	@$(EXE)
	@-$(DIFF) $(HIDDEN_TEST)/divide/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(HIDDEN_TEST)/divide/error_dump.rpt error_dump.rpt

pipeline4	:
	@echo "Testing 'pipeline4'"
	@cp $(HIDDEN_TEST)/pipeline4/*.bin .
	@$(EXE)
	@-$(DIFF) $(HIDDEN_TEST)/pipeline4/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(HIDDEN_TEST)/pipeline4/error_dump.rpt error_dump.rpt

pipeline5	:
	@echo "Testing 'pipeline5'"
	@cp $(HIDDEN_TEST)/pipeline5/*.bin .
	@$(EXE)
	@-$(DIFF) $(HIDDEN_TEST)/pipeline5/snapshot.rpt snapshot.rpt
	@-$(DIFF) $(HIDDEN_TEST)/pipeline5/error_dump.rpt error_dump.rpt

valid_testcase	: student_test

student_test	:
	@-for dir in $(VALID_TEST)/*/ ;                  \
	do                                               \
	  echo "Testing '$${dir}'" ;                     \
	  cp $${dir}*.bin . ;                            \
	  $(EXE) ;                                       \
	  diff -q $${dir}snapshot.rpt snapshot.rpt ;     \
	  diff -q $${dir}error_dump.rpt error_dump.rpt ; \
	done

clean		:
	$(RM) $(TARGET) $(TARGET).core
