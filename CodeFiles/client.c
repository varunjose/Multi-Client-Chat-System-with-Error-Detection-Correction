#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include "handle.h"

void trim_newline(char *text)
{
  int len = strlen(text) - 1;
  if (text[len] == '\n')
  {
    text[len] = '\0';
  }
}

void clear_stdin_buffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

// get a username from the user.
void get_username(char *username)
{
  while (true)
  {
    printf("Login with your username!!\n");
    printf("Enter a username: ");
    fflush(stdout);
    memset(username, 0, 1000);
    fgets(username, 22, stdin);
    trim_newline(username);

    if (strlen(username) > 20)
    {
      puts("Username must be 8 characters or more.");
    }
    else
    {
      break;
    }
  }
}

//send local username to the server.
void set_username(connection_info *connection)
{
  message msg;
  msg.type = SET_USERNAME;
  strncpy(msg.username, connection->username, 20);
  //
  if (send(connection->socket, (void *)&msg, sizeof(msg), 0) < 0)
  {
    perror("Send failed");
    exit(1);
  }
}

void stop_client(connection_info *connection)
{
  close(connection->socket);
  exit(0);
}

//initialize connection to the server.
void connect_to_server(connection_info *connection, char *address, char *port)
{

  while (true)
  {
    get_username(connection->username);

    //Create socket
    if ((connection->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      perror("Could not create socket");
    }

    connection->address.sin_addr.s_addr = inet_addr(address);
    connection->address.sin_family = AF_INET;
    connection->address.sin_port = htons(atoi(port));

    //Connect to remote server
    if (connect(connection->socket, (struct sockaddr *)&connection->address, sizeof(connection->address)) < 0)
    {
      perror("Connect failed.");
      exit(1);
    }

    set_username(connection);

    message msg;
    ssize_t recv_val = recv(connection->socket, &msg, sizeof(message), 0);
    if (recv_val < 0)
    {
      perror("recv failed");
      exit(1);
    }
    else if (recv_val == 0)
    {
      close(connection->socket);
      printf("The username \"%s\" is taken, please try another name.\n", connection->username);
      continue;
    }

    break;
  }

  puts("Connected to server.");
  puts("Type /help for usage.");
}

void handle_user_input(connection_info *connection)
{
  char input[255];
  fgets(input, 255, stdin);
  trim_newline(input);

  if (strcmp(input, "LOGOUT") == 0 )
  {
    stop_client(connection);
  }
  else if (strcmp(input, "LOGIN_LIST") == 0)
  {
    message msg;
    msg.type = GET_USERS;

    if (send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
      perror("Send failed");
      exit(1);
    }
  }
  else if (strcmp(input, "/h") == 0 || strcmp(input, "/help") == 0)
  {
    puts("LOGOUT : Exit the program.");
    puts("HELP: Displays help information.");
    puts("LOGIN_LIST: Displays list of users in chatroom.");
    puts("MSG <username> <message> Send a private message to given username.");
  }
  else if (strncmp(input, "MSG", 2) == 0)
  {
    message msg;
    msg.type = PRIVATE_MESSAGE;

    char *toUsername, *chatMsg;

    toUsername = strtok(input + 3, " ");

    if (toUsername == NULL)
    {
      puts(KRED "The format for private messages is: MSG <username> <message>" RESET);
      return;
    }

    if (strlen(toUsername) == 0)
    {
      puts(KRED "You must enter a username for a private message." RESET);
      return;
    }

    if (strlen(toUsername) > 8)
    {
      puts(KRED "The username must be between 1 and 8 characters." RESET);
      return;
    }

    chatMsg = strtok(NULL, "");

    if (chatMsg == NULL)
    {
      puts(KRED "You must enter a message to send to the specified user." RESET);
      return;
    }

    strncpy(msg.username, toUsername, 8);
    strncpy(msg.data, chatMsg, 255);

    if (send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
      perror("Send failed");
      exit(1);
    }
  }
  else 
  {
    message msg;
    msg.type = PUBLIC_MESSAGE;
    strncpy(msg.username, connection->username, 20);

    if (strlen(input) == 0)
    {
      return;
    }

    strncpy(msg.data, input, 255);

    //Send some data
    if (send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
      perror("Send failed");
      exit(1);
    }
  }
}

void handle_server_message(connection_info *connection)
{
  message msg;

  //Receive a reply from the server
  ssize_t recv_val = recv(connection->socket, &msg, sizeof(message), 0);
  if (recv_val < 0)
  {
    perror("recv failed");
    exit(1);
  }
  else if (recv_val == 0)
  {
    close(connection->socket);
    puts("Server disconnected.");
    exit(0);
  }

  switch (msg.type)
  {

  case CONNECT:
    printf(KYEL "%s has connected." RESET "\n", msg.username);
    break;

  case DISCONNECT:
    printf(KYEL "%s has disconnected." RESET "\n", msg.username);
    break;

  case GET_USERS:
    printf("%s", msg.data);
    break;

  case SET_USERNAME:
    //TODO: implement: name changes in the future?
    break;

  case PUBLIC_MESSAGE:
    printf(KGRN "%s" RESET ": %s\n", msg.username, msg.data);
    break;

  case PRIVATE_MESSAGE:
    printf(KWHT "From %s:" KCYN " %s\n" RESET, msg.username, msg.data);
    break;

  case TOO_FULL:
    fprintf(stderr, KRED "Server chatroom is too full to accept new clients." RESET "\n");
    exit(0);
    break;

  default:
    fprintf(stderr, KRED "Unknown message type received." RESET "\n");
    break;
  }
}

int main(int argc, char *argv[])
{
  connection_info connection;
  //The fd_set data type represents file descriptor sets for the select function. It is actually a bit array.
  fd_set file_descriptors;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  connect_to_server(&connection, argv[1], argv[2]);

  
  //keep communicating with server
  while (true)
  {
    FD_ZERO(&file_descriptors);
    FD_SET(STDIN_FILENO, &file_descriptors);
    FD_SET(connection.socket, &file_descriptors);
    fflush(stdin);

    if (select(connection.socket + 1, &file_descriptors, NULL, NULL, NULL) < 0)
    {
      perror("Select failed.");
      exit(1);
    }

    if (FD_ISSET(STDIN_FILENO, &file_descriptors))
    {
      handle_user_input(&connection);
    }

    if (FD_ISSET(connection.socket, &file_descriptors))
    {
      handle_server_message(&connection);
    }
  }

  int dataatrec[10];
  int test[10];
  int p, p1, p2, p4;

  printf("The data received is: ");
  for (int i = 0; i < 10; i++)
  {
    scanf("%d", &dataatrec[i]);
  }

  printf("Enter data to test: ");
  for (int i = 0; i < 10; i++)
  {
    scanf("%d", &test[i]);
  }

  p1 = test[6] ^ test[4] ^ test[2] ^ test[0];
  p2 = test[5] ^ test[4] ^ test[1] ^ test[0];
  p4 = test[3] ^ test[0] ^ test[2] ^ test[1];
  p = p4 * 4 + p2 * 2 + p1;

  printf("The data for testing is: ");
  for (int i = 0; i < 7; i++)
  {
    printf("%d", test[i]);
  }

  if(p == 0)
  {
    printf("No error in data");
  }
  else
  {
    printf("Error on position %d", p);
    printf("The correct data is: ");
    if(test[7-p] == 0)
    {
      test[7-p] = 1;
    }
    else
    {
      test[7-p] = 0;
    }
    for (int i = 0; i < 7; i++)
    {
      printf("%d", test[i]);
    }
  }

  close(connection.socket);
  return 0;
}