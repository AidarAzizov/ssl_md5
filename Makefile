NAME = ft_ssl


 FLAGS = -O0#-Wall -Wextra -Werror

 FUNC = main utils helpers hashes/md5 hashes/parse hashes/print hashes/sha256 \
 		overcodes/base64 overcodes/print overcodes/parse

 HEADER = -I includes/

 SRC = $(addprefix srcs/, $(addsuffix .c, $(FUNC)))
 OBJ = $(addprefix build/, $(addsuffix .o, $(FUNC)))


 #=====COLOUR========
 RED = \033[1;31m
 GREEN = \033[1;92m
 BLUE = \033[34;1m
 YELLOW = \033[0;33m
 #===================

all: .PHONY $(NAME)

.PHONY:
	@mkdir -p build
	@mkdir -p build/hashes
	@mkdir -p build/overcodes


$(NAME): $(OBJ)
	@echo "$(GREEN)(ﾉ◕ヮ◕)ﾉ*:・ﾟ✧ Compiling project... Wait a sec."
	@gcc $(FLAGS) -o $(NAME) $(OBJ) $(HEADER)
	@echo "$(GREEN)(•̀ᴗ•́)و $(NAME) generated!"

build/%.o: srcs/%.c includes/utils.h includes/boolean.h includes/ft_ssl.h
	@gcc $(FLAGS) -o $@ -c $< $(HEADER)

clean:
	@/bin/rm -f $(OBJ)
	@/bin/rm -f $(OBJ_PS)
	@rm -rf build
	@echo "  $(BLUE)OBJ files have been deleted."

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "  $(BLUE)$(NAME) have been deleted."

re: fclean all
