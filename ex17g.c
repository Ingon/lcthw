#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// #define MAX_DATA 512
// #define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_rows;
    int max_data;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

static struct Connection *conn;

// Forward declarations
void Database_close();

void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    if(conn) {
        Database_close(conn);
    }

    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load()
{
    int rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to load database.");

    rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to load database.");

    conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
    if(! conn->db->rows) die("Memory error");

    // rc = fread(conn->db->rows, sizeof(struct Address), conn->db->max_rows, conn->file);
    // if(rc != conn->db->max_rows) die("Failed to load database.");
    for(int i = 0, n = conn->db->max_rows; i < n; i++) {
        struct Address addr = {.id = i};

        rc = fread(&addr.set, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to load database.");

        if(addr.set) {
            addr.name = malloc(sizeof(char) * conn->db->max_data);
            rc = fread(addr.name, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to load database.");

            addr.email = malloc(sizeof(char) * conn->db->max_data);
            rc = fread(addr.email, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to load database.");
        }

        conn->db->rows[i] = addr;
    }
}

void Database_write()
{
    rewind(conn->file);

    int rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to write database.");

    rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to write database.");

    // rc = fwrite(conn->db->rows, sizeof(struct Address), conn->db->max_rows, conn->file);
    // if(rc != conn->db->max_rows) die("Failed to write database.");

    for(int i = 0, n = conn->db->max_rows; i < n; i++) {
        struct Address *addr = conn->db->rows + i;

        rc = fwrite(&addr->set, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to write database.");

        if(addr->set) {
            rc = fwrite(addr->name, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to write database.");

            rc = fwrite(addr->email, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to write database.");
        }
    }

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.");
}

void Connection_init()
{
    conn = malloc(sizeof(struct Connection));
    if(! conn) die("Memory error");

    conn->db = malloc(sizeof(struct Database));
    if(! conn->db) die("Memory error");
}

void Database_init(const char *filename, int max_rows, int max_data)
{
    Connection_init();

    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;

    conn->db->rows = malloc(sizeof(struct Address) * max_rows);
    if(! conn->db->rows) die("Memory error");

    conn->file = fopen(filename, "w");
    if(! conn->file) die("Failed to open the file");
}

void Database_open(const char *filename)
{
    Connection_init();

    conn->file = fopen(filename, "r+");
    if(! conn->file) die("Failed to open the file");

    Database_load();
}

void Database_close()
{
    if(conn) {
        for(int i = 0, n = conn->db->max_rows; i < n; i++) {
            struct Address *addr = conn->db->rows + i;
            if(addr->set) {
                free(addr->name);
                free(addr->email);
            }
        }

        if(conn->db->rows) free(conn->db->rows);
        if(conn->db) free(conn->db);

        if(conn->file) fclose(conn->file);

        free(conn);
    }
}

void Database_create()
{
    int i = 0;

    for(i = 0; i < conn->db->max_rows; i++) {
        struct Address addr = {
            .id = i, 
            .set = 0
        };
        conn->db->rows[i] = addr;
    }
}

void Database_set(int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first");

    addr->set = 1;

    if(addr->name == NULL) {
        addr->name = malloc(sizeof(char) * conn->db->max_data);
    }
    char *res = strncpy(addr->name, name, conn->db->max_data);
    if(! res) die("Name copy failed");

    if(addr->email == NULL) {
        addr->email = malloc(sizeof(char) * conn->db->max_data);
    }
    res = strncpy(addr->email, email, conn->db->max_data);
    if(! res) die("Email copy failed");
}

void Database_get(int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

void Database_delete(int id)
{
    struct Address addr = {
        .id = id, 
        .set = 0
    };
    conn->db->rows[id] = addr;
}

void Database_list()
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < db->max_rows; i++) {
        struct Address *addr = db->rows + i;

        if(addr->set) {
            Address_print(addr);
        }
    }
}

void Database_find(const char* term)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < db->max_rows; i++) {
        struct Address *addr = db->rows + i;

        if(addr->set && (strstr(addr->name, term) || strstr(addr->email, term))) {
            Address_print(addr);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];

    if(action == 'c') {
        int max_rows = atoi(argv[3]);
        int max_data = atoi(argv[4]);

        Database_init(filename, max_rows, max_data);
        Database_create();
        Database_write();
        Database_close();
    } else {
        Database_open(filename);

        int id = 0;
        if(argc > 3) id = atoi(argv[3]);
        if(id >= conn->db->max_rows) die("There's not that many records.");

        switch(action) {
            case 'g':
                if(argc != 4) die("Need an id to get");
                Database_get(id);
                break;

            case 's':
                if(argc != 6) die("Need id, name, email to set");
                Database_set(id, argv[4], argv[5]);
                Database_write();
                break;

            case 'd':
                if(argc != 4) die("Need id to delete");
                Database_delete(id);
                Database_write();
                break;

            case 'l':
                Database_list();
                break;

            case 'f':
                Database_find(argv[3]);
                break;

            default:
                die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
        }

        Database_close();
    }

    return 0;
}