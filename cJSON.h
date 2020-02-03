#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define cJSON_AddStringToObject(object,name,s)	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))
#define cJSON_AddStringToObject(object,name,s)	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))


typedef struct{
char *array ;

} cJSON;





 cJSON *cJSON_CreateObject(){


 cJSON* object;
 object = malloc(sizeof(object));

 (object->array)=(char*)calloc(10000,1);

 (object->array)[0]='{';





 return object;
}
 char  *cJSON_PrintUnformatted(cJSON *item){
	return (item->array);
}
 cJSON *cJSON_CreateArray(){
	cJSON *array;
  array=malloc(sizeof(array));
	(array->array)=(char*)calloc(10000,1);
	(array->array)[0]='[';
	(array->array)[1]=']';

	return array;

}
void   cJSON_AddItemToArray(cJSON *array, cJSON *item){
	if((array->array)[strlen(array->array)-2]=='}') (array->array)[strlen(array->array)-1] =',';
	else (array->array)[strlen(array->array)-1]='\0';
 	char string[1000];
	sprintf(string,"%s",item->array);
	strcat(array->array,string);
	strcat(array->array,"]");
	return;
}
 void	cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item){


  if((object->array)[strlen(object->array)-1]=='}') (object->array)[strlen(object->array)-1] =',';




char str[1000];
sprintf(str,"\"%s\":",string);


strcat(object->array,str);
if(item->array[0]=='[') sprintf(str,"%s",item->array);
else sprintf(str,"%s",item->array);


strcat(object->array,str);
strcat(object->array,"}");
return;


}
 cJSON *cJSON_CreateString(const char *string){
	cJSON *String;
  String=malloc(sizeof(String));
  String->array=calloc(1000,1);
  if((string)[0]=='\"') sprintf(String->array,"%s",string);
  else sprintf(String->array,"\"%s\"",string);

	return String;
}
