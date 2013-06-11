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

// Forward declarations
void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
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

void Database_load(struct Connection *conn)
{
    int rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);

    rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);

    conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
    if(! conn->db->rows) die("Memory error", conn);

    // rc = fread(conn->db->rows, sizeof(struct Address), conn->db->max_rows, conn->file);
    // if(rc != conn->db->max_rows) die("Failed to load database.", conn);
    for(int i = 0, n = conn->db->max_rows; i < n; i++) {
        struct Address addr = {.id = i};

        rc = fread(&addr.set, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to load database.", conn);

        if(addr.set) {
            addr.name = malloc(sizeof(char) * conn->db->max_data);
            rc = fread(addr.name, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to load database.", conn);

            addr.email = malloc(sizeof(char) * conn->db->max_data);
            rc = fread(addr.email, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to load database.", conn);
        }

        conn->db->rows[i] = addr;
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    // rc = fwrite(conn->db->rows, sizeof(struct Address), conn->db->max_rows, conn->file);
    // if(rc != conn->db->max_rows) die("Failed to write database.", conn);

    for(int i = 0, n = conn->db->max_rows; i < n; i++) {
        struct Address *addr = conn->db->rows + i;

        rc = fwrite(&addr->set, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to write database.", conn);

        if(addr->set) {
            rc = fwrite(addr->name, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to write database.", conn);

            rc = fwrite(addr->email, sizeof(char), conn->db->max_data, conn->file);
            if(rc != conn->db->max_data) die("Failed to write database.", conn);
        }
    }

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

struct Connection *Connection_init()
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(! conn) die("Memory error", NULL);

    conn->db = malloc(sizeof(struct Database));
    if(! conn->db) die("Memory error", conn);

    return conn;
}

struct Connection *Database_init(const char *filename, int max_rows, int max_data)
{
    struct Connection *conn = Connection_init();
    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;

    conn->db->rows = malloc(sizeof(struct Address) * max_rows);
    if(! conn->db->rows) die("Memory error", conn);

    conn->file = fopen(filename, "w");
    if(! conn->file) die("Failed to open the file", conn);

    return conn;
}

struct Connection *Database_open(const char *filename)
{
    struct Connection *conn = Connection_init();

    conn->file = fopen(filename, "r+");
    if(! conn->file) die("Failed to open the file", conn);

    Database_load(conn);

    return conn;
}

void Database_close(struct Connection *conn)
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

void Database_create(struct Connection *conn)
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

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;

    if(addr->name == NULL) {
        addr->name = malloc(sizeof(char) * conn->db->max_data);
    }
    char *res = strncpy(addr->name, name, conn->db->max_data);
    if(! res) die("Name copy failed", conn);

    if(addr->email == NULL) {
        addr->email = malloc(sizeof(char) * conn->db->max_data);
    }
    res = strncpy(addr->email, email, conn->db->max_data);
    if(! res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {
        .id = id, 
        .set = 0
    };
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
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

void Database_find(struct Connection *conn, const char* term)
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
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];

    if(action == 'c') {
        int max_rows = atoi(argv[3]);
        int max_data = atoi(argv[4]);

        struct Connection *conn = Database_init(filename, max_rows, max_data);
        Database_create(conn);
        Database_write(conn);
        Database_close(conn);
    } else {
        struct Connection *conn = Database_open(filename);

        int id = 0;
        if(argc > 3) id = atoi(argv[3]);
        if(id >= conn->db->max_rows) die("There's not that many records.", conn);

        switch(action) {
            case 'g':
                if(argc != 4) die("Need an id to get", conn);
                Database_get(conn, id);
                break;

            case 's':
                if(argc != 6) die("Need id, name, email to set", conn);
                Database_set(conn, id, argv[4], argv[5]);
                Database_write(conn);
                break;

            case 'd':
                if(argc != 4) die("Need id to delete", conn);
                Database_delete(conn, id);
                Database_write(conn);
                break;

            case 'l':
                Database_list(conn);
                break;

            case 'f':
                Database_find(conn, argv[3]);
                break;

            default:
                die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
        }

        Database_close(conn);
    }

    return 0;
}