CC = g++
CFLAGS = -Wall

TARGET_NAME = program

BUILD_DIR = ./build
MARKER_DIR = markers
WRITER_DIR = writers
TEST_DIR = testing

MARKERS = Marker MarkerRegistry StubMarker SimpleMarker
WRITERS = ResultWriter WriterRegistry DebugLogger
TEST_COMPS = TestResult SourceLocation

DEPS = $(MARKERS:%=$(BUILD_DIR)/$(MARKER_DIR)/%.o) $(WRITERS:%=$(BUILD_DIR)/$(WRITER_DIR)/%.o) $(TEST_COMPS:%=$(BUILD_DIR)/$(TEST_DIR)/%.o)

$(TARGET_NAME): $(BUILD_DIR)/$(TARGET_NAME).o $(DEPS)
	$(CC) $(CFLAGS) -o program $^

$(BUILD_DIR)/%.o: ./%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fv $(TARGET_NAME)
	rm -fv build/*.o
	rm -fv build/$(MARKER_DIR)/*.o
	rm -fv build/$(WRITER_DIR)/*.o
