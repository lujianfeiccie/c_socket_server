LOCAL_PATH :=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE :=phone_server
LOCAL_SRC_FILES := \
	main.c \
	event/input.c \
	event/suinput.c \
	socket/socketserver.c \
	util/othertool.c
LOCAL_C_INCLUDES := \
	event/input.h \
	event/suinput.h \
	event/uinput.h \
	socket/socketserver.h \
	util/othertool.h 
include $(BUILD_EXECUTABLE)
