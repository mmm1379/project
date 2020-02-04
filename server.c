//to run(with official cjson):  clear && gcc cJSON.c server.c -o server -lm && ./server
//to run(with unofficial cjson):  clear && gcc -o server  server.c && ./server



#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <time.h>
#include "cJSON.h"
struct sockaddr_in server, client;
char* member[100][3];
//the first one is username
//the second one is AuthToken
//the third one is channel logged in

int counters[100];
//counter for not showing previous messages to user



char buffer[200];
int client_socket;
int server_socket;
void look();
void registerm();
void login();
void create();
void join();
void logout();
void sendm();
void refresh();
void listpeople();
void quitchannel();
int authtoken_finder();
int channel_finder();
void successfulsender();
void errorsender(char *p);



//the main function creates and bounds socket. then it starts listening
int main(){




	// Create and verify socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	// Assign IP and port
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(12345);

	// Bind newly created socket to given IP and verify
	if ((bind(server_socket, (struct sockaddr*)&server, sizeof(server))) != 0) {
		printf("Socket binding failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully bound..\n");

	// Now server is ready to listen and verify
	if ((listen(server_socket, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");


	look();

return 0;

}

//the look function accepts client everytime and checks what it sent , calls the right function and then closes the socket
void look(){

int n;
//char yn;
	while (1) {
		//printf("continue?\n" );
		//scanf("%c\n", &yn);
		//if (yn=='n') return;
		socklen_t len = sizeof(client);
		client_socket = accept(server_socket, (struct sockaddr*)&client, &len);
		if (client_socket < 0) {
			printf("Server accceptance failed...\n");
			exit(0);
		}
		else
			printf("Server acccepted the client..\n");
		bzero(buffer, sizeof(buffer));



		// Read the message from client and copy it to buffer
		recv(client_socket, buffer, sizeof(buffer), 0);
		printf("client sent %s\n", buffer);
		for (int n=0;buffer[n] != '\0';n++) printf("%c",buffer[n]);

		if (strcmp(buffer,"goodbye")==0) return;
		else if (strstr(buffer,"register")==buffer) registerm();
		else if (strstr(buffer,"login")== buffer) login();
		else if (strstr(buffer,"create")==buffer) create();
		else if (strstr(buffer,"join")==buffer) join();
		else if (strstr(buffer,"logout")==buffer) logout();
		else if (strstr(buffer,"send")==buffer) sendm();
		else if (strstr(buffer,"channel members")==buffer) listpeople();
		else if (strstr(buffer,"leave")==buffer) quitchannel();
		else if (strstr(buffer,"refresh")==buffer) refresh();
		else {printf("wrong answer:D\n");errorsender("wrong answer:D");return;}

		shutdown(server_socket, SHUT_RDWR);













		}
	}

//the authtoken_finder checks member to see if they have the authtoken in the buffer. if they had the authtoken then it returns the j of member[j][1]. else it returns -1
int authtoken_finder(){
	int j=0;
	int check=0;
	for(;member[j][1]!=NULL;j++){printf("%s\n",member[j][1]); if(strstr(buffer,member[j][1])!=NULL)  {printf("authtoken was found\n");check=1;break;}}
	if (check!=1) {printf("no authtoken found\n");errorsender("no authtoken found");return -1;}
	return j;
}
//channel_finder will find the channel number in channels array
int channel_finder(){

	char channelnamec[50];

	int j=0;
	int check=0;
	FILE *fp;
	fp=fopen("channels.txt","r");
	if (fp==NULL) {printf("error opening channels.text file\n");send(client_socket, "hello", sizeof("hello"), 0);return -1;}

//
while(1){if (fgetc(fp)==EOF) {printf("no channel found\n");errorsender("Channel not found.");return -1;}
fseek(fp, -1L, SEEK_CUR);
fscanf(fp,"%s\n",channelnamec);
if(strcmp(buffer,channelnamec)==0){
	printf("channel was found\n");
	if(fclose(fp)==EOF) {printf("error closing channels.text file\n");return -1;}
return 0;
}
}
}
//successfulsender as it says sends a Successful message to the client
void successfulsender(){

cJSON *successful;

successful = cJSON_CreateObject();

cJSON_AddStringToObject(successful,"type","Successful");

cJSON_AddStringToObject(successful,"content","");





send(client_socket, strcat(cJSON_PrintUnformatted(successful),"\n"), strlen(cJSON_PrintUnformatted(successful))+1, 0);
//send(client_socket, "{\"type\":\"Successful\",\"content\":\"\"}\n", sizeof("{\"type\":\"Successful\",\"content\":\"\"}\n"), 0);
}
void errorsender(char *p){


	cJSON *error;


	error = cJSON_CreateObject();


	cJSON_AddStringToObject(error,"type","Error");


	cJSON_AddStringToObject(error,"content",p);





	send(client_socket, strcat(cJSON_PrintUnformatted(error),"\n"), strlen(cJSON_PrintUnformatted(error))+1, 0);
}



// the registerm function opens members.txt for append and prints the buffer in it.
void registerm(){

	char username[50];
	char password[50];
	char usernamec[50];
	char passwordc[50];
	sscanf(buffer,"register %[^,], %s\n",username,password);
	FILE *fp;
	fp=fopen("members.txt","a");
	fclose(fp);
	fp=fopen("members.txt","r");
	if (fp==NULL) {printf("error opening members.text file\n");errorsender("error opening members.text file");return;}
	while(fgetc(fp)!=EOF){
		fseek(fp, -1L, SEEK_CUR);

	fscanf(fp,"%[^|]|%s\n",usernamec,passwordc);
	if(strcmp(usernamec,username)==0){errorsender("this username is not available.");return;}
}if(fclose(fp)==EOF) {printf("error closing channels.text file\n");return;}



	fp=fopen("members.txt","a");
if (fp==NULL) {printf("error opening members.text file\n");errorsender("error opening members.text file");return;}
	fprintf(fp,"%s|%s\n",username,password);
	if(fclose(fp)==EOF) {printf("error closing members.text file\n");return;}
			successfulsender();

}
//the login function reads the members.txt to find the username which is in the buffer. if there was one it creates an authtoken and sends it to the client.
//the authtoken made is stored in the heap section so we have to free it later.
//this function is also used to store username and the authtoken in the member array.
void login(){
time_t t;
srand((unsigned) time(&t));
char username[100];
char usernamec[100];
char password[100];
char passwordc[100];
char registers[200];
FILE *fp;
FILE *user;
fp=fopen("members.txt","a");
fclose(fp);
fp=fopen("members.txt","r");
if (fp==NULL) {printf("error opening members.text file\n");errorsender("error opening members.text file");return;}
sscanf(buffer,"login %[^,], %s",username,password);
printf("username is %s, password is %s\n",username, password);
while(1){if (fgetc(fp)==EOF) {errorsender("Username is not valid");return;}
fseek(fp, -1L, SEEK_CUR);
fscanf(fp,"%[^|]|%s\n",usernamec,passwordc);


printf("%s,%s\n",usernamec,passwordc );
if(strcmp(usernamec,username)==0) {
	for(int i=0;member[i][0]!=NULL;i++) if(strcmp(member[i][0],username)==0) {errorsender("The user is already logged in.");return;}
	if(strcmp(password,passwordc)==0){printf("successfully logged in\n" );
//making an authtoken and sending it
	int i=0;
	for(;i<100;i++) if(member[i][0]==NULL) {member[i][0]=malloc(50);strcpy(member[i][0],username);break;}

	member[i][1]=malloc(33);
	for(int j=0;j<32;j++) (member[i][1][j]) = 'A' + (rand() % 26);
	((member[i][1][32]))='\0';




cJSON * authtokensender;
authtokensender = cJSON_CreateObject();

cJSON_AddStringToObject(authtokensender,"type","AuthToken");

cJSON_AddStringToObject(authtokensender,"content",member[i][1]);
printf("%s\n",cJSON_PrintUnformatted(authtokensender));



	//char authtokensender[100];
	//sprintf(authtokensender,"{\"type\":\"AuthToken\",\"content\":\"%s\"}",member[i][1]);
	send(client_socket,cJSON_PrintUnformatted(authtokensender),strlen(cJSON_PrintUnformatted(authtokensender)), 0);
	printf("the auth token %s was made and sent\n",member[i][1]);
	i++;return;}

	else {errorsender("Wrong password");return;}}
}
if(fclose(fp)==EOF) {printf("error closing members.text file\n");return;}}

//the create function is used to add the buffer(which contains the channel name) to channels.txt

void create(){
	int check=0;
char channel_name[50];
char channelnamec[50];
char auth_token[33];
int j=0;

if ((j=authtoken_finder())==-1) return;


sscanf(buffer,"create channel %[^,], %s\n",channel_name,auth_token);
FILE *fp;
fp=fopen("channels.txt","a");
fclose(fp);
fp=fopen("channels.txt","r");
if (fp==NULL) {printf("error opening channels.text file\n");errorsender("error opening channels.text file");return;}
while(fgetc(fp)!=EOF){
	fseek(fp, -1L, SEEK_CUR);

fscanf(fp,"%s\n",channelnamec);
if(strcmp(channel_name,channelnamec)==0){errorsender("Channel name is not available.");return;}
}if(fclose(fp)==EOF) {printf("error closing channels.text file\n");return;}
fp=fopen("channels.txt","a");
if (fp==NULL) {printf("error opening channels.text file\n");errorsender("error opening channels.text file");return;}
fprintf(fp, "%s\n",channel_name);
if(fclose(fp)==EOF) {printf("error closing channels.text file\n");return;}
//


char checker[100];
check=0;



int n=0;


fp=fopen("channels.txt","r");
if (fp==NULL) {printf("error opening channels.text file\n");errorsender("error opening channels.text file");return;}

while(1){
	if (fgetc(fp)==EOF) {errorsender("channel don't exists");break;}
	fseek(fp, -1L, SEEK_CUR);
fscanf(fp,"%[^\n]\n",checker);
printf("%s\n",checker);

if(strstr(checker,channel_name)!=NULL){
	 printf("channel found\n");
	 successfulsender();
	 member[j][2]=malloc(100);
	 strcpy(member[j][2],channel_name);
	 int i=0;

	 break;
 }

}
if(fclose(fp)==EOF) {printf("error closing channels.text file\n");return;}

//



}
//the join function checks channels.txt to see if the channel name in the buffer is in the channels.txt or not.if the channel was found it adds the channel name to member array.
//it also gives the joined channel a number using channels array
void join(){
char channel_name[100];
char auth_token[33];
char checker[100];
int check=0;

sscanf(buffer,"join channel %[^,], %s",channel_name,auth_token);
FILE *fp;
int n=0;
int j=0;
if ((j=authtoken_finder())==-1) return;
printf("%d\n",j);
fp=fopen("channels.txt","a");
fclose(fp);
fp=fopen("channels.txt","r");
if (fp==NULL) {printf("error opening channels.text file\n");errorsender("error opening channels.text file");return;}

while(1){
	if (fgetc(fp)==EOF) {errorsender("Channel not found.");break;}
	fseek(fp, -1L, SEEK_CUR);
fscanf(fp,"%[^\n]\n",checker);
printf("%s\n",checker);

if(strstr(checker,channel_name)!=NULL){
	 printf("channel found\n");
	 successfulsender();
	 member[j][2]=malloc(100);
	 strcpy(member[j][2],channel_name);
	 int i=0;

	 break;
 }

}
if(fclose(fp)==EOF) {printf("error closing channels.text file\n");return;}

}
//the logout function changes the member array as if no such authtoken existed.
void logout(){
	char auth_token[33];
	int check=0;
	sscanf(buffer,"logout %s",auth_token);
	int j=0;
	if ((j=authtoken_finder())==-1) return;
	for(;member[j][1]!=NULL;j++)for(int i=0;i<3;i++) member[j][i]=member[j+1][i];

	successfulsender();


return;}
//the sendm function saves the message to channel_message[j] where j is the number found in channels function
//opens messages.txt file and print sth like this: message|username|channel\n
void sendm(){
	FILE *fp;
	fp=fopen("messages.txt","a");
if (fp==NULL) {printf("error opening messages.text file\n");errorsender("error opening messages.text file");return;}



	char auth_token[33];
	char message[100];
//send <message>, <auth_token>
sscanf(buffer,"send %[^,], %s",message,auth_token);

int j=0;
if ((j=authtoken_finder())==-1) return;
if (member[j][2]==NULL) {errorsender("user is not joined in any channel\n");return;}
for(int i=0;i<strlen(message);i++) fprintf(fp,"%c",message[i]);

fprintf(fp,"|");
fprintf(fp, "%s|",member[j][0]);


strcpy(buffer,member[j][2]);

//printf("you reached 1\n" );
if ((channel_finder())==-1) return;

//printf("you reached 2\n" );
int i=0;
fprintf(fp, "%s\n",member[j][2]);
if(fclose(fp)==EOF) {printf("error closing messages.text file\n");return;}

//printf("you reached 3\n" );
successfulsender();

return;}
//the refresh function prints everything from index 0 to the index where channel_message is NULL there
void refresh(){
	char sender[5000];
	char senderhelper [100];
	char auth_token[33];
	int check=0;
	char message[100];
	char username[50];
	char channelname[50];
	int counter;
	counter=0;
	int counter2;
	//
	cJSON *refresher;
	refresher = cJSON_CreateObject();

	cJSON_AddStringToObject(refresher,"type","List");
	//cJSON_AddStringToObject(refresher,"content","user is not part of any channel!");
	//cJSON_AddStringToObject(refresher,"content","user is not part of any channel!");


	//
cJSON *array;

array=cJSON_CreateArray();

	FILE *fp;
	int n=0;
	int j=0;
	if ((j=authtoken_finder())==-1) return;
	counter2=counters[j];
	//printf("counter2 is %d\n", counter2);
if (member[j][2]==NULL) {errorsender("user is not joined in any channel\n");return;}
//strcpy(sender,"{\"type\":\"List\",\"content\":[");
fp=fopen("messages.txt","a");
fclose(fp);
	fp=fopen("messages.txt","r");
	if (fp==NULL) {printf("error opening messages.text file\n");errorsender("error opening messages.text file");return;}

	while(1){
		if (fgetc(fp)==EOF) break;
		fseek(fp, -1L, SEEK_CUR);
	fscanf(fp,"%[^|]|%[^|]|%s\n",message,username,channelname);
	printf("%s\t%s\t%s\n", message,username,channelname);


	strcpy(buffer,member[j][2]);


	if ((channel_finder())==-1) return;


	if(strstr(buffer,channelname)==NULL) continue;

	if(counter2>0) {counter2--;continue;}
	//sprintf(senderhelper, "{\"sender\":\"%s\",\"content\":\"%s\"},",username,message);
	cJSON *arrayhelper;
	arrayhelper=cJSON_CreateObject();

	//printf("you reached 1\n");
	cJSON_AddStringToObject(arrayhelper,"sender",username);
	//printf("you reached 2\n");
	cJSON_AddStringToObject(arrayhelper,"content",message);
	counter++;
	//printf("you reached 3\n");
	cJSON_AddItemToArray(array,arrayhelper);
	//printf("you reached 4\n");



	//strcat(sender,senderhelper);

}
cJSON_AddItemToObject(refresher,"content",array);
	if(fclose(fp)==EOF) {printf("error opening messages.text file\n");return;}
//	int size;
//	sender[strlen(sender)-1]=']';
//	size =strlen(sender);
//	sender[size]='}';
//	sender[size+1]='\n';
//	sender[size+2]='\0';

  printf("%s\n",cJSON_PrintUnformatted(refresher));
	//send(client_socket, sender, sizeof(sender), 0);
	send(client_socket, cJSON_PrintUnformatted(refresher), strlen(cJSON_PrintUnformatted(refresher)), 0);
	counters[j]+=counter;
	return;}
//the listpeople function makes a sender string which contains username of those who are joined into channel using member array
void listpeople(){
	cJSON *lister;
	lister = cJSON_CreateObject();

	//char sender[500];
	int j=0;
	if ((j=authtoken_finder())==-1) return;
	if (member[j][2]==NULL) {errorsender("user is not joined in any channel\n");return;}
  cJSON_AddStringToObject(lister,"type","List");
	//strcpy(sender,"{\"type\":\"List\",\"content\":[");
	cJSON *array;

	array=cJSON_CreateArray();
	for (int i=0;i<100;i++)if(member[i][2]!=NULL){
		if(strcmp(member[j][2],member[i][2])==0){
			printf("the member %s is in the channel",member[i][0]);



			//printf("you reached 1\n");



			//printf("you reached 3\n");
			//cJSON_AddItemToObject(root, "name", cJSON_CreateString(member[i][0]));
			cJSON_AddItemToArray(array,cJSON_CreateString(member[i][0]));
			//printf("you reached 4\n");
			//strcat(sender,"\"");strcat(sender,member[i][0]);
			//strcat(sender,"\",");
		}
		 }
		 cJSON_AddItemToObject(lister,"content",array);
/*int size;
size=strlen(sender);
sender[size]=']';
sender[size+1]='}';
sender[size+2]='\n';
sender[size+3]='\0';*/
//printf("sender is %s\n",sender );
printf("json is %s\n",cJSON_PrintUnformatted(lister));
//send(client_socket, sender, sizeof(sender), 0);
send(client_socket, cJSON_PrintUnformatted(lister), strlen(cJSON_PrintUnformatted(lister)), 0);
}
//quitchannel converts the channel part in member array to NULL
void quitchannel(){
	char auth_token[33];
	sscanf(buffer,"leave %s",auth_token);
	int j=0;
	if ((j=authtoken_finder())==-1) return;

	if(member[j][2]!=NULL) {member[j][2]=NULL;successfulsender();}
	else 	errorsender("user is not joined in any channel\n");
}
