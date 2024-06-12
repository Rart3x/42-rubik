NAME = rubik

SRC_DIR = ./
OBJ_DIR = .obj

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC = clang++
CPPFLAGS = -Wall -Wextra -Werror -MMD -MP -gdwarf-2
EMCC = em++

DIRDUP = mkdir -p $(@D)

MAIN_SRC = main.cpp

all: $(NAME) wasm

$(NAME): $(OBJS) $(MAIN_OBJ)
	@printf "\033[0;32mCompilation successful.\033[0m\n"
	@$(CC) $(OBJS) $(MAIN_OBJ) -o $(NAME)

wasm: $(MAIN_SRC)
	@printf "\033[0;32mCompiling to WebAssembly.\033[0m\n"
	@$(EMCC) $(MAIN_SRC) -O2 -o $(NAME).html --emrun --bind

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@$(DIRDUP)
	@$(CC) $(CPPFLAGS) -c -o $@ $<

$(MAIN_OBJ): $(MAIN_SRC)
	@$(DIRDUP)
	@$(CC) $(CPPFLAGS) -c -o $@ $<

-include $(DEPS)

clean:
	@rm -rf $(OBJ_DIR)
	@printf "\033[0;32mCleanup successful.\033[0m\n"

fclean: clean
	@rm -f $(NAME) $(NAME).wasm $(NAME).html $(NAME).js 
	@printf "\033[0;32mFull cleanup successful.\033[0m\n"

re: fclean all wasm

.PHONY: all clean fclean re wasm