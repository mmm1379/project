#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

char authtoken[50];
int server_socket;
int printsocketcount;//for not printing socketmaker everytime


void socketmaker();
void Register();
void menu();
void login();
void loggedinmenu();
void createchannel();
void joinchannel();
void logout();
void chatmenu();
void sendmessage();
void refresh();
void listpeople();
void quitchannel();
void clear();


int main(){
	int netsocket;//to store socket info;
	char buffer[100];


	menu();
	send(server_socket, "goodbye", 8, 0);

	printf("have a nice time\n" );



//to send buffer to server:
//send(server_socket, buffer, sizeof(buffer), 0);
//to recieve and paste to buffer:
//recv(server_socket, buffer, sizeof(buffer), 0);
/*
socketmaker(); // socket ro ijad mikone
memset(buffer, 0, sizeof(buffer)); //kolle buffer ro 0 mikone
send(server_socket, buffer, sizeof(buffer), 0); //ersale Dastoor
memset(buffer, 0, sizeof(buffer));
recv(server_socket, buffer, sizeof(buffer), 0); //daryafte javad
closesocket(server_socket); //socket ro mibande
*/
//IMPORTANT: akharin khane string \n bashad.



	return 0;
}


void clear(){
    system("@cls||clear");
}

void socketmaker(){
	struct sockaddr_in servaddr;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket == -1) {
clear();printf("Socket creation failed...\n");printsocketcount++;
exit(0);
}
else
if (printsocketcount < 2){printf("Socket successfully created..\n");printsocketcount++;}
// Assign IP and port
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = INADDR_ANY;//0.0.0.0
//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
servaddr.sin_port = htons(12345);
// Connect the client socket to server socket
if (connect(server_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
clear();printf("Connection to the server failed...\n"); printsocketcount++;
exit(0);
}
else
if (printsocketcount < 2){printf("Successfully connected to the server..\n");printsocketcount++;}
return ;
}
void menu(){

	char choice[100];
	int intchoice=0;

	printf("Account menu:\n1: Register\n2: Login\n3: quit app\n");
	scanf("%s",choice);
	if (strcmp(choice,"1")==0) intchoice=1;
	else if (strcmp(choice,"2")==0) intchoice=2;
	else if (strcmp(choice,"3")==0) intchoice=3;
	else memset(choice, 0, sizeof(choice));
	switch (intchoice) {
		case 1: {clear();Register();}
		break;
		case 2: {clear();login();}
		break;
		case 3: return;
		break;
		default: {printf("wrong number\n"); menu();}
		break;
	}
}
void Register(){
	clear();
	char sender[256] = {"register "};
	char reciever[256];
	char username[100];
	char password[100];
	printf("Enter Username\n");
	scanf("%s",username);
	printf("Enter Password\n");
	scanf("%s",password);
	strcat(sender,username);
	strcat(sender,", ");
	strcat(sender,password);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	memset(sender, 0, sizeof(sender));
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);

	if (strstr(reciever,"{\"type\":\"Successful\",\"content\":\"\"}\n")!=0) {
			printf("you successfully created your account\n\n");

			menu();
	}
	else if(strstr(reciever,"this username is not available.")!=0){
	printf("username exists\n\n");
	menu();
	}
	else printf("unexpected error\n"),menu();



}
void login(){

	char sender[256] = {"login "};
	char reciever[256];
	char username[100];
	char password[100];
	printf("Enter Username\n");
	scanf("%s",username);
	printf("Enter Password\n");
	scanf("%s",password);
	strcat(sender,username);
	strcat(sender,", ");
	strcat(sender,password);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);

	if (strstr(reciever,"AuthToken")!=0) {
		sscanf(reciever,"{\"type\":\"AuthToken\",\"content\":\"%[^\"]\"}",authtoken);
			printf("welcome %s\n ",username);
			loggedinmenu();
	}
	else if(strstr(reciever,"Username is not valid")!=0){
	printf("Username is not valid\n\n");
	menu();
}
else if(strstr(reciever,"is already logged in")!=0){
	char choice;
printf("%s is already logged in\n", username);
menu();
}
else if(strstr(reciever,"Wrong password")!=0) printf("wrong password\n" ), login();
else printf("unexpected error\n"),menu();

}
void loggedinmenu(){


char choice[100];
int intchoice=0;

printf("1: create channel\n2: join channel\n3:Logout\n");
scanf("%s",choice);
if (strcmp(choice,"1")==0) intchoice=1;
else if (strcmp(choice,"2")==0) intchoice=2;
else if (strcmp(choice,"3")==0) intchoice=3;
else memset(choice, 0, sizeof(choice));
switch (intchoice) {
	case 1: createchannel();
	break;
	case 2: joinchannel();
	break;
	case 3: logout();
	break;
	default: printf("wrong number\n" ), loggedinmenu();
	break;
}
}
void createchannel(){
	clear();
	char sender[256] = {"create channel "};
	char reciever[256];
	char name[100];
	printf("Enter channel name\n");
	scanf("%s",name);
	strcat(sender,name);
	strcat(sender,", ");
	strcat(sender,authtoken);
	//strcat(sender,"moassesemahak");
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);

	if (strstr(reciever,"Successful")!=0) {
			printf("you successfully created your channel\n\n");
			chatmenu();
	}
	else if(strstr(reciever,"Channel name is not available.")!=0) printf("channel name is already taken\n"),loggedinmenu();
	else printf("unexpected error\n" ),loggedinmenu();


}
void joinchannel(){
	clear();
	char sender[256] = {"join channel "};
	char reciever[256];
	char name[100];
	printf("Enter channel name\n");
	scanf("%s",name);
	strcat(sender,name);
	strcat(sender,", ");
	strcat(sender,authtoken);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);

	if (strstr(reciever,"Successful")!=0) {
			printf("you successfully joined %s\n\n",name);
			chatmenu();
	}
	else if (strstr(reciever,"Channel not found.")!=0) {
			printf("there is no channel with name %s\n", name);
			loggedinmenu();
	}
	else printf("unexpected error\n"), loggedinmenu();




}
void logout(){
	char sender[256]="logout ";
	char reciever[256];
	strcat(sender,authtoken);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);
	if (strstr(reciever,"Successful")!=0){
	menu();}
	else printf("unexpected error\n"),loggedinmenu();
}
void chatmenu(){



	char choice[100];
	int intchoice=0;

	printf("1: send messages\n2: refresh\n3:channel members\n4:leave channel\n");
	scanf("%s",choice);
	if (strcmp(choice,"1")==0) intchoice=1;
	else if (strcmp(choice,"2")==0) intchoice=2;
	else if (strcmp(choice,"3")==0) intchoice=3;
	else if (strcmp(choice,"4")==0) intchoice=4;
	else memset(choice, 0, sizeof(choice));
	switch (intchoice) {
		case 1: sendmessage();
		break;
		case 2: refresh();
		break;
		case 3: listpeople();
		break;
		case 4: quitchannel();
		break;
		default: printf("wrong number\n"),chatmenu();
		break;

}}
void sendmessage(){
	clear();
	char sender[256] = {"send "};
	char reciever[256];
	char name[100];
	printf("write your message\n");
	scanf(" %[^\n]",name);
	strcat(sender,name);
	strcat(sender,", ");
	strcat(sender,authtoken);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);

	chatmenu();

}
void refresh(){

	char sender[256] = {"refresh "};
	//char reciever[100000];
	char *reciever=calloc(1000000,1);
	char name[100];
	char content[100];


	//using calloc instead of having these arrays in functions is better. i know.

	//printf("reciever is %s address is %x \n", reciever, reciever);
	strcat(sender,authtoken);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, 1000000, 0);
	printf("you recieved %s\n",reciever);
	char* ptr=reciever+26;


	printf("******************************************************\n");
	while (strlen(ptr)>3){
	sscanf(ptr,"{\"sender\":\"%[^\"]\",\"content\":\"%[^\"]\"}", name, content);

	printf("%s : %s\n",name , content);
	ptr+= 27+strlen(name)+strlen(content);

}
	printf("******************************************************\n");
	free(reciever);
	chatmenu();



}
void listpeople(){

	char sender[256] = {"channel members "};
  char *reciever=calloc(1000000,1);
	char name[100];




	strcat(sender,authtoken);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, 1000000, 0);
	printf("you recieved %s\n",reciever);
	char* ptr=reciever+26;
	printf("******************************************************\n");
	while (strlen(ptr)>3){
	sscanf(ptr,"\"%[^\"]\"", name);

	printf("%s\n",name );

	ptr+= 3+strlen(name);

}
	printf("******************************************************\n");
	free(reciever);
	chatmenu();




}
void quitchannel(){
	char sender[256] = {"leave "};
	char reciever[256];
	char name[100];
	strcat(sender,authtoken);
	strcat(sender,"\n");
	socketmaker();
	send(server_socket, sender, sizeof(sender), 0);
	clear();recv(server_socket, reciever, sizeof(reciever), 0);
	printf("you recieved %s\n",reciever);
	loggedinmenu();


	return;
}
