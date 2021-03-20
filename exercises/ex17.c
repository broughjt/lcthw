#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
  int id;
  int set;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

struct Database {
 struct Address rows[MAX_ROWS];
};

struct Connection {
  FILE *file;
  struct Database *database;
};

void Database_close(struct Connection *connection);

void die(struct Connection *connection, const char *message)
{
  Database_close(connection);

  if (errno) { // TODO: Figure out where this comes from
    perror(message); // TODO: Figure out what this does
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

void Address_print(struct Address *address)
{
  printf("%d %s %s\n", address->id, address->name, address->email);
}

void Database_load(struct Connection *connection)
{
  // Loads sizeof(struct Database) bytes from the stream connection->file into connection->database (which is an array of addresses)
  int rc = fread(connection->database, sizeof(struct Database), 1, connection->file);
  if (rc != 1)
    die(connection, "Failed to load database");
}

struct Connection *Database_open(const char *filename, char mode)
{
  struct Connection *connection = malloc(sizeof(struct Connection));
  if (!connection)
    die(connection, "Memory error");

  connection->database = malloc(sizeof(struct Database));
  if (!connection->database)
    die(connection, "Memory error");
  
  if (mode == 'c') {
    // connection->file is a pointer to the file returned by fopen
    connection->file = fopen(filename, "w");
  } else {
    // same here
    connection->file = fopen(filename, "r+");

    if (connection->file)
      Database_load(connection);
  }

  if (!connection->file)
    die(connection, "Failed to open the file");

  return connection;
}

void Database_close(struct Connection *connection)
{
  if (connection) {
    if (connection->file)
      fclose(connection->file);
    if (connection->database)
      free(connection->database);
    free(connection);
  }
}

void Database_write(struct Connection *connection)
{
  // TODO: find out what this does
  rewind(connection->file);

  int rc = fwrite(connection->database, sizeof(struct Database), 1, connection->file);
  if (rc != 1)
    die(connection, "Failed to write database");

  rc = fflush(connection->file);
  if (rc == -1)
    die(connection, "Cannot flush database");
}

void Database_create(struct Connection *connection)
{
  int i = 0;

  for (i = 0; i < MAX_ROWS; i++) {
    struct Address address = {.id = i, .set = 0};
    connection->database->rows[i] = address;
  }
}

void Database_set(struct Connection *connection, int id, const char *name, const char *email)
{
  struct Address *address = &connection->database->rows[id];
  if (address->set)
    die(connection, "Already set, delete it first");

  address->set = 1;
  // WARNING: bug, read the "How to break it" and fix this
  char *res = strncpy(address->name, name, MAX_DATA);
  // demonstrate the strncpy bug
  if (!res)
    die(connection, "Name copy failed");

  res = strncpy(address->email, email, MAX_DATA);
  if (!res)
    die(connection, "Email copy failed");

  address->name[511] = '\0';
  address->email[511] = '\0';
}

void Database_get(struct Connection *connection, int id)
{
  struct Address *address = &connection->database->rows[id];
  if (address->set) {
    Address_print(address);
  } else {
    die(connection, "ID is not set");
  }
}

void Database_delete(struct Connection *connection, int id)
{
  struct Address address = {.id = id, .set = 0};
  connection->database->rows[id] = address;
}

void Database_list(struct Connection *connection)
{
  int i = 0;
  struct Database *database = connection->database;

  for (i = 0; i < MAX_ROWS; i++) {
    struct Address *address = &database->rows[i];

    if (address->set)
      Address_print(address);
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3)
    die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  struct Connection *connection = Database_open(filename, action);
  int id = 0;

  // TODO: what does atoi do?
  if (argc > 3)
    id = atoi(argv[3]);
  if (id >= MAX_ROWS)
    die(connection, "There's not that many records");

  switch (action) {
  case 'c':
    Database_create(connection);
    Database_write(connection);
    break;

  case 'g':
    if (argc != 4)
      die(connection, "Need an id to get");

    Database_get(connection, id);
    break;

  case 's':
    if (argc != 6)
      die(connection, "Need id, name, email to set");

    Database_set(connection, id, argv[4], argv[5]);
    Database_write(connection);
    break;

  case 'd':
    if (argc != 4)
      die(connection, "Need id to delete");

    Database_delete(connection, id);
    Database_write(connection);
    break;

  case 'l':
    Database_list(connection);
    break;
  default:
    die(connection, "Invalid action: c=create, g=get, s=set, d=delete, l=list");
  }

  Database_close(connection);

  return 0;
}
      
