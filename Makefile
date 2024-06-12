NAME = rubik

SRC_DIR = srcs/classes
OBJ_DIR = .obj

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

MAIN_SRC = srcs/main.cpp
MAIN_OBJ = $(OBJ_DIR)/main.o
MAIN_WASM = main

CC = clang++
CPPFLAGS = -Wall -Wextra -Werror -MMD -MP -gdwarf-2
EMCC = em++

DIRDUP = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS) $(MAIN_OBJ)
	@printf "\033[0;32mCompilation successful.\033[0m\n"
	@$(CC) $(OBJS) $(MAIN_OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@$(DIRDUP)
	@$(CC) $(CPPFLAGS) -c -o $@ $<

$(MAIN_OBJ): $(MAIN_SRC)
	@$(DIRDUP)
	@$(CC) $(CPPFLAGS) -c -o $@ $<

-include $(DEPS)

wasm: $(MAIN_OBJ)
	@printf "\033[0;32mCompiling to WebAssembly.\033[0m\n"
	@$(EMCC) $(MAIN_SRC) -o website/$(MAIN_WASM).html --emrun --bind \
		-o website/js/$(MAIN_WASM).js \
		-o website/wasm/$(MAIN_WASM).wasm -s WASM=1 -lembind

clean:
	@rm -rf $(OBJ_DIR)
	@printf "\033[0;32mCleanup successful.\033[0m\n"

fclean: clean
	@rm -f $(NAME) website/$(MAIN_WASM).wasm website/$(MAIN_WASM).html website/js/$(MAIN_WASM).js 
	@printf "\033[0;32mFull cleanup successful.\033[0m\n"

re: fclean all

.PHONY: all clean fclean re