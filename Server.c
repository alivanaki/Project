#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.c"
#include <process.h>
#include <dir.h>
#include <time.h>
char input_request[10000];
int socket1 , i=0;
struct information
{
    char Username[1000];
    char Password[1000];
    char AuthToken[1000];
    char Channel[1000];
    int Refresh;
};
struct information info[1000];
int Build_Socket(int server_socket)
{
    struct sockaddr_in client;
    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    int len = sizeof(client);
    int client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");

    // Function for chatting between client and server
    return client_socket;
}
void login()
{
        char a[1000] , Username[1000] , Password[1000];
        sscanf(input_request+6,"%s",a);
        sscanf(input_request+7+strlen(a),"%s",Password);
        strcpy(Username,a);
        Username[strlen(Username)-1]='\0';
        if (input_request[6+strlen(a)-1]!=',')
        {
            login_invalid_username();
            return;
        }
        int j;
        for (j=0;j<=i;++j)
        {
            if (!strcmp(info[j].Username,Username))
            {
                if (!strcmp(info[j].Password,Password)) login_successful(j);
                else login_wrong_password();
            }
        }
        login_username_not_existed();
}
void login_successful(int n)
{
        char AuthToken[32];
        char *a="qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
        int i;
        srand(time(0));
        for (i=0;i<32;++i)
        AuthToken[i]=a[rand()%52];
        AuthToken[31]='\0';
        cJSON* output_request=cJSON_CreateObject();
        cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("AuthToken"));
        cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(AuthToken));
        send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
        strcpy(info[n].AuthToken,AuthToken);
}
void login_wrong_password()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("Your password is wrong"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void login_username_not_existed()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This Username is not existed"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void login_invalid_username()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("You Should not use space in your username"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void registeration( )
{
        char a[1000] , Username[1000] , Password[1000];
        sscanf(input_request+9,"%s",a);
        sscanf(input_request+10+strlen(a),"%s",Password);
        strcpy(Username,a);
        Username[strlen(Username)-1]='\0';
        if (input_request[9+strlen(a)-1]!=',') {register_fail_invalid_username();
        return;}
        FILE *fptr;
        fptr=fopen("Resources/Users/User_List.txt","r");
            char User_in_server[1000];
            fscanf(fptr,"%s",User_in_server);
            while (!feof(fptr))
            {
                if (!(strcmp(Username , User_in_server)))
                {
                    register_already_exist();
                    fclose(fptr);
                    return;
                }
                fscanf(fptr,"%s",User_in_server);
            }
        fclose(fptr);
        register_successful(Username,Password);

}
void register_already_exist()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This Username is already exist"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void register_successful(char* Username , char* Password)
{
        char path[1000]="Resources/Users/";
        strcat(path,Username);
        strcat(path,".txt");
        FILE *fptr;
        FILE *fptr2;
        fptr=fopen(path,"w");
        fprintf(fptr,"%s\n%s\n",Username,Password);
        fclose(fptr);
        fptr2=fopen("Resources/Users/Pass_List.txt","a");
        fptr=fopen("Resources/Users/User_List.txt","a");
        fprintf(fptr,"%s\n",Username);
        fclose(fptr);
        fprintf(fptr2,"%s\n",Password);
        fclose(fptr2);
        ++i;
        strcpy(info[i].Username,Username);
        strcpy(info[i].Password,Password);
        info[i].Refresh=0;
        cJSON* output_request=cJSON_CreateObject();
        cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(""));
        send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void register_fail_invalid_username()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("Invalid Username(Do not use space in Username)"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void CreateChannel()
{
    char channel_name[1000] , AuthToken[1000] , Creator[1000];
    sscanf(input_request+15,"%s",channel_name);
    sscanf(input_request+16+strlen(channel_name),"%s",AuthToken);
    if (input_request[15+strlen(channel_name)-1]!=',')
    {
        Channel_name_invalid();
        return;
    }
    int k;
    channel_name[strlen(channel_name)-1]='\0';
    int flag=0 ,j;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            strcpy(Creator,info[j].Username);
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        CreateChannel_invalid_AuthToken();
        return;
    }
    FILE *file;
    char Channel_created[100];
    file =fopen("Resources/Channels/Channel_List.txt","r");
    fscanf(file,"%s",Channel_created);
    while (!feof(file))
    {
        if (!strcmp(Channel_created,channel_name))
        {
            CreateChannel_existed( );
            fclose(file);
            return;
        }
        fscanf(file,"%s",Channel_created);
    }
    fclose(file);
    CreateChannel_successful(channel_name,Creator,k);
}
void Channel_name_invalid()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("Please change your Channel name(Do not use space in your name)"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void CreateChannel_existed()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This Channel is already existed"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void CreateChannel_successful(char *channel_name , char *Creator, int k)
{
    strcpy(info[k].Channel,channel_name);
    FILE *fptr;
    fptr=fopen("Resources/Channels/Channel_List.txt","a");
    fprintf(fptr,"%s\n",channel_name);
    fclose(fptr);
    char Message[1000];
    memset(Message,0,1000);
    strcat(Message,Creator);
    strcat(Message," Created ");
    strcat(Message,channel_name);
    char path2[1000]="Resources/Channels/Message/";
    strcat(path2,channel_name);
    strcat(path2,".txt");
    fptr=fopen(path2,"a");
    fprintf(fptr,"%s\n",Message);
    fclose(fptr);
    char path[1000]="Resources/Channels/Sender/";
    memset(Message,0,1000);
    strcat(Message,"Server");
    strcat(path,channel_name);
    strcat(path,".txt");
    fptr=fopen(path,"a");
    fprintf(fptr,"%s\n",Message);
    fclose(fptr);
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(""));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void CreateChannel_invalid_AuthToken()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This Username is not login"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void JoinChannel()
{
    char channel_name[1000] , AuthToken[1000] , Joiner[1000];
    sscanf(input_request+13,"%s",channel_name);
    sscanf(input_request+14+strlen(channel_name),"%s",AuthToken);
    int k;
    if (input_request[13+strlen(channel_name)-1]!=',')
    {
        Channel_name_invalid();
        return;
    }
    channel_name[strlen(channel_name)-1]='\0';
    int flag=0 ,j;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            strcpy(Joiner,info[j].Username);
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    FILE *file;
    char Channel_created[100];
    file =fopen("Resources/Channels/Channel_List.txt","r");
    fscanf(file,"%s",Channel_created);
    while (!feof(file))
    {
        if (!strcmp(Channel_created,channel_name))
        {
            JoinChannel_successful(channel_name,Joiner,k);
            fclose(file);
            return;
        }
        fscanf(file,"%s",Channel_created);
    }
    fclose(file);
    JoinChannel_not_existed();
}
void JoinChannel_successful(char *channel_name,char *Joiner,int k)
{
    strcpy(info[k].Channel,channel_name);
    FILE *fptr;
    char Message[1000];
    memset(Message,0,1000);
    strcat(Message,Joiner);
    strcat(Message," Joined at ");
    strcat(Message,channel_name);
    char path2[1000]="Resources/Channels/Message/";
    strcat(path2,channel_name);
    strcat(path2,".txt");
    fptr=fopen(path2,"a");
    fprintf(fptr,"%s\n",Message);
    fclose(fptr);
    char path[1000]="Resources/Channels/Sender/";
    strcat(path,channel_name);
    strcat(path,".txt");
    fptr=fopen(path,"a");
    fprintf(fptr,"Server\n");
    fclose(fptr);
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(""));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void JoinChannel_not_existed()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This Channel is not existed"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void JoinChannel_invalid_AuthToken()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This Username is not login"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void Logout()
{
    int flag=0 , j;
    char AuthToken[1000];
    sscanf(input_request+7,"%s",AuthToken);
    int k;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;
        }
    }
    if (flag==1)
    {
    memset(info[k].AuthToken,0,1000);
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(""));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
    }
    else
    {
       JoinChannel_invalid_AuthToken();
    }
}
void Send_Message()
{
    char Message[1000] , AuthToken[1000];
    sscanf(input_request,"%*s %[^,], %[^\n]",Message,AuthToken);
    int flag=0  , j , k;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    Send_Message_successful(Message,k);
}
void Send_Message_successful(char *Message,int k)
{
    char path[1000]="Resources/Channels/Sender/";
    strcat(path,info[k].Channel);
    strcat(path,".txt");
    FILE *fptr;
    fptr=fopen(path,"a");
    fprintf(fptr,"%s\n",info[k].Username);
    fclose(fptr);
    char path2[1000]="Resources/Channels/Message/";
    strcat(path2,info[k].Channel);
    strcat(path2,".txt");
    fptr=fopen(path2,"a");
    fprintf(fptr,"%s\n",Message);
    fclose(fptr);
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(""));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void Channel_members()
{
    char AuthToken[1000];
    sscanf(input_request+15,"%s",AuthToken);
    int j , flag=0 , k;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;

        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    char Channel_server[1000];
    FILE *fptr;
    fptr=fopen("Resources/Channels/Channel_List.txt","r");
    fscanf(fptr,"%s",Channel_server);
    while(!feof(fptr))
    {
        if (!strcmp(Channel_server,info[k].Channel))
        {
            Channel_members_successful(k);
            fclose(fptr);
            return;
        }
        fscanf(fptr,"%s",Channel_server);
    }
    fclose(fptr);
    Channel_members_not_existed();
}
void Channel_members_successful(int n)
{
    int j;
    cJSON *output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("List"));
    cJSON *Members=cJSON_CreateArray();
    for (j=0;j<=i;++j)
    {
        if (!strcmp(info[n].Channel,info[j].Channel))
        {
            cJSON_AddItemToArray(Members,cJSON_CreateString(info[j].Username));
        }
    }
    cJSON_AddItemToObject(output_request,"content",(Members));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void Channel_members_not_existed()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This User did not join to any channel"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void Leave()
{
    char AuthToken[1000];
    sscanf(input_request+6,"%s",AuthToken);
    int j , k ,flag=0;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;

        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    Leave_successful(k);
}
void Leave_successful(int n)
{
    char path[1000]="Resources/Channels/Message/";
    strcat(path,info[n].Channel);
    strcat(path,".txt");
    char Message[1000];
    memset(Message,0,1000);
    strcat(Message,info[n].Username);
    strcat(Message," left the channel");
    FILE *fptr;
    fptr=fopen(path,"a");
    fprintf(fptr,"%s\n",Message);
    fclose(fptr);
    char path2[1000]="Resources/Channels/Sender/";
    strcat(path2,info[n].Channel);
    strcat(path2,".txt");
    fptr=fopen(fptr,"a");
    fprintf(fptr,"Server");
    fclose(fptr);
    memset(info[n].Channel,0,1000);
    info[n].Refresh=0;
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString(""));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);

}
void Refresh()
{
    char AuthToken[1000];
    sscanf(input_request+8,"%s",AuthToken);
    int j , k ,flag=0;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;

        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    Refresh_successful(k);
}
void Refresh_successful(int k)
{
    char path[1000]="Resources/Channels/Message/";
    strcat(path,info[k].Channel);
    strcat(path,".txt");
    char path2[1000]="Resources/Channels/Sender/";
    strcat(path2,info[k].Channel);
    strcat(path2,".txt");
    FILE *fptr;
    FILE *fptr2;
    fptr=fopen(path,"r");
    fptr2=fopen(path2,"r");
    char Message[1000] , Sender[1000];
    int counter=0;
    cJSON *output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("List"));
    cJSON *cjson_content=cJSON_CreateArray();
    fgets(Message,1000,fptr);
    fgets(Sender,1000,fptr2);
    while (!feof(fptr))
    {
        Message[strlen(Message)-1]='\0';
        Sender[strlen(Sender)-1]='\0';
        if (counter>=info[k].Refresh)
        {
            cJSON *sender=cJSON_CreateObject();
            cJSON_AddItemToObject(sender,"sender",cJSON_CreateString(Sender));
            cJSON_AddItemToObject(sender,"content",cJSON_CreateString(Message));
            cJSON_AddItemToArray(cjson_content,sender);
        }
        fgets(Message,1000,fptr);
        fgets(Sender,1000,fptr2);
        counter++;
    }
    info[k].Refresh=counter;
    cJSON_AddItemToObject(output_request,"content",cjson_content);
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void FindUser()
{
    char User[1000] , AuthToken[1000];
    sscanf(input_request+9,"%s%s",User,AuthToken);
    AuthToken[31]='\0';
    if (User[strlen(User)-1]!=',')
    {
        FindUser_invalid_username();
        return;
    }
    User[strlen(User)-1]='\0';
    int j , flag=0 , k;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    for (j=0;j<=i;++j)
    {
        if ((!strcmp(info[k].Channel,info[j].Channel))&&(!strcmp(User,info[j].Username))) {FindUser_successful(User,k);
        return;}
    }
    FindUser_Fail();
}
void FindUser_Fail()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("This username did not exist in  this channel"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void FindUser_successful(char *User , int k)
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("We can find this username"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void FindUser_invalid_username()
{
    cJSON* output_request=cJSON_CreateObject();
    cJSON_AddItemToObject(output_request,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(output_request,"content",cJSON_CreateString("Do not use space in the username"));
    send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
void FindMessage()
{
    char AuthToken[1000] , Message[1000];
    sscanf(input_request+12 , "%s%s" ,Message,AuthToken);
    AuthToken[31]='\0';
    Message[strlen(Message)-1]='\0';
    int j , flag=0 , k;
    for (j=0;j<=i;++j)
    {
        if (!strcmp(AuthToken,info[j].AuthToken))
        {
            k=j;
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        JoinChannel_invalid_AuthToken();
        return;
    }
    char channel[1000];
    strcpy(channel , info[k].Channel);
    char path[1000]="Resources/Channels/Message/";
    strcat(path,channel);
    strcat(path,".txt");
    FILE* fptr;
    fptr = fopen (path , "r");
    char Message_input[1000];
    cJSON *output_request=cJSON_CreateObject();
    cJSON *cjson_content=cJSON_CreateArray();
    fgets(Message_input , 1000 , fptr);
    flag = 0;
    while (!feof(fptr))
    {
        int counter=0;
        while (counter < strlen(Message_input))
        {
            char sub_Message[1000];
            sscanf(Message_input+counter , "%s" , sub_Message);
            counter+=strlen(sub_Message)+1;
            if (!strcmp(Message , sub_Message))
            {
                flag=1;
                Message_input[strlen(Message_input)-1] = '\0';
                cJSON_AddItemToArray(cjson_content , cJSON_CreateString(Message_input));
            }
        }
            fgets(Message_input , 1000 , fptr);
    }
        cJSON_AddItemToObject(output_request ,"type" , cJSON_CreateString("Find"));
        cJSON_AddItemToObject(output_request , "content" ,cjson_content);
        send(socket1,cJSON_PrintUnformatted(output_request),1000,0);
}
int main( )
{
    int server_socket, client_socket;
    struct sockaddr_in server;
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
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
    printf("Socket successfully bound..\n");
    char sub_input[1000] , order[1000];
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    mkdir("Resources/Channels/Message");
    mkdir("Resources/Channels/Sender");
    FILE *fptr;
    if (fopen("Resources/Users/User_List.txt","r")==NULL)
    {
        fptr=fopen("Resources/Users/User_List.txt","w");
        fclose(fptr);
    }
    if (fopen("Resources/Users/Pass_List.txt","r")==NULL)
    {
        fptr=fopen("Resources/Users/Pass_List.txt","w");
        fclose(fptr);
    }
    if (fopen("Resources/Channels/Channel_List.txt","r")==NULL)
    {
        fptr=fopen("Resources/Channels/Channel_List.txt","w");
        fclose(fptr);
    }
    char a[1000];
    FILE *fptr2;
    fptr2=fopen("Resources/Users/Pass_List.txt","r");
    fptr=fopen("Resources/Users/User_List.txt","r");
    fscanf(fptr2,"%s",info[i].Password);
    fscanf(fptr,"%s",info[i].Username);
    info[i].Refresh=0;
    while(!feof(fptr))
    {
        ++i;
        info[i].Refresh=0;
        fscanf(fptr,"%s",info[i].Username);
        fscanf(fptr2,"%s",info[i].Password);
    }
    --i;
    fclose(fptr);
    fclose(fptr2);
    while (TRUE)
    {
        socket1=Build_Socket(server_socket);
        recv(socket1,input_request,10000,0);
        sscanf(input_request,"%s",order);
        printf("%s",input_request);
        if (!(strcmp(order , "register")))
        {
            registeration();
        }
        if (!(strcmp(order,"login")))
        {
            login();
        }
        if (!(strcmp(order,"create")))
        {
            CreateChannel();
        }
        if (!(strcmp(order,"join")))
        {
            JoinChannel();
        }
        if (!(strcmp(order,"logout")))
        {
            Logout();
        }
        if (!(strcmp(order,"send")))
        {
            Send_Message();
        }
        if (!(strcmp(order,"channel")))
        {
            Channel_members();
        }
        if (!strcmp(order,"leave"))
        {
            Leave();
        }
        if (!strcmp(order,"Refresh"))
        {
            Refresh();
        }
        if (!strcmp(order,"FindUser"))
        {
            FindUser();
        }
        if (!strcmp(order,"FindMessage"))
        {
            FindMessage();
        }
    }
}
