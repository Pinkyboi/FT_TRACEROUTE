LIB_NAME = libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC_PATH = ./src
OBJ_PATH = ./obj
INC_PATH = ./inc

HDR_FILES = libft.h

SRC_FILES = ft_atoi.c\
			ft_bzero.c\
			ft_howmany.c\
			ft_isalnum.c\
			ft_isalpha.c\
			ft_isascii.c\
			ft_isdigit.c\
			ft_isprint.c\
			ft_itoa.c\
			ft_lstadd.c\
			ft_lstclear.c\
			ft_lstdel.c\
			ft_lstdelone.c\
			ft_lstiter.c\
			ft_lstmap.c\
			ft_lstnew.c\
			ft_memalloc.c\
			ft_memccpy.c\
			ft_memchr.c\
			ft_memcmp.c\
			ft_memcpy.c\
			ft_memdel.c\
			ft_memmove.c\
			ft_memset.c\
			ft_power_positif.c\
			ft_putchar.c\
			ft_putchar_fd.c\
			ft_putendl.c\
			ft_putendl_fd.c\
			ft_putnbr.c\
			ft_putnbr_fd.c\
			ft_putstr.c\
			ft_putstr_fd.c\
			ft_replace.c\
			ft_strcat.c\
			ft_strchr.c\
			ft_strclr.c\
			ft_strcmp.c\
			ft_strcpy.c\
			ft_strdel.c\
			ft_strdup.c\
			ft_strequ.c\
			ft_striter.c\
			ft_striteri.c\
			ft_strjoin.c\
			ft_strlcat.c\
			ft_strlen.c\
			ft_strmap.c\
			ft_strmapi.c\
			ft_strncat.c\
			ft_strncmp.c\
			ft_strncpy.c\
			ft_strnequ.c\
			ft_strnew.c\
			ft_strnstr.c\
			ft_strrchr.c\
			ft_strsplit.c\
			ft_strstr.c\
			ft_strsub.c\
			ft_strtouper.c\
			ft_strtrim.c\
			ft_tolower.c\
			ft_toupper.c\

OBJ_FILES = $(SRC_FILES:.c=.o)

HDR = $(addprefix $(INC_PATH)/, $(HDR_FILES))
SRC = $(addprefix $(SRC_PATH)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_FILES))

INC = -I $(INC_PATH)

.PHONY: all clean fclean re

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	@ar rc $@ $^

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c $(HDR)| $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean 
	@rm -f $(LIB_NAME)

re: fclean all