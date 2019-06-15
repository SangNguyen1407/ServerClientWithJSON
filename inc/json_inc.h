#ifndef _JSON_INC_H_
#define _JSON_INC_H_


#define READ_ONLY_MODE "r"
#define READ_WRITE_MODE "rw"
#define WRITE_ONLY_MODE "w"

#define JSON_OBJECT_SIZE 50 

typedef struct JSON_CONTENT
{
	char *buffer;
	int size;
}JSON_CONTENT;

typedef struct JSON_OBJECT
{
	char key[JSON_OBJECT_SIZE];
	char value[JSON_OBJECT_SIZE];
}JSON_OBJECT;

typedef struct JSON_OBJECT_CONTENT
{
	int size;
	JSON_OBJECT *json_obj;
}JSON_OBJECT_CONTENT;


void writeFile (JSON_CONTENT json_buff, char *filename, char *mode);
JSON_CONTENT readFile (char *filename, char *mode);
int isStringValue (char *stringValue);
int get_file_size(char *filename, char *mode);
int remove_whitespace(char *buffer);

#endif