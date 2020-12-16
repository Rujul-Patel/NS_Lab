#include<time.h>
#include<math.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


void sendMsg(int sockid,char* msg){
    send(sockid,msg,strlen(msg),0);
}


void recvMsg(int sockid,char* buff)
{
    recv(sockid,buff,100,0);
}


//generator , private key, prime
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

    printf("Deffie-Hellman Key Exchange\nServer Program\n");
    printf("------------------------------------\n\n");


    //Create and start the server
    int server_sockid,client_sockid;
    if((server_sockid = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("Error, cannot create socket!!!\n");
        exit(-1);
    }

    //defining the address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(55800);

    //binding socket
    if( bind(server_sockid,(struct sockaddr*) &addr,sizeof(addr)) < 0){
        printf("Failed to bind socket. Quiting!!! \n");
        exit(-1);
    }
    listen(server_sockid,5);
    
    /***** Server Started, wait for client to connect ****/
    printf("Server Started. Waiting for client to connect...\n");
    if((client_sockid = accept(server_sockid,NULL,NULL)) < 0)
    {
        printf("Error, Cannot connect with client\n");
        exit(-1);
    }
    printf("\nConnected with Client\n\n");



    /***** Connected with Client ******/

    char buff[100];
    char msg[100];
    
        
    //Client Requests to initiate Key Exchange
    recvMsg(client_sockid,buff);
    printf("Msg from Client => %s",buff);
    
    printf("Acknowledging Client Request\n");
    sendMsg(client_sockid,"Request Accepted\n");

    
    
    //Generate a prime P and Generator, G and send to server
    printf("\nGenerating P, G and sending it to client\n");
    int p = 23;
    int g = 9;

    sprintf(msg,"%d",p);
    sendMsg(client_sockid,msg);
    sprintf(msg,"%d",g);
    sendMsg(client_sockid,msg);
    printf("Sent P:%d G:%d to client\n\n",p,g);



    //recieve r1 from client
    recvMsg(client_sockid,buff);
    int r1 = atoi(buff);
    printf("Msg from Client => R1 : %d\n\n",r1);

    //generate private value and private key
    int x = rand()%(p-1) + 1;
    int r2 = compute_exp_modulo(g,x,p);
    
    //send r2 to client
    printf("Generated X : %d , R2 : %d \n",x,r2);
    printf("Sending R2 to client\n\n");

    sprintf(buff,"%d",r2);
    sendMsg(client_sockid,buff);

    int shared_secret = compute_exp_modulo(r1,x,p);
    printf("\n\nShared Secret calculated by clients is %d\n\n",shared_secret);


    close(server_sockid);

}














