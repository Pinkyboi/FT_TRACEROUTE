
NAME = ft_traceroute

CC = gcc

CFLAGS =
# CFLAGS = -Wall -Werror -Wextra

SRC_PATH = ./src
OBJ_PATH = ./obj
INC_PATH = ./inc

LIBFT_PATH = ./libft
LIBFT_INC = $(addprefix $(LIBFT_PATH)/, inc)

HDR_FILES = ft_traceroute.h

SRC_FILES = main.c\
			option.c\
			time.c\
			error.c\
			address.c\

OBJ_FILES = $(SRC_FILES:.c=.o)
HDR = $(addprefix $(INC_PATH)/,$(HDR_FILES))
SRC = $(addprefix $(SRC_PATH)/,$(SRC_FILES))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_FILES))

LIBS = -L$(LIBFT_PATH) -lft -lm

INC = -I$(INC_PATH) -I$(LIBFT_INC)

.PHONY: all clean fclean re

all : libft_all $(NAME)


libft_all :
	@make -C $(LIBFT_PATH)

$(NAME) : $(OBJ)
	@$(CC) $^ $(LIBS) -o $@

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c $(HDR)| $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH)

clean :
	@make -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ_PATH)

fclean : clean
	@make -C $(LIBFT_PATH) fclean
	@rm -f $(NAME)

re: fclean all