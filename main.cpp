
unsigned long plainText=0;
unsigned long inputKey=0;
unsigned long arrayOfLongsSize=0;

#include <stdio.h>
#include <stdlib.h>
/***/
int roundShifts[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
void generateKeys(unsigned long* keys,unsigned long key);
void splitKey(unsigned long key,unsigned long* leftKey,unsigned long* rightKey);
unsigned long S_box(unsigned long number);
unsigned long Permutation(int sizeOfData,unsigned int *permutationArray,unsigned int size,unsigned long* leftPermutedData,unsigned long* rightPermutedData);
unsigned long leftCircularShift(unsigned long number,int shifts);
void splitKey(unsigned long key,unsigned long* leftKey,unsigned long* rightKey);
unsigned int S_boxValues[8][4][16]=
        {{
                 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
                 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
                 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
                 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
         },
         {
                 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
                 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
                 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
                 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
         },
         {
                 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
                 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
                 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
                 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
         },
         {
                 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
                 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
                 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
                 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
         },
         {
                 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
                 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
                 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
                 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
         },
         {
                 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
                 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
                 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
                 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
         },
         {
                 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
                 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
                 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
                 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
         },
         {
                 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
                 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
                 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
                 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
         }};



#define HEXA_FORMATE 1
#define ASCI_FORMATE 2
#define READ_FORMATE ASCI_FORMATE


int sizeOfArrayOfPlainText = 0;

int sizeOfArrayofLong = 0 ;


/*This function reads file in  Text Formate */
char *readFile(char *filename)
{
    FILE *textfile;
    char ch;
    long numbytes;
    char *text;
    char *temp;
    char *temp2;
    char paddedzeros = 0;

    textfile = fopen(filename, "r");

    if (textfile == NULL)
        return NULL;

    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);
    text = (char *)calloc(numbytes, sizeof(char));

    if (text == NULL)
        return NULL;

    temp = text;

    while ((ch = fgetc(textfile)) != EOF)
    {
        if (ch != '\n' && ch != ' ')
        {
            *temp = ch;
            temp++;
        }
        else
        {
            numbytes--;
        }
    }

    fclose(textfile);

    /* Padding */
    if ((numbytes) % 16 != 0 && (numbytes) != -1)
    {
        for (int i = 0; i < (16 - (numbytes) % 16); i++)
        {
            *temp = '0';
            temp++;
            paddedzeros++;
        }
    }
    /* adding the null character */
    text[numbytes + paddedzeros] = '\0';

    sizeOfArrayOfPlainText = (numbytes + paddedzeros) / 16;
    arrayOfLongsSize=sizeOfArrayOfPlainText;
    return text;
}

/* This Function Convert Normal ASCI to hex ASCI */
unsigned char *readFileASCI(unsigned char *filename)
{
    FILE *filePtr;
    unsigned char *ASCIfile;
    unsigned char temp;
    int fileSize;
    int actualSize = 0;
    filePtr = fopen(reinterpret_cast<const char *>(filename), "rb");

    /* return null if the file is not existed */
    if (filePtr == NULL)
    {
        printf("Unable to open file : %s ", filename);
        return NULL;
    }

    /*calculate size of file */
    fseek(filePtr, 0L, SEEK_END);
    fileSize = ftell(filePtr);
    fseek(filePtr, 0L, SEEK_SET);

    ASCIfile =(unsigned char*)calloc(fileSize + 8, sizeof(char)); // + 8 beacause 1 for null character and  7 for maximum padding

    for (int i = 0; i < fileSize; i++)
    {
        //  if(i==fileSize-1){
        //      break;
        // }
        ASCIfile[actualSize] = fgetc(filePtr);
            actualSize++;
    }

    if (actualSize % 8 == 0) // no need for padding
    {
        ASCIfile[actualSize] = '\0';
    }
    else
    {
        char paddingSize = 8 - (actualSize % 8);

        for (char i = 0; i < paddingSize; i++)
        {
            ASCIfile[actualSize] =  (unsigned long)0;
            actualSize++;

        }
        ASCIfile[actualSize] = '\0';


    }

    /* Close the file */
    fclose(filePtr);
    sizeOfArrayofLong = actualSize / 8;

    return ASCIfile;
}


