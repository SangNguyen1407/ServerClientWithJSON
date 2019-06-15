#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../inc/json_inc.h"

int json_parse(JSON_CONTENT json_buff, JSON_OBJECT_CONTENT *json_object) {
	int c_json = 0;
	int c_flag = 0;
	int c_obj_num = 0;
	int key_flag = 1; /*check key or value*/
	int c_num	= 0;
	if (json_object == NULL){
		return 0;
	}

	for (; c_json< json_buff.size; c_json++){
		switch (json_buff.buffer[c_json])
		{
			case '{':
			case '}':
			case '\r':
			case '\n':
			case '\"':
				break;
			case ':':	/*get value*/
				json_object->json_obj[c_obj_num].key[c_num] = '\0';
				key_flag = 0;
				c_num = 0;
				break;
			case ',':	/*the other object*/
				json_object->json_obj[c_obj_num].value[c_num] = '\0';
				key_flag = 1; /*get key*/
				c_num = 0;
				c_obj_num ++;
				break;
			default:
				if (key_flag){
					json_object->json_obj[c_obj_num].key[c_num++] = json_buff.buffer[c_json];
				}
				else{
					json_object->json_obj[c_obj_num].value[c_num++] = json_buff.buffer[c_json];
				}
		}	
	}
	return c_obj_num;
}

int json_value_size( JSON_CONTENT jsData ){
	
	int json_num = 0;
	int obj_num = 0;
	if ( jsData.buffer == NULL ){
		return 0;
	}
	
	for (; json_num < jsData.size; json_num ++){
		if ( jsData.buffer[json_num] == ','){
			obj_num ++;
		}
	}
	
	return obj_num++;
}

int j2str(JSON_OBJECT_CONTENT json_object, JSON_CONTENT *json_buff){
	
	int json_num = 0;
	int json_buff_end = 0;
	int isChar = 1;
	int file_size = 0;
	
	if (json_object.json_obj == NULL){
		return 0;
	}

	file_size = get_file_size("json.txt", READ_ONLY_MODE);
	json_buff->buffer = (char*) malloc(file_size );
	memset (json_buff->buffer, '\0', file_size);
	
	json_buff->buffer[0] = '{';
	json_buff->buffer[1] = '\r';
	json_buff->buffer[2] = '\n';

	json_buff_end = 3;
	for (; json_num < json_object.size+1; json_num ++){
		if (json_num != 0){
			strcpy(json_buff->buffer+(json_buff_end++), ",");
			json_buff->buffer[json_buff_end++]= '\r';
			json_buff->buffer[json_buff_end++] = '\n';
			isChar = 1;
		}
		
		strcpy(json_buff->buffer+(json_buff_end++), "\"");
		strcpy(json_buff->buffer+json_buff_end, json_object.json_obj[json_num].key);
		json_buff_end += strlen(json_object.json_obj[json_num].key);
		strcpy(json_buff->buffer+(json_buff_end++), "\"");
		
		strcpy(json_buff->buffer+(json_buff_end++), ":");
		
		isChar = isStringValue(json_object.json_obj[json_num].value);
		if (isChar){
			strcpy(json_buff->buffer+(json_buff_end++), "\"");
		}
		strcpy(json_buff->buffer+json_buff_end, json_object.json_obj[json_num].value);
		json_buff_end += strlen(json_object.json_obj[json_num].value);
		
		if (isChar){
			strcpy(json_buff->buffer+(json_buff_end++), "\"");
		}
		
	}
	json_buff->buffer[json_buff_end++]= '\r';
	json_buff->buffer[json_buff_end++] = '\n';	
	json_buff->buffer[json_buff_end++] = '}';
	json_buff->buffer[json_buff_end++] = '\0';
	json_buff->size = json_buff_end;
	return 1;
}

// int main () {
	// JSON_CONTENT json_buff;
	// JSON_CONTENT json_buff_temp;
	// JSON_OBJECT_CONTENT json_object;
	
	// json_buff = readFile("json.txt", READ_ONLY_MODE);
	
	// json_object.size = json_value_size(json_buff);
	// json_object.json_obj = (JSON_OBJECT *)malloc(sizeof(JSON_OBJECT) * json_object.size);
	
	// json_parse(json_buff, &json_object);
	// j2str(json_object, &json_buff_temp);
	// writeFile(json_buff_temp, "json_wr.txt", WRITE_ONLY_MODE);
	
	// return 0;
// }