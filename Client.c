#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.c"
#include <conio.h>
#include <windows.h>
//Mahdi Abootorabi
char AuthToken[200];
char type[10000], content[10000];
int socket_saz( )
{
    int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	//else
		//printf("Socket successfully created..\n");
	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
    return client_socket;
	// Function for chat
	//chat(client_socket);

	// Close the socket
	//closesocket(client_socket);
}
void SetColor(int ForgC)
{
    WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
void send_and_recive_array (char request[] , char type[10000] , char content[10000])
{
    SetColor(0);
    int a , i;
    char javab[1000];
    memset(javab , 0 , 1000);
    a=socket_saz();
    send(a,request,strlen(request),0);
    recv(a,javab,1000,0);
    cJSON* translate1=cJSON_Parse(javab);
    cJSON* translate=cJSON_GetObjectItem(translate1,"content");
    for (i=0;i<cJSON_GetArraySize(translate);++i)
    {
    cJSON* content_array=cJSON_GetArrayItem(translate,i);
    cJSON* content_cjson=cJSON_GetObjectItem(content_array,"content");
    cJSON* sender=cJSON_GetObjectItem(content_array,"sender");
    strcpy(type,sender->valuestring);
    strcpy(content,content_cjson->valuestring);
    printf("%s : %s\n",type,content);}
}
void send_and_recive(char request[] , char type[10000] , char content[10000])
{
    int a;
    char javab[1000];
    memset(javab, 0, 1000);
    a=socket_saz();
    send(a,request,strlen(request),0);
    recv(a,javab,1000,0);
    cJSON* translate=cJSON_Parse(javab);
    cJSON* type_cjson=cJSON_GetObjectItem(translate,"type");
    cJSON* content_cjson=cJSON_GetObjectItem(translate,"content");
    strcpy(type,type_cjson->valuestring);
    strcpy(content,content_cjson->valuestring);
}
int login( )
{
    system("cls");
    int flag=1;
    char username[1000],password[1000],server[1000];
    memset(server , 0 , 1000);
    printf("Sign in\n");
    printf("Enter your Username : ");
    gets(username);
    printf("Enter your Password : ");
    gets(password);
    strcat(server,"login ");
    strcat(server,username);
    strcat(server,", ");
    strcat(server,password);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    system("cls");
    if (!strcmp(type,"Error")) {
    printf("%s\n-----------------\nPress Any Key to continue\n",content);
    getch();
    system("cls");
    flag=0;}
    if (flag==1)
    {
    memset(AuthToken,0,200);
    strcpy(AuthToken,content);
    }
    return flag;

}
int registeration( )
{
    system("cls");
    int flag=1;
    char username[1000], password[1000], server[1000];
    memset(server, 0, 1000);
    printf("Sign up\n");
    printf("Enter your Username : ");
    gets(username);
    printf("Enter your Password : ");
    gets(password);
    strcat(server, "register ");
    strcat(server,username);
    strcat(server,", ");
    strcat(server,password);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    system("cls");
    if (!strcmp(type,"Error")) {
    system("cls");
    printf("%s\n-----------------------------\nPress Any Key to continue\n",content);
    getch();
    system("cls");
    flag=0;}
    return flag;
}
int Channel_creat( )
{
    int flag=1;
    system("cls");
    char Channel_name[1000],server[1000];
    memset(server , 0 , 1000);
    printf("Please Enter your Channel Name : ");
    gets(Channel_name);
    strcat(server,"create channel ");
    strcat(server,Channel_name);
    strcat(server,", ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    system("cls");
    if (!strcmp(type,"Error")) {
    printf("%s\n-----------------\nPress Any Key to continue\n",content);
    getch();
    system("cls");
    flag=0;}
    return flag;
}
int Join_channel( )
{
    int flag=1;
    system("cls");
    char Channel_name[1000],server[1000];
    memset(server , 0 , 1000);
    printf("Please Enter your Channel Name : ");
    gets(Channel_name);
    strcat(server,"join channel ");
    strcat(server,Channel_name);
    strcat(server,", ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    system("cls");
    if (!strcmp(type,"Error")) {
    printf("%s\n-----------------\nPress Any Key to continue\n",content);
    getch();
    system("cls");    flag=0;}
    return flag;
}
int logout()
{
    int flag=1;
    char server[1000];
    memset(server , 0 , 1000);
    strcat(server,"logout ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    system("cls");
    if (!strcmp(type,"Error")) {
    printf("%s\n-----------------\nPress Any Key to continue\n",content);
    getch();
    system("cls");
    flag=0;}
    return flag;
}
void SendMessag( )
{
    system("cls");
    char server[1000] , message[500];
    memset(server , 0 , 1000);
    gets(message);
    strcat(server,"send ");
    strcat(server,message);
    strcat(server,", ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    printf("Enter Any key to continue\n");
    getch();
    system("cls");
}
void leavechannel( )
{
    system("cls");
    char server[1000];
    memset(server , 0 , 1000);
    strcat(server,"leave ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive(server,type,content);
    printf("%s\n-----------------\nPress Any Key to continue\n",content);
    getch();
    system("cls");
}
void refresh( )
{
    system("cls");
    char server[1000];
    memset(server , 0 , 1000);
    strcat(server,"Refresh ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive_array(server,type,content);
    printf("Enter Any key to continue\n");
    getch();
    system("cls");
}
void members( )
{
    system("cls");
    char server[1000];
    memset(server , 0 , 1000);
    strcat(server,"channel members ");
    strcat(server,AuthToken);
    strcat(server,"\n");
    send_and_recive_members(server,type,content);
    printf("Enter Any key to continue\n");
    getch();
    system("cls");
}
void send_and_recive_members(char request[] , char type[10000] , char content[10000])
{
    int a , i;
    char javab[10000];
    memset(javab , 0 , 10000);
    a=socket_saz();
    send(a,request,strlen(request),0);
    recv(a,javab,10000,0);
    cJSON* translate=cJSON_Parse(javab);
    cJSON* content_cJSON=cJSON_GetObjectItem(translate,"content");
    for (i=0;i<cJSON_GetArraySize(content_cJSON);++i)
    {
        cJSON* content1=cJSON_GetArrayItem(content_cJSON,i);
        strcpy(content , content1->valuestring);
        printf("%s\n",content);
    }
}
void send_and_recive_Find(char request[] , char type[10000] , char content[10000])
{
    int a , i;
    char javab[10000];
    memset(javab , 0 , 10000);
    a=socket_saz();
    send(a,request,strlen(request),0);
    recv(a,javab,10000,0);
    cJSON* translate=cJSON_Parse(javab);
    cJSON* type_cJSON = cJSON_GetObjectItem(translate , "type");
    if (!strcmp(type_cJSON->valuestring , "Error"))
    {
            printf("Error");
    }
    else
    {
    cJSON* content_cJSON=cJSON_GetObjectItem(translate,"content");
    for (i=0;i<cJSON_GetArraySize(content_cJSON);++i)
    {
        cJSON* content1=cJSON_GetArrayItem(content_cJSON,i);
        strcpy(content , content1->valuestring);
        printf("%s\n",content);
    }
    }
    strcpy(type , type_cJSON->valuestring);
}
int FindUser()
{
    system("cls");
    printf("\n");
    char server[1000] , User[1000];
    memset(server,0,1000);
    strcat(server,"FindUser ");
    printf("Please Enter the username that you want to find : ");
    gets(User);
    strcat(server,User);
    strcat(server,", ");
    strcat(server,AuthToken);
    send_and_recive(server,type,content);
    printf("%s\n",content);
    printf("Press Any to continue\n");
    getch();
    system("cls");
    if (!strcmp(type,"Error")) return 0;
    return 1;
}
int FindPm()
{
    system("cls");
    char server [1000] , Message[1000];
    memset(server , 0  , 1000);
    strcat(server , "FindMessage ");
    printf("Please Enter the message you search for : ");
    gets(Message);
    strcat(server,Message);
    strcat(server ,", ");
    strcat(server,AuthToken);
    send_and_recive_Find(server , type , content);
    printf("Press Any to continue\n");
    getch();
    system("cls");
     if (!strcmp(type,"Error")) return 0;
    return 1;
}
int main( )
{
    system("COLOR F8");
    int a , d=0 , result;
    char nu;
    while (TRUE)
    {
        SetColor(0);
    printf("Account Menu\n");
        SetColor(1);
    printf("1: Register\n");
        SetColor(2);
    printf("2: Login\n");
        SetColor(4);
    printf("3: Exit\n");
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int counter = 1;
    COORD position = {0 , counter};
    SetConsoleCursorPosition(hconsole , position);
    int Value1 = 1 , Value2 = 1;
    Value1 = getch();
    if (Value1 == 49) counter = 1;
    else if (Value1 == 13) counter = 1;
    else if (Value1 == 50) counter = 2;
    else if (Value1 == 51) counter = 3;
    else
    {
        while (TRUE)
        {
        Value2=getch();
        if (Value2 == 80)
        {
           if (counter<3)
           {
               counter++;
               COORD position = {0 , counter};
               SetConsoleCursorPosition(hconsole , position);
           }
        }
        if (Value2 == 72)
        {
            if (counter>1)
            {
                counter--;
                COORD position = {0 , counter};
                SetConsoleCursorPosition(hconsole , position);
            }
        }
        if (Value2 == 49)
        {counter = 1;
        break;}
        if (Value2 == 50)
        {counter = 2;
        break;}
        if (Value2 == 51)
        {counter = 3;
        break;}
        if (Value2 == 13) break ;
    }
    }
    if (counter==3) {
    system("cls");
    break ;
    }
    if (counter==1) result=registeration();
    if (counter==2) result=login();
    if (counter==1 && result==1) result=login();
    if (result==0)
    {
        //printf("\n");
        continue;
    }
    while (TRUE)
    {
    SetColor(0);
    printf("Main Menu\n");
    SetColor(9);
    printf("1: Create Channel\n");
    SetColor(3);
    printf("2: Join Channel\n");
    SetColor(5);
    printf("3: Logout\n");
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    counter = 1;
    COORD position = {0 , counter};
    SetConsoleCursorPosition(hconsole , position);
    Value1 = 1;
    Value2 = 1;
    Value1 = getch();
    if (Value1 == 49) counter = 1;
    else if (Value1 == 13) counter = 1;
    else if (Value1 == 50) counter = 2;
    else if (Value1 == 51) counter = 3;
    else
    {
        while (TRUE)
        {
        Value2=getch();
        if (Value2 == 80)
        {
           if (counter<3)
           {
               counter++;
               COORD position = {0 , counter};
               SetConsoleCursorPosition(hconsole , position);
           }
        }
        if (Value2 == 72)
        {
            if (counter>1)
            {
                counter--;
                COORD position = {0 , counter};
                SetConsoleCursorPosition(hconsole , position);
            }
        }
        if (Value2 == 49)
        {counter = 1;
        break;}
        if (Value2 == 50)
        {counter = 2;
        break;}
        if (Value2 == 51)
        {counter = 3;
        break;}
        if (Value2 == 13) break ;
        }
    }
    if (counter == 1) result=Channel_creat();
    if (counter == 2) result=Join_channel();
    if (counter == 3) result=logout();
    if (result==0) continue;
    if (counter == 3 && result==1) break;
    if ((counter == 1 || counter == 2)&&(result==1)){
    while (TRUE)
    {
    SetColor(2);
    printf("1: Send Message\n");
    SetColor(3);
    printf("2: Refresh\n");
    SetColor(4);
    printf("3: Channel Members\n");
    SetColor(6);
    printf("4: Leave Channel\n");
    SetColor(5);
    printf("5: FindUser\n");
    SetColor(0);
    printf("6: FindPm\n");
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    counter = 1;
    COORD position = {0 , counter-1};
    SetConsoleCursorPosition(hconsole , position);
    Value1 = 1;
    Value2 = 1;
    Value1 = getch();
    if (Value1 == 49) counter = 1;
    else if (Value1 == 13) counter = 1;
    else if (Value1 == 50) counter = 2;
    else if (Value1 == 51) counter = 3;
    else if (Value1 == 52) counter = 4;
     else if (Value1 == 53) counter = 5;
    else if (Value1 == 54) counter = 6;
    else
    {
        while (TRUE)
        {
        Value2=getch();
        if (Value2 == 80)
        {
           if (counter<6)
           {
               counter++;
               COORD position = {0 , counter-1};
               SetConsoleCursorPosition(hconsole , position);
           }
        }
        if (Value2 == 72)
        {
            if (counter>0)
            {
                counter--;
                COORD position = {0 , counter-1};
                SetConsoleCursorPosition(hconsole , position);
            }
        }
        if (Value2 == 49)
        {counter = 1;
        break;}
        if (Value2 == 50)
        {counter = 2;
        break;}
        if (Value2 == 51)
        {counter = 3;
        break;}
        if (Value2 == 52)
        {counter = 4;
        break;}
        if (Value2 == 53)
        {counter = 5;
        break;}
        if (Value2 == 54)
        {counter = 6;
        break;}
        if (Value2 == 13) break ;
        }
    }
    if (counter == 1) SendMessag();
    else if (counter == 2) refresh() ;
    else if (counter == 3) members() ;
    else if (counter == 4) {leavechannel();
    break;}
    else if (counter == 5) result = FindUser();
    else if (counter == 6) result = FindPm();
    }}}}
}