/*This function convert hex ASCI To long*/
unsigned long *hexToLongArr( char *FileStr)
{
    int charIndex = 0;
    unsigned long *arrayOfPlainText = (unsigned long *)calloc(sizeOfArrayOfPlainText, sizeof(long));

    for (int i = 0; i < sizeOfArrayOfPlainText; i++)
    {
        for (int j = 0; j < 16; j++)
        {

            if (FileStr[charIndex] >= '0' && FileStr[charIndex] <= '9')
            {
                arrayOfPlainText[i] = (arrayOfPlainText[i] << 4) | ((FileStr[charIndex] - '0') & 0x0F);
            }
            else
            {
                arrayOfPlainText[i] = (arrayOfPlainText[i] << 4) | ((FileStr[charIndex] - 'a' + 10) & 0x0F);
            }
            charIndex++;
        }
    }

    return arrayOfPlainText;
}

/* This function convert Normal ASCI To long */
unsigned long *AsciToLong(unsigned char *FileStr)
{

    int charIndex = 0;
    unsigned long *arrayOfPlainText = (unsigned long *)calloc(sizeOfArrayofLong, sizeof(long));
    for (int i = 0; i < sizeOfArrayofLong; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            
            
                arrayOfPlainText[i] = (arrayOfPlainText[i] << 8) | (unsigned long)((FileStr[charIndex]) & (0x00000000000000FF));
                charIndex++;
            
           

        }
    }
    return arrayOfPlainText;
}

/*********************************DEAL WITH THIS FUNCTION DIRECTLY*******************************/
// function that calls the readFile function and hexToLongArr function
unsigned long *readFileLong(unsigned char *filename)
{
    return (AsciToLong(readFileASCI(filename)));
}

unsigned long* readFileLongForKey(char *filename)
{
    return (hexToLongArr(readFile(filename)));
}
// function that converts array of long to string
char *longArrToHex(unsigned long *arr, int arr_size)
{

    char *string = (char *)calloc(arr_size * 16, sizeof(char));
    char hexadecimalnum[16];
    int stringIndex = 0;

    for (int i = 0; i < arr_size; i++)
    {

        int j = 15;
        unsigned long num = arr[i];
        unsigned long remainder = 0;

        while (num != 0)
        {
            remainder = num % 16;
            if (remainder < 10)
            {
                hexadecimalnum[j--] = 48 + remainder;
            }
            else
            {
                hexadecimalnum[j--] = 55 + remainder;
            }
            num = num / 16;
        }

        while (j != -1)
        {
            hexadecimalnum[j--] = '0';
        }

        for (int z = 0; z < 16; z++)
        {
            string[stringIndex++] = hexadecimalnum[z];
        }
    }
    return string;
}

/* write file function */
void writeFile(char *data, char *filename)
{
    FILE *fptr;
    fptr = fopen(filename, "w");
    for (int i = 0; data[i] != '\0'; i++)
    {
        fputc(data[i], fptr);
    }
    fclose(fptr);
}

char *longArrToAsci(unsigned long *arr, int arr_size)
{
    char *string = (char *)calloc((arr_size * 8) + 1, sizeof(char));
    char *ptr;
    int index = 0;
    for (int i = 0; i < arr_size; i++)
    {
        ptr = (char *)&(arr[i + 1]) - 1;
        for (int j = 0; j < 8; j++)
        {

            string[index] = *ptr;
            index++;
            ptr--;
        }
    }
    string[index] = '\0';

     FILE *fptr;
    fptr = fopen("EncOutPlain.txt", "w");
    for (int i = 0; i < arr_size*8 + 1 ; i++)
    {
        fputc(string[i], fptr);
    }
    fclose(fptr);
    return string;
}

