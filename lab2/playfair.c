#include<stdio.h>
#include<string.h>
#include<ctype.h>

/****************************************************************************************
 *          UTILITY FUNCTIONS
 * **************************************************************************************/

//Converts string to upper case
void toUpper(char* str)
{
    char* itr = str;
    while(*itr != '\0')
    {
        *itr = toupper(*itr);
        itr++;
    }
}

int isAlpha(char ch)
{
    if(ch >= 'A' && ch <= 'Z')return 1;
    return 0;
}

int isNumeric(char ch)
{
    if(ch >= '0' && ch <= '9') return 1;
    return 0;
}

//Converts a 1-36 ranged index of 6x6 matrix to i,j form
//and inserts the character ch at that location
void addToMatrix(char ch,int matrix_indx,char keyMatrix[6][6])
{
    keyMatrix[matrix_indx/6][(matrix_indx%6)-1] = ch;
}



//generates a 6x6 key matrix from a given key
void generateKeyMatrix(char* key,char keyMatrix[6][6])
{
    printf("\nGenerating a 6x6 Key Matrix for the key : %s\n",key);

    //First Convert the key to uppercase
    toUpper(key);
    
    //An array to check if the characters are already included in the key matrix
    int char_check[36] = {0};       //0-25 => A-Z & 26-35 => 0-9

    //counter to indicate no of chars added in key matrix (1-36)
    int matrix_index = 1;


    //First, add unique chars from the key
    //the next character of the key
    char* iter = key;
    while(*iter != '\0')
    {
        char ch = *iter;
        iter++;

        if(isAlpha(ch) && char_check[ch - 'A'] == 0)
            char_check[ch - 'A'] = 1;
        else if(isNumeric(ch) && char_check[ch - '0' + 26] == 0)
            char_check[ch - '0' + 26] = 1;
        else
            continue;
        

        //adds character ch to the key matrix as it's matrix_indexth entry
        addToMatrix(ch,matrix_index,keyMatrix);
        matrix_index++;
    }

    //Now add all remaining characters to the matrux
    //
    for(int i=0;i<26;i++)
    {
        if(char_check[i] == 0)
        {
            addToMatrix('A' + i,matrix_index,keyMatrix);
            matrix_index++;
        }
    }

    for(int i=26;i<36;i++)
    {
        if(char_check[i] == 0)
        {
            addToMatrix('0' + i - 26,matrix_index,keyMatrix);
            matrix_index++;
        }
    }
    


    //Done
    //Print the 2d key matrix for reference

    for(int i =0;i<6;i++)
    {
        for(int j=0;j<6;j++)
            printf("%c ",keyMatrix[i][j]);
        printf("\n");
    }
}

/*
 * Searches the key matrix for character ch, 
 * and save i,j value to pos[0] and pos[1] respectively
 */
void searchKeyMatrix(char ch,char keyMatrix[6][6],int pos[2])
{
    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
            if(keyMatrix[i][j] == ch)
            {
                pos[0] = i;
                pos[1] = j;
                return;
            }
}


/*
 * Returns
 *  index = (index + 1)mod6 when encrypt = 1 
 *  index = (index - 1)mod6 when encrypt = 0 ,i.e. while decrypting
 */
int transform(int indx,int encrypt)
{
    if(encrypt == 1)
        return (indx + 1)%6;
    else
    {
        indx = indx - 1;
        if(indx < 0)
            indx = 5;
        return indx;

    }

}


/**
 * Encrypt = 1 -> Encrypt the input file and save to outfile
 * Encrypt = 0 -> Decrypt the input file and save to outfile
 */
