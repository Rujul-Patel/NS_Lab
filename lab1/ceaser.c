#include<stdio.h>
#include<stdlib.h>

int ceaserEncrypt(int key,char* fileName,char* out_fileName)
{
    FILE *inpFile;
    inpFile = fopen(fileName,"r");
    
    if(inpFile == NULL)
    {
        printf("Error !!! File Not Found \n\n");
        return -1;
    }

    FILE *outFile;
    outFile = fopen(out_fileName,"w");

    int ch;
    while((ch = fgetc(inpFile)) != EOF)
        fputc((ch + key)%256,outFile);
    
    fclose(inpFile);
    fclose(outFile);
    return 1;
}


int ceaserDecrypt(int key,char* fileName,char* out_fileName)
{
    FILE *inpFile;
    inpFile = fopen(fileName,"r");

    FILE *outFile;
    outFile = fopen(out_fileName,"w");

    int ch;
    while((ch = fgetc(inpFile)) != EOF)
        fputc((ch - key)%256,outFile);
    
    fclose(inpFile);
    fclose(outFile);

    return 1;
}



//Reads first 100 characters from file and stores in a buffer array
void getFirstFewChars(char* filename,char* buff)
{
    FILE *inpFile;
    inpFile = fopen(filename,"r");

    for(int i=0;i<100;i++)
        buff[i] = fgetc(inpFile);

    fclose(inpFile);
}



int bruteForce(char* fileName,char* out_file)
{
    //Brute-force to find key

    //Read first 100 chars from encrypted file
    char buff[100];
    getFirstFewChars(fileName,buff);
    getchar();
    
    int key = 1;
    //Run the below code forever, till we find a key
    //
    do
    {
        //Decrypting  using set of 10 keys at a time
        //That is, decrypt using keys 1 to 10, if key not found then 
        //decrypt using 11 to 20 and continue.
        printf("\nTrying Keys from %d to %d \n\n",key,key+10);
        printf("Key\tDecrypted Sample\n");
        
        for(int k=0;k<10;k++){
            printf("%d\t",key);
            
            //Decrypt the buffer(100 chars) using key
            for(int i=0;i<100;i++)
                printf("%c",(buff[i] - key)%256);
            printf("\n");
            key++;
        }    

        
        printf("\nFound a Key ? (y/n) : ");

        char inp_chr;
        scanf("%c",&inp_chr);
        getchar();

        if(inp_chr == 'y' || inp_chr == 'Y')
        {
            //key found
            printf("\nEnter value of key : ");
            scanf("%d",&key);
            getchar();
            break;
        }else
            printf("\n Continuing with next 10 \n");

        if(key >= 256)break;
            
    }while(1);

    printf("\n\n Found Key %d. Saved Decrypted Output to %s\n",key,out_file);
    ceaserDecrypt(key,fileName,out_file);
    return 0;
}

int freq_analysis(char* fileName,char* out_file)
{
    //english frequency 
    char eng_freq[26] = {'e','t','a','o','i','n','s','r','h','d','l','u','c','m','f','y','w','g','p','b','v','k','x','q','j','z'};
    getchar();

    //open encrypted file
    FILE *inpFile;
    inpFile = fopen(fileName,"r");

    //Create frequency map
    int text_freq[256] = {0};
    
    //Now count the frequency of each character 
    int ch;
    while((ch = fgetc(inpFile)) != EOF)
        text_freq[ch-1] += 1;
    
    fclose(inpFile);


    inpFile = fopen(fileName,"r");

    char buff[100];
    for(int i=0;i<100;i++)
        buff[i] = fgetc(inpFile);
   
    int key = -1;
    do
    {
        int keyFound = 0;

        printf("\nTrying 10 highest frequency Chars...\n\n");
        
        printf("Char\tASCII\tFreq\tKey\tDecrypted Sample by comparing to English Character Frequency\n");
        int cnt = 0;
        while(cnt++ < 10){

            //Find the highest freq
            int highest = 0;
            for(int i=0;i<256;i++)
                if(text_freq[i] > text_freq[highest])
                        highest = i;

            //Try this key
            key = (highest + 1) - eng_freq[0];
            if(key < 0)
                key = 256 + key;


            printf("%c\t %d\t %d\t %d\t",(highest+1),(highest +1),text_freq[highest],key);
            text_freq[highest] = 0;

            for(int i=0;i<100;i++)
                printf("%c",(buff[i] - key)%256);
            printf("\n");
        }    

        
    
        printf("\nFound a Key ? (y/n) : ");
        char inp_chr;
        scanf("%c",&inp_chr);
        getchar();

        if(inp_chr == 'y' || inp_chr == 'Y')
        {
            //key found
            printf("\nEnter value of key : ");
            scanf("%d",&key);
            getchar();
            break;
        }else
            printf("\n Continuing with next 10 \n");
            

    }while(1);

    printf("\n\n Found Key %d. Saved Decrypted Output to %s\n",key,out_file);
    ceaserDecrypt(key,fileName,out_file);

}


int main()
{
    char inputFile[] = "Plaintext.txt";
    char encryptedFile[] = "Cipher.txt";
    char decryptedFile[] = "Recover.txt";


    int inp;
    do
    {
        printf("Welcome, \n");
        printf("------------------------------------------\n");
        printf("1. Encrypt a file \n2. Decrypt a file\n3. Brute-Force\n4. Frequency Analysis\n0. Exit\n");
        printf("Enter a choice (0-4) : ");
        scanf("%d",&inp);

        switch(inp)
        {
            case 1:             //Encrypt
            {
                int key;
                printf("Enter Encryption Key : ");
                scanf("%d",&key);

                if(ceaserEncrypt(key,inputFile,encryptedFile))
                    printf("Encrypted File saved : %s\n",encryptedFile);
                else
                    printf("Task Failed!!\n");
                break;
            }
            case 2:             //Decrypt
            {
                    int key;
                    printf("Enter Decryption Key : ");
                    scanf("%d",&key);

                    if(ceaserDecrypt(key,encryptedFile,decryptedFile))
                        printf("Decrypted File saved : %s\n",decryptedFile);
                    else
                        printf("Task Failed!\n");
                break;
            }
            case 3 :            //Brute-Force
            {
                //bruteForce(inputFile,outputFile);
                bruteForce(encryptedFile,decryptedFile);
                break;
            }   
            case 4:         //Frequency-Analysis
            {
                //freq_analysis(inputFile,outputFile);
                freq_analysis(encryptedFile,decryptedFile);
                break;
            }
            default:
                inp = 0;
        }

        printf("\nPress any key to continue\n");
        getchar();
        getchar();

    }while(inp != 0);
}//main end