void writeFileLongHEX(unsigned long *arr, int arr_size, char *filename)
{
    writeFile(longArrToHex(arr, arr_size), filename);
}
void writeFileLongASCI(unsigned long *arr, int arr_size, char *filename)
{
    (longArrToAsci(arr, arr_size), filename);
}
int main(int argc, char** argv) {
    
char type[8];
*type=*argv[1];

/*////////////////////////////////
           Arrays
///////////////////////////////*/
    unsigned int permuationChoice1[56] = {
            57,49,41,33,25,17,9,
            1,58,50,42,34,26,18,
            10,2,59,51,43,35,27,
            19,11,3,60,52,44,36,
            63,55,47,39,31,23,15,
            7,62,54,46,38,30,22,
            14,6,61,53,45,37,29,
            21,13,5,28,20,12,4
    };
    unsigned int permuationChoice2[48] = {
            14,17,11,24,1,5,
            3,28,15,6,21,10,
            23,19,12,4,26,8,
            16,7,27,20,13,2,
            41,52,31,37,47,55,
            30,40,51,45,33,48,
            44,49,39,56,34,53,
            46,42,50,36,29,32
    };
    unsigned int initialPermutationTable[64] = {
            58,50,42,34,26,18,10,2,
            60,52,44,36,28,20,12,4,
            62,54,46,38,30,22,14,6,
            64,56,48,40,32,24,16,8,
            57,49,41,33,25,17,9,1,
            59,51,43,35,27,19,11,3,
            61,53,45,37,29,21,13,5,
            63,55,47,39,31,23,15,7
    };
    // The expansion table
    unsigned int expansionTable[48] = {
            32,1,2,3,4,5,4,5,
            6,7,8,9,8,9,10,11,
            12,13,12,13,14,15,16,17,
            16,17,18,19,20,21,20,21,
            22,23,24,25,24,25,26,27,
            28,29,28,29,30,31,32,1
    };

    // The permutation table
     unsigned int permutationTable[32] = {
            16,7,20,21,29,12,28,17,
            1,15,23,26,5,18,31,10,
            2,8,24,14,32,27,3,9,
            19,13,30,6,22,11,4,25
    };
    // The inverse permutation table
    unsigned int inversePermutationTable[64]= {
            40,8,48,16,56,24,64,32,
            39,7,47,15,55,23,63,31,
            38,6,46,14,54,22,62,30,
            37,5,45,13,53,21,61,29,
            36,4,44,12,52,20,60,28,
            35,3,43,11,51,19,59,27,
            34,2,42,10,50,18,58,26,
            33,1,41,9,49,17,57,25
    };
    char keyFile[] = {'k','e','y','.','t','x','t','\0'};
    inputKey= *readFileLongForKey(keyFile);

if((*type)=='d' || (*type) == 'D') {
    /*******************************************************************************/
    /// Decryption ///
    /*******************************************************************************/
    char hexaInputFile[]={'D','e','c','I','n','H','e','x','.','t','x','t','\0'};
    unsigned long  *arrayOfLongs = readFileLongForKey(hexaInputFile);

    for (unsigned int counter = 0; counter < arrayOfLongsSize; counter++) {
        unsigned long leftperm = 0;
        unsigned long rightperm = 0;
        unsigned long firstPermutedKey = 0;
        plainText = inputKey;
        firstPermutedKey = Permutation(64, permuationChoice1, 56, &leftperm, &rightperm);
        unsigned long keys[16];
        generateKeys(keys, firstPermutedKey);
        unsigned long permutedKeys[16] = {0};
        for (int i = 0; i < 16; i++) {
            plainText = keys[i];
            permutedKeys[i] = Permutation(56, permuationChoice2, 48, &leftperm, &rightperm);
        }
        plainText = arrayOfLongs[counter];
        unsigned long nonPermutedData = 0;
        Permutation(64, initialPermutationTable, 64, &leftperm, &rightperm);
        unsigned long dummyLeftPerm = 0;
        unsigned long dummyrightPerm = 0;
        unsigned long tempRight = 0;
        for (int i = 15; i >= 0; i--) {
            tempRight = rightperm;
            plainText = rightperm;
            rightperm = Permutation(32, expansionTable, 48, &dummyLeftPerm, &dummyrightPerm);
            rightperm = (rightperm ^ permutedKeys[i]);
            rightperm = S_box(rightperm);
            plainText = rightperm;
            rightperm = Permutation(32, permutationTable, 32, &dummyLeftPerm, &dummyrightPerm);
            rightperm = (rightperm ^ leftperm);
            leftperm = tempRight;
        }
        unsigned long cipherText = 0;
        cipherText = ((rightperm << 32) | (leftperm));
        plainText = cipherText;
        cipherText = Permutation(64, inversePermutationTable, 64, &dummyLeftPerm, &dummyrightPerm);
        arrayOfLongs[counter]=cipherText;

    }

    char outputdecryptedFile[] = {'D','e','c','O','u','t','P','l','a','i','n','.','t','x','t','\0'};
    
    writeFile(longArrToAsci(arrayOfLongs, arrayOfLongsSize), outputdecryptedFile);
   // writeFileLongASCI(arrayOfLongs,arrayOfLongsSize,outputdecryptedFile);
    char outputHexaFile[] = {'D','e','c','O','u','t','H','e','x','.','t','x','t','\0'};
    writeFileLongHEX(arrayOfLongs,arrayOfLongsSize,outputHexaFile);
/********************************************************************************/
}else {
/********************************************************************************/
    /// encryption  ///
/********************************************************************************/
    unsigned   char inputFile[] = {'E','n','c','I','n','P','l','a','i','n','.','t','x','t','\0'};
    unsigned long  *arrayOfLongs = readFileLong(inputFile);
    arrayOfLongsSize=sizeOfArrayofLong;

for(unsigned int counter=0;counter<arrayOfLongsSize;counter++) {

    unsigned long leftperm=0;
    unsigned long rightperm=0;
    unsigned long firstPermutedKey=0;
    plainText=inputKey;
    firstPermutedKey = Permutation(64, permuationChoice1, 56, &leftperm, &rightperm);
    unsigned long keys[16];
    generateKeys(keys, firstPermutedKey);
    unsigned long permutedKeys[16]={0};
    for (int i = 0; i < 16; i++) {
        plainText = keys[i];
        permutedKeys[i] = Permutation(56, permuationChoice2, 48, &leftperm, &rightperm);

    }
    plainText=arrayOfLongs[counter];
    unsigned long initialPermutedData=0;
    initialPermutedData=Permutation(64,initialPermutationTable, 64, &leftperm, &rightperm);
    unsigned long dummyLeftPerm=0;
    unsigned long dummyrightPerm=0;
    unsigned long tempRight=0;
    for(int i=0;i<16;i++){
        tempRight=rightperm;
        plainText=rightperm;
        rightperm=Permutation(32,expansionTable, 48, &dummyLeftPerm, &dummyrightPerm);
        rightperm=(rightperm ^permutedKeys[i]);
        rightperm=S_box(rightperm);
        plainText= rightperm;
        rightperm=Permutation(32,permutationTable, 32, &dummyLeftPerm, &dummyrightPerm);
        rightperm=(rightperm^leftperm);
        leftperm=tempRight;
    }
unsigned long cipherText=0;
    cipherText= ((rightperm << 32) | (leftperm));
    plainText=cipherText;
   cipherText=Permutation(64, inversePermutationTable, 64, &dummyLeftPerm, &dummyrightPerm);
   arrayOfLongs[counter]=cipherText;
}

    char outputHexaFile[] = {'E','n','c','O','u','t','H','e','x','.','t','x','t','\0'};
    writeFileLongHEX(arrayOfLongs,arrayOfLongsSize,outputHexaFile);
    char outputEncryptedFile[] = {'E','n','c','O','u','t','P','l','a','i','n','.','t','x','t','\0'};
    writeFileLongASCI(arrayOfLongs,arrayOfLongsSize,outputEncryptedFile);
/***********************************************************************/
}

return 0;

}

