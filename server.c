#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   1024

int   main( void)
{
   char  buf_c[256];
   int   atoi_int = 0;
   int   server_socket;
   int   client_socket;
   int   client_addr_size;

   struct sockaddr_in   server_addr;
   struct sockaddr_in   client_addr;

   char   buff_rcv[BUFF_SIZE];
   char   buff_snd[BUFF_SIZE];



   server_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == server_socket)
   {
      printf( "server socket fail\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons( 4000);
   server_addr.sin_addr.s_addr= htonl( INADDR_ANY);

   if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr)))
   {
      printf( "bind() fail\n");
      exit( 1);
   }

   if( -1 == listen(server_socket, 5))
   {
      printf( "listen() fail\n");
      exit( 1);
   }

   while( 1)
   {
      client_addr_size  = sizeof( client_addr);
      client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

      if ( -1 == client_socket)
      {
         printf( "accept fail\n");
         exit( 1);
      }
      read ( client_socket, buff_rcv, BUFF_SIZE);
      printf( "receive: %s\n", buff_rcv);
      atoi_int = atoi(buff_rcv);
      if(atoi_int != 0)
      {
	sprintf(buf_c, "sudo raspistill -t 1000 -o image%d.jpg", atoi_int);
	system(buf_c);
	//exit(1);
      }
      close( client_socket);
   }
}