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

OBJ		=	$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CFLAGS	=	-Wall -Wextra -I./$(IDIR) -I./$(IDIR)/LR35902 -std=c++17

LDFLAGS	=	-lsfml-graphics -lsfml-window -lsfml-system -lpthread -g

NAME	=	dmg

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

all:	$(NAME)

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f vgcore*
	rm -f *~
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

debug:	$(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

re: 	fclean all

d:		debug

red:	fclean debug

vg:
	valgrind --tool=helgrind --fair-sched=yes ./dmg -r custom_roms/Tetris.GB