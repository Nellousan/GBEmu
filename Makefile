##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## 
##

SRCDIR	=	src

OBJDIR 	=	obj

IDIR	=	include

SRC 	=	$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)

OBJ	=	$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CFLAGS	=	-Wall -Wextra -I./$(IDIR) -I./$(IDIR)/LR35902 -std=c++17 -g

LDFLAGS	=	-lsfml-graphics -lsfml-window -lsfml-system

NAME	=	dmg

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

all:	$(NAME)

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f *~
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: 	fclean all
