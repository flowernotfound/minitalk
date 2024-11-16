CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./inc
SRC_DIR = src
SRC = $(SRC_DIR)/server.c $(SRC_DIR)/utils.c
CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/utils.c
BONUS_DIR = bonus
BONUS_SRC = $(BONUS_DIR)/server_bonus.c $(BONUS_DIR)/utils_bonus.c
CLIENT_BONUS_SRC = $(BONUS_DIR)/client_bonus.c $(BONUS_DIR)/utils_bonus.c
OBJ = $(SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)
CLIENT_BONUS_OBJ = $(CLIENT_BONUS_SRC:.c=.o)
SERVER = server
CLIENT = client
SERVER_BONUS = server_bonus
CLIENT_BONUS = client_bonus

all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -o $@

bonus: $(SERVER_BONUS) $(CLIENT_BONUS)

$(SERVER_BONUS): $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) -o $@

$(CLIENT_BONUS): $(CLIENT_BONUS_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_BONUS_OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(CLIENT_OBJ) $(BONUS_OBJ) $(CLIENT_BONUS_OBJ)

fclean: clean
	rm -f $(SERVER) $(CLIENT) $(SERVER_BONUS) $(CLIENT_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re
