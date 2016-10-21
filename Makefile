
###在这里添加源文件目录
SRCDIR=	./\
		./db_server/\
		./game_server/\
		./gate_server/\
		./log_server/\
		./login_server/\
		./master_server/\
		./misc/\
		./v8/

###这里定义目标文件目录
OBJDIR =./obj/

TARGET_NAME=server

BIN=./

INCLUDE=-I/usr/local/include/netlib\
		$(addprefix -I, $(SRCDIR))

LIBDIR=-L./

LIB=-lnetlib\
	-lv8\
	-lv8_libplatform\
	-lcurl\
	-lcrypto\
	-lmongoclient\
	-lmysqlcppconn\
	-ljsoncpp\
	-lboost_filesystem\
	-lboost_thread-mt\

CC=g++

DEPENS=-MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"

DEBUGFLAG=-O0 -g3 -c -Wall -fmessage-length=0 -std=c++11

RELEASEFLAG=-O3 -Wall -c -fmessage-length=0 -std=c++11

LDFLAG=

BIN_TARGET=$(OBJDIR)bin/$(TARGET_NAME)

SRCS=$(wildcard $(addsuffix *.cpp, $(SRCDIR)))

OBJECTS:=$(addprefix $(OBJDIR), $(subst ./,,$(SRCS:.cpp=.o)))

.PHONY:all mkobjdir clean  

all:mkobjdir $(BIN_TARGET)

-include $(OBJECTS:.o=.d)

$(BIN_TARGET):$(OBJECTS)
	$(CC) $(LDFLAG) -o $@ $^ $(LIBDIR) $(LIB)
	@echo " "
	@echo "Finished building target: $(TARGET_NAME)"
	@echo " "
	@-cp -f $(BIN_TARGET) $(BIN)

$(OBJDIR)%.o:%.cpp
ifeq ($(MODE), DEBUG)
	@echo "Building DEBUG MODE target $@"
	$(CC) $(INCLUDE) $(DEBUGFLAG) $(DEPENS) -o "$(@)" "$(<)"
else
	@echo "Building RELEASE MODE target $@"
	$(CC) $(INCLUDE) $(RELEASEFLAG) $(DEPENS) -o "$(@)" "$(<)"
endif
	@echo " "

mkobjdir:
	@test -d $(OBJDIR) || (mkdir $(OBJDIR) && mkdir $(OBJDIR)bin $(addprefix $(OBJDIR), $(subst ./,,$(SRCDIR))))

clean:
	-rm -rf $(OBJDIR)
