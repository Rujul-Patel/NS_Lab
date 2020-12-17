#include<stdio.h>
#include<string.h>
#include<ctype.h>


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




int main()
{


    
    //1st generate playfair matrix
    char key[100];
    printf("\nEnter Encryption Key : ");
    scanf("%s",key);
    getchar();

    
    printf("\n Generating Playfair Matrix \n\n");
    char keyMatrix[6][6];
    generateKeyMatrix(key,keyMatrix);

   
    char playfairkey[] = "DFAVXG";
    
    char message[100];
    printf("\nEnter Message to Encrypt\n");
    scanf("%s",message);


    //Phase 1 Encryption
    char encrypted[100];
    int enc_indx = 0;

    char* iter = message;
    while(*iter != '\0')
    {
        char ch = *iter;
        iter++;

        int x,y;

        //loop the message char by char
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                if(keyMatrix[i][j] == ch)
                {
                    x = i;
                    y = j;
                    break;
                }

        //found the location replace with word at index
        encrypted[enc_indx++] = playfairkey[x];
        encrypted[enc_indx++] = playfairkey[y];

    }

    encrypted[enc_indx] = '\0';
    printf("Messge After Phase 1 Encryption %s\n\n",encrypted);


    //---------------------------------------
    //  PHASE 1 COMPLETED
    //  ----------------------------

    char transpositionKey[] = "MYKEYS\0";
    int transKeyLength = 6;
    int noOfRows = enc_indx/transKeyLength;


    int t = 0;
    //matrix
    char transmat[noOfRows][transKeyLength];
    
    //fill the transposition matrix
    for(int i=0;i<(enc_indx/6);i++)
    {
        for(int j=0;j<6;j++)
            transmat[i][j] = encrypted[t++];
    }

    printf("Transformation Key %s\n",transpositionKey);
    printf("Original Transformation Matrix\n");
    for(int i=0;i<(enc_indx/6);i++)
    {
        for(int j=0;j<6;j++)
            printf("%c ",transmat[i][j]);
        printf("\n");
    }

    
    //sorting the transposition key
    int sortedIndex[6];
    int currIndex = 0;

    for(char tmp='A';tmp<='Z';tmp++)
    {
        //search if the char is in transposeKey
        for(int t2=0;t2<6;t2++)
        {
            if(transpositionKey[t2] == tmp)
                sortedIndex[currIndex++] = t2;
        }
    }
    
    printf("Sorted Columns Order\n\n");
    for(int i=0;i<6;i++)
    {
        printf("%d ",sortedIndex[i]);
    }

    
    printf("\nFinal Transformation Matrix\n");
    //now print the final encrypted string according to the order
    for(int i=0;i<enc_indx/6;i++)
    {
        for(int j=0;j<6;j++)
            printf("%c ",transmat[i][sortedIndex[j]]);
        printf("\n");
    }

    printf("\nFinal Encrypted String : \n");
    //now print the final encrypted string according to the order
    for(int i=0;i<enc_indx/6;i++)
    {
        for(int j=0;j<6;j++)
            printf("%c",transmat[i][sortedIndex[j]]);
    }







    printf("\nPress any key to continue\n");
    getchar();










}
