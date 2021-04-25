#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sd; //socket description
    struct sockaddr_in server_address;
    char buffer[1000];
    int portNumber;
    char serverIP[29];
    int rc = 0;

    if (argc < 3){
        printf ("usage is client <ipaddr> <port> \n"); //let the user enter the IP address and port number
        exit(1);
    }

    //sd = socket (AF_INET, SOCK_DGRAM, 0);
    sd = socket (AF_INET, SOCK_STREAM, 0);

    portNumber = strtol(argv[2], NULL, 10);
    strcpy(serverIP, argv[1]);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNumber);
    server_address.sin_addr.s_addr = inet_addr(serverIP);

    //handle a "busy signal"
    rc = connect(sd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));

    //if (connect(sd,(struct sockaddr *)&server_address, sizeof(struct sockaddr_in))<0){
    if (rc < 0) {
        close (sd);
        perror ("error connecting to stream socket ");
	exit (1);
    }
    char comamand[10];
    char filename[10];
    int length = 0;
    FILE *fptr;
    for(;;){
        printf("Which of following comamands would you like?: \n");
        printf("SHow files: \n");
        printf("Upload: \n");
        printf("Download: \n");
        printf("bye\n");
        scanf("%s",comamand);
        //printf("Command was '%s'", comamand);
        if (strcmp(comamand,"ls")==0)
        {
            memset (buffer, 0, 1000);
            sprintf (buffer, "ls");
            rc = write(sd, buffer, strlen(buffer));


            if (rc < 0){
                perror("Send to ");
                printf("Sent %d bytes\n", rc);
            }
            memset(buffer, 0, 1000);
            rc = read (sd, buffer, 1000);
            printf ("received this:\n, %s\n", buffer);
        }

        else if(strcmp(comamand, "Upload")==0)
        {
            memset (buffer, 0, 1000);
            sprintf (buffer, "Upload");
            rc = write(sd, buffer, strlen(buffer));



            printf("What file name would you like to upload? ");
            scanf("%s", filename);
            memset (buffer, 0, 1000);
            sprintf (buffer,"%s",filename);
            rc = write(sd, buffer, strlen(buffer));

            fptr = fopen(filename,"rb");
            fseek(fptr, 0, SEEK_END);   
            long fileSize = ftell(fptr);     
             fseek(fptr, 0, SEEK_SET); 

             memset (buffer, 0, 1000);
            sprintf (buffer,"%ld",fileSize);
            rc = write(sd, buffer, strlen(buffer));

            for (int i = 1; i <= fileSize; i++ )
            {
                memset(buffer, 0, 1000);
                //printf("Here\n");
                rc = fread (buffer, 1,1,fptr);
                //printf("Return Code %d\n", rc);
                rc = write(sd, buffer, 1);
                //printf("Return Code %d\n", rc);




            }
            printf("Done\n");
            fclose(0);




        }


        else if (strcmp(comamand, "Download")==0)
        {
             //printf("BIGGGGG: '%s'\n", comamand);
            memset (buffer, 0, 1000);
            sprintf (comamand, "Download");
            rc = write(sd, comamand, strlen(comamand));

            printf("What file name would you like to Download? ");
            memset (filename, 0, 10);
            scanf("%s", filename);
            //printf("This is the file name%s\n ", filename);
            //memset (buffer, 0, 1000);
            //sprintf (buffer,"%s",filename);
            //printf("This is the sd: %d\n ", sd);
            rc = write(sd, filename, strlen(filename));    

            //rc = write(sd, buffer, 100);
            //printf("I am before the open\n");

            if ((fptr = fopen(filename, "w")) == NULL) 
            { // pipe allows me to get output                                            // from ls command      
             printf("Error opening file!\n");       
             return -1;     
            } 
            //strcpy(filename, buffer);  
            //printf("Filename %s\n ", filename); 

            //printf("I am before the read\n");
            memset (buffer, 0, 1000); 
            rc = read(sd, buffer, 10);
            sscanf(buffer,"%10d", &length);
            //printf("Length of file: %d\n", length);

            for (int i = 1; i <= length; i++ )
            {
                memset(buffer, 0, 1000);
                //printf("Here\n");
                rc = read (sd, buffer, 1);
                //printf("After read:\n ");
                rc = fwrite(buffer, 1,1,fptr);
                //printf("After write\n");
                







          }
        printf("Done\n"); 
        fclose(0);

        }


            
        else if(strcmp(comamand, "bye")==0)
        {
            printf("Bye\n");
            close(sd);
            exit(0);
        }     
        
    }
    
    
    
    
    
    memset (buffer, 0, 1000);
    sprintf (buffer, "bye");
    rc = write(sd, buffer, strlen(buffer));
    return 0;
}