void playfair(char* key,char* inpFileName,char* outFileName,int encrypt)
{
    
    //Generate the keymatrix
    char keyMatrix[6][6];
    generateKeyMatrix(key,keyMatrix);
    
    //Open the input and output file's
    FILE* inpFile = fopen(inpFileName,"r");
    FILE* outFile = fopen(outFileName,"w");

    //Check if both files are opened propely
    if(inpFile == NULL)
    {
        printf("Cannot open %s File.\n Exitting \n",inpFileName);
        return;
    }
    if(outFile == NULL)
    {
        printf("Cannot open %s File.\n Exitting \n",outFileName);
        return;
    }

    //Read a char from inp file
    char a;
    while((a = fgetc(inpFile)) != EOF)
    {
        //if a is not alphanumeric, print it directly to output file
        a = toupper(a);
        if(!(isAlpha(a) || isNumeric(a)))
        {
            fputc(a,outFile);
            continue;
        }
        
        //now, a is alphanumeric
        //read next character and store it in b
        char b;

        //we will store all non-alphanumeric chars between a and b in this array
        char buff[100];
        int invalid_len = 0;
        
        while(1)
        {
            //while we do not read a valid second char or reach the eof
            b = fgetc(inpFile);
            if(b == EOF)
            {
                //reached the eof
                b = 'Z';
                break;
            }
                        
            b = toupper(b);
            if(isAlpha(b) || isNumeric(b))
                break;

            //non alpha-numeric save it to buffer
            buff[invalid_len++] = b;
        }

        //we now have three variables
        //a - alphanumeric value
        //buffer - multiple non alphanumeric value occuring b/w a and b
        //b - alphanumeric value

        //find the index of a and b in the key matrix
        int a_indx[2],b_indx[2];
        searchKeyMatrix(a,keyMatrix,a_indx);
        searchKeyMatrix(b,keyMatrix,b_indx);
        

        //case both in same col
        if(a_indx[1] == b_indx[1])      //same col
        {
            //transform will add or subtract 1 modulo 6 depending on
            //whether we are encrypting or decrypting
            a = keyMatrix[transform(a_indx[0],encrypt)][a_indx[1]];
            b = keyMatrix[transform(b_indx[0],encrypt)][b_indx[1]];
        }else if(a_indx[0] == b_indx[0])        //same row
        {
            a = keyMatrix[a_indx[0]][transform(a_indx[1],encrypt)];
            b = keyMatrix[b_indx[0]][transform(b_indx[1],encrypt)];
        }else       //rectangle
        {
            a = keyMatrix[a_indx[0]][b_indx[1]];
            b = keyMatrix[b_indx[0]][a_indx[1]];
        }

        
        buff[invalid_len] = '\0';

        //Now we will first write a to output file
        //then, write the buffer 
        //and last write b 
        fputc(a,outFile);       
        for(int t =0;t<invalid_len;t++)
            fputc(buff[t],outFile);
        fputc(b,outFile);

        //completed. move to next pair
    }
    
    fclose(inpFile);
    fclose(outFile);


}

void encryptFile(char* key,char* inpFile,char* outFile)
{
    printf("\nEncrypting %s using key %s\n",inpFile,key);

    //encrypt the file (1 means encrypt)
    playfair(key,inpFile,outFile,1); 


    printf("Completed. Saved the encrypted file as %s\n",outFile);
}

void decryptFile(char* key,char* inpFile,char* outFile)
{
    printf("\nDecrypting %s using key %s\n",inpFile,key);

    //Decrypt the file. (0 means decrypt)
    playfair(key,inpFile,outFile,0);


    printf("Completed. Saved the decrypted file as %s\n",outFile);
}

int main()
{
    char encryptedFile[] = "cipher.txt";
    char decryptedFile[] = "recover.txt";

    
    int inp;
    do
    {
        printf("Welcome, \n");
        printf("------------------------------------------\n");
        printf("1. Encrypt a file \n2. Decrypt a file\n0. Exit\n");
        printf("Enter a choice (0-2) : ");
        scanf("%d",&inp);

        switch(inp)
        {
            case 1:            
            {
                //Encrypt
                char key[100],inpFile[100];
                printf("Enter Text File Name : ");
                scanf("%s",inpFile);
                getchar();

                printf("Enter Encryption Key : ");
                scanf("%s",key);
                getchar();

                encryptFile(key,inpFile,encryptedFile);
                break;
            }
            case 2:             //Decrypt
            {
                char key[100];
                printf("Enter Encryption Key : ");
                scanf("%s",key);
                getchar();

                decryptFile(key,encryptedFile,decryptedFile);
                break;
            }
            default:
                inp = 0;
        }

        printf("\nPress any key to continue\n");
        getchar();

    }while(inp != 0);
}