void generateKeys(unsigned long* keys,unsigned long key){ //key is giver after pc1
    unsigned long accKey=key;
    unsigned long leftKeys=0;
    unsigned long rightKeys=0;

    for(int i=0;i<16;i++) {
        splitKey(accKey,&leftKeys,&rightKeys);
        leftKeys = leftCircularShift(leftKeys,roundShifts[i]);
        rightKeys = leftCircularShift(rightKeys,roundShifts[i]);
        keys[i] =(leftKeys << 28) |( rightKeys);
        accKey=keys[i];
    }

}
void splitKey(unsigned long key,unsigned long* leftKey,unsigned long* rightKey){
   unsigned long leftMask=0x00FFFFFFF0000000;
   unsigned long rightMask=0x000000000FFFFFFF;
    *rightKey =(key & rightMask);
    *leftKey= ((  key & leftMask) >> 28);
}


/**********************************************************************/
///
/// function name:  permutation
///
///  input:  sizeOfData - number of actual bits for the sent key(eg.for permutation choice 2 the sizeOfData = 56)
///  input:  permutationArray - the table of permutation or expansion
///  input:  size - it's the size of the array (eg. size for permutation choice 2 is 48
///  input:  leftPermutedData - unsigned integer passed by reference will be updated with left part of the permuted data
///  input:  rightPermutedData - unsigned integer passed by reference will be updated with right part of the permuted data
///
///  output:  unsigned long carry the value of the data after permutation
///
///  description:  the function takes the table of the permutation and the data to be permuted must be declared
///  and initialized globally and the returns the data permuted
///
/***********************************************************************/
unsigned long Permutation(int sizeOfData,unsigned int *permutationArray,unsigned int size,unsigned long* leftPermutedData,unsigned long* rightPermutedData){
    unsigned long permutedData=0;
    unsigned long value=0;
    unsigned long one=1;

    for(int i=0;i<size;i++){
        value= (plainText & (one << (sizeOfData-permutationArray[i])));
        value=(value >> (sizeOfData-permutationArray[i]));
        permutedData |= (value << (size -i-1)) ;
    }
    *leftPermutedData = permutedData >> size/2;
    *rightPermutedData = ((permutedData <<(64-(size/2))) >>(64-(size/2)));
    return permutedData;
}
/**************************************************************/
///              test for permutation choice 2
/// plianText: 0x00123456789ABCDE
/// Permutation(56,permuationChoice2,48,&leftperm,&rightperm)
/// Actual/right result: 1010 0001 0000 1011 1001 1100 1101 0100 0110 1110 0110 1110
/*************************************************************/
unsigned long leftCircularShift(unsigned long number,int shifts){
number = ((number << shifts) | (number >> (28-shifts)) )&0x000000000FFFFFFF;
return number;
}
unsigned long S_box(unsigned long number){
    unsigned long row;
    unsigned long column;
    unsigned long editedNumber;
    unsigned long outputNumber=0;
    unsigned long mask=0x00000000000003F;
    for(int i=0;i<8;i++){
        editedNumber =number & (mask<<(42-i*6));
        editedNumber=editedNumber >>(42-i*6);
        row = (editedNumber  & 1) | ((editedNumber & 1<<5)>>4);
        column = (editedNumber & 0b00011110)>>1;
        outputNumber |=S_boxValues[i][row][column] <<(28-i*4);
    }

return outputNumber;
}
