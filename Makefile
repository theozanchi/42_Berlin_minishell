# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jschott <jschott@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/12 10:48:22 by tzanchi           #+#    #+#              #
#    Updated: 2023/11/15 13:53:40 by jschott          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Compiler targets and libraries
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g #-fsanitize=address
NAME		=	minishell
LIBFT		=	libft.a
READLINE	=	-lreadline

# Directories
SRCS_DIR	=	./sources/
HEAD_DIR	=	./includes/
LIBFT_DIR	=	./libft/
OBJ_DIR		=	${SRCS_DIR}.o

# Colours, symbols and utils
GREEN		=	\033[1;32m
CYAN		=	\033[1;36m
YELLOW		=	\033[1;33m
RED			=	\033[1;31m
NC			=	\033[0m
BOLD		=	\033[1m
TICK		=	✓

SRC			=	0_utils/utils_1.c \
				1_lexer/lexer_main.c \
				1_lexer/lexer_utils.c \
				1_lexer/save_symbol.c \
				1_lexer/save_word_save_quote.c \
				2_parser/concatenate_final_commands.c \
				2_parser/parser_main.c \
				2_parser/parser_utils.c \
				2_parser/populate_node.c \
				3_executer/cmd_execute.c \
				3_executer/env_extract_path.c \
				3_executer/executer_main.c \
				4_free/free_1.c \
				4_free/free_2.c \
				init.c \
				main.c \

SRCS		=	$(addprefix ${SRCS_DIR}, ${SRC})
SRC_NR		=	$(words ${SRCS})

OBJS		=	$(patsubst ${SRCS_DIR}%, ${OBJ_DIR}/%, $(SRCS:.c=.o))

all:			project_logo ${OBJ_DIR}
				@make -s ${LIBFT}
				@make -s ${NAME}

${LIBFT}:
				@echo "${CYAN}\nCOMPILING $$(echo ${LIBFT} | tr '[:lower:]' '[:upper:]')${NC}"
				@if [ -d ${LIBFT_DIR} ]; \
				then git -C ${LIBFT_DIR} pull; \
				else git clone https://github.com/theozanchi/42_Berlin_libft.git ${LIBFT_DIR}; \
				fi
				make -C ${LIBFT_DIR}

${NAME}:		entry_message ${OBJS}
				@${CC} ${CFLAGS} ${SRCS} -I${HEAD_DIR} ${LIBFT_DIR}${LIBFT} ${READLINE} -o ${NAME}
				@echo "${YELLOW}\nCompilation complete, ${NAME} executable at the root of the directory${NC}\n"

${OBJ_DIR}:
				@if [ ! -d "${OBJ_DIR}" ]; \
				then mkdir -p "${OBJ_DIR}"; \
				fi

$(OBJ_DIR)/%.o:	$(SRCS_DIR)%.c
				@mkdir -p $(dir $@)
				@printf "Compiling $(notdir $<)"; \
				${CC} ${CFLAGS} -I${HEAD_DIR} -c $< -o $@; \
				printf "${GREEN}"; \
				printf " ${TICK}\n"; \
				printf "${NC}"; 


clean:
				@make -sC ${LIBFT_DIR} clean >/dev/null 2>&1
				@if [ ! -d "${OBJ_DIR}" ]; \
				then \
					echo "Repo already clean"; \
				else \
					echo "Removing all .o files"; \
					rm -r ${OBJ_DIR}; \
				fi

fclean:			clean
				@make -sC ${LIBFT_DIR} fclean >/dev/null 2>&1
				@echo "Removing ${NAME} and ${LIBFT} files from root"
				@rm -f ${NAME} ${LIBFT}

re:				fclean all

project_logo:
				@echo "${RED}   _     _     _     _     _     _     _     _     _  \n  / \   / \   / \   / \   / \   / \   / \   / \   / \ \n ( ${CYAN}m${RED} ) ( ${CYAN}i${RED} ) ( ${CYAN}n${RED} ) ( ${CYAN}i${RED} ) ( ${CYAN}s${RED} ) ( ${CYAN}h${RED} ) ( ${CYAN}e${RED} ) ( ${CYAN}l${RED} ) ( ${CYAN}l${RED} )\n  \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/ \n\n          ${NC}a 42 project by ${BOLD}Jakob Schott${NC} and ${BOLD}Théo Zanchi${NC}"

entry_message:
				@echo "${CYAN}\nCOMPILING $$(echo ${NAME} | tr '[:lower:]' '[:upper:]')\n${NC}${BOLD}Compiling necessary .o files out of ${SRC_NR} .c files in total${NC}"

.PHONY:			all clean fclean re project_logo entry_message
