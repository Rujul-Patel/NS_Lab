#include<time.h>
#include<math.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>

void sendMsg(int sockid,char* msg){
    send(sockid,msg,strlen(msg),0);
}


void recvMsg(int sockid,char* buff)
{
    recv(sockid,buff,100,0);
}


//Connects the client to server
int connectToServer()
{
    int sockid;
    if((sockid = socket(AF_INET,SOCK_STREAM,0)) <= 0)
    {
        printf("Failed to create socket. Quiting Server!!!\n");
        exit(-1);
    }
    
    //defining the address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(55800);
    
    //Connect to server
    if (connect(sockid,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("Cannot Connect to server\n");
        exit(-1);
    }
    else
        printf("Connected with Server\n");
    
    //connected to server at this stage
    return sockid;
}







int compute_exp_modulo(int a, int b, int p) {
    long long x = 1, y = a;
    while (b > 0) {
        if (b % 2 == 1)
            x = (x * y) % p;
        y = (y * y) % p;
        b /= 2;
    }
    return (int)(x % p);
}
















int main()
{
    srand(time(NULL));

    printf("Deffie-Hellman Key Exchange, \n\n Client Program\n");
    printf("-------------------------------------\n");
    
    //connect with server
    int sockfd = connectToServer();
    
    /***** Connected with server *****/
    char buff[100];
    char msg[100];

    //Requesting server to start key exchange
    printf("Requesting server for key exchange\n");
    sendMsg(sockfd,"Request for Key Exchange\n");    
    
    recvMsg(sockfd,buff); 
    printf("Msg from Server => %s\n",buff);

    //Receive p and g from server
    int p,g;

    recvMsg(sockfd,buff);
    p = atoi(buff);
    recvMsg(sockfd,msg);
    g = atoi(msg);

    printf("Msg from Server => p:%d g:%d \n\n",p,g);
    
    
    //generate private value and private key
    int x= rand()%(p-1) + 1;
    int r1 = compute_exp_modulo(g,x,p);

    //send r1 to server
    printf("Generated X : %d , R1 : %d \n",x,r1);
    printf("Sending R1 to server\n\n");
    
    sprintf(buff,"%d",r1);
    sendMsg(sockfd,buff);


    //receive r2 from server
    recvMsg(sockfd,buff);
    int r2 = atoi(buff);
    printf("Msg from Server => R2 : %d\n\n",r2);

    
    int shared_secret = compute_exp_modulo(r2,x,p);
    printf("\n\nShared Secret calculated by clients is %d\n\n",shared_secret);






    





}
