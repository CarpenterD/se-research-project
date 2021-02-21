CC = g++
CFLAGS = -Wall -DDEBUG

TARGET_NAME = program

BUILD_DIR = ./build
MARKER_DIR = markers
WRITER_DIR = writers
TEST_DIR = testing
UTIL_DIR = utils

ALL_DIRS = $(MARKER_DIR) $(WRITER_DIR) $(TEST_DIR) $(UTIL_DIR)

MARKERS = Marker MarkerRegistry StubMarker SimpleMarker
WRITERS = ResultWriter WriterRegistry DebugLogger
TEST_COMPS = SourceLocation
UTILS = ArgumentParser FileIO

DEPS = $(MARKERS:%=$(BUILD_DIR)/$(MARKER_DIR)/%.o) $(WRITERS:%=$(BUILD_DIR)/$(WRITER_DIR)/%.o) $(TEST_COMPS:%=$(BUILD_DIR)/$(TEST_DIR)/%.o) $(UTILS:%=$(BUILD_DIR)/$(UTIL_DIR)/%.o)

$(TARGET_NAME): $(BUILD_DIR)/$(TARGET_NAME).o $(DEPS)
	$(CC) $(CFLAGS) -o program $^

$(BUILD_DIR)/%.o: ./%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -pv $(ALL_DIRS:%=$(BUILD_DIR)/%)

clean:
	rm -fv $(TARGET_NAME)
	rm -frv $(BUILD_DIR)
