#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



int main ()
{

int sock_descrip; //socket discriptior
int connected_sd;
int rc;

struct sockaddr_in server_address;
struct sockaddr_in from_address;

char buffer [1000];
int flags;

socklen_t fromLength;


sock_descrip = socket(AF_INET, SOCK_STREAM, 0); //creating socket

server_address.sin_family = AF_INET;
server_address.sin_port = htons(24000); //port is 24000 like Sue's Apartment number
server_address.sin_addr.s_addr = INADDR_ANY; //binds to any address on this machine


bind(sock_descrip,(struct sockaddr *)&server_address, sizeof(server_address)); // binding server address to the socket discriptor from line 11,  Listening for data from socket

listen(sock_descrip,5); // how many people do you want on hold
char  user_input;
char user_input2[3]="Bye";

for(;;)
{
    connected_sd = accept(sock_descrip, (struct sockaddr *)&from_address, &fromLength);
    int flag =0;
    int length = 0;
    char fileName[20];
    char dFile[10];
    while (flag == 0) 
    {
         memset (buffer, 0, 1000);
          rc = read(connected_sd, buffer, 100); //willing to accept 100 bytes of data as sepcifed in line 17
            /* assume that the only command i am going to process is LS */   
            printf ("received the following %s\n", buffer);
        if ((rc <=0) || (strcmp(buffer, "bye")==0)){printf("Bye\n");flag = 1;}
        else if (strcmp(buffer, "ls")==0)
        {     

            printf ("got an LS command\n"); // print command     
            memset (buffer, 0, 1000); // always zero out the buffer     
            FILE *fp; //  for the pipe command i will run     
            if ((fp = popen("ls", "r")) == NULL) 
            { // pipe allows me to get output                                            // from ls command      
             printf("Error opening pipe!\n");       
             return -1;     
            }     
             fseek(fp, 0, SEEK_END); // look for end of pipe (how much is read in     
             long pipeSize = ftell(fp);     
             fseek(fp, 0, SEEK_SET);  // set pointer to start of file     
             fread(buffer, 1, pipeSize, fp);// making the assumption < 1000                                    // send data back to sender     
            rc = write (connected_sd, buffer, 1000);
            pclose (fp);

        }
        else if (strcmp(buffer, "Upload")==0)
        {
            printf ("got an Upload command\n"); // print command     
            memset (buffer, 0, 1000); // always zero out the buffer     
            FILE *fp; //  for the pipe command i will run

            rc = read(connected_sd, buffer, 100);     
            if ((fp = fopen(buffer, "w")) == NULL) 
            { // pipe allows me to get output                                            // from ls command      
             printf("Error opening pipe!\n");       
             return -1;     
            } 
            strcpy(fileName, buffer);  
            printf("Filename %s\n ", fileName);  
            memset (buffer, 0, 1000); 
            rc = read(connected_sd, buffer, 100);
            sscanf(buffer,"%d", &length);
            printf("Length of file: %d\n", length);

            for (int i = 1; i <= length; i++ )
            {
                memset(buffer, 0, 1000);
                //printf("Here\n");
                rc = read (connected_sd, buffer, 1);
                //printf("After read:\n ");
                rc = fwrite(buffer, 1,1,fp);
                //printf("After write\n");
                


            }
            printf("Done\n");
            fclose(0);

            
             
        }


        else if(strcmp(buffer, "Download")==0)
        {
            printf("Got a download command\n");
            memset (buffer, 0, 1000);
            //sprintf (buffer, "Upload");
            //rc = write(connected_sd, buffer, strlen(buffer));
             FILE *fptr;



            memset (buffer, 0, 1000);
            rc = read(connected_sd, buffer,100);
            strcpy(dFile,buffer);
           // printf("Return Code is: %d\n", rc);
            //printf("Buffer is '%s'\n",buffer);
            //printf("DFILE: %s\n", dFile);
            fptr = fopen(dFile,"r");
            fseek(fptr, 0, SEEK_END);   
            long fileSize = ftell(fptr);     
             fseek(fptr, 0, SEEK_SET); 

             memset (buffer, 0, 1000);
            sprintf (buffer,"%10ld",fileSize);
            rc = write(connected_sd, buffer, strlen(buffer));
            printf("Filename %s\n ", dFile);
            printf("Length of file: %ld\n", fileSize);
            for (int i = 1; i <= fileSize; i++ )
            {
                memset(buffer, 0, 1000);
                //printf("Here\n");
                rc = fread (buffer, 1,1,fptr);
                //printf("Return Code %d\n", rc);
                rc = write(connected_sd, buffer, 1);
                //printf("Return Code %d\n", rc);




            }
            printf("Done");
            fclose(0);




        } 
    }
    //close (connected_sd);
    

    ///printf("Recieved the following %s\n", buffer);
    return 0;
}

}
