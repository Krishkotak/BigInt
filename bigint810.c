#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIGITS 310 
#define MAX_DIGITS 620

typedef struct {
    char digits[DIGITS];
    int length;
    int pos;
} BigInt;

typedef struct{
    char digits[MAX_DIGITS];
    int length;
    int pos;
} BigBigInt;

void initializeBigInt(BigInt *biggy,  char *str) {
    
    int len=strlen(str);
    biggy->length = len;
    if(str[0]=='-')
    {
            biggy->pos=0;
            
            biggy->length--;

    }
    else{
        biggy->pos=1;
    }
    
    for (int i = 0; i < biggy->length; ++i) {
       // if(!biggy->pos)

        biggy->digits[i] = str[len - 1 - i] - '0';
        //printf("%d\n",biggy->digits[i]);
    }
}

void printBigInt(  BigInt *biggy,int pos) {
    if(!pos)
        printf("-");
    for (int i = biggy->length - 1; i >= 0; --i) {
        printf("%d", biggy->digits[i]);
    }
    printf("\n");
}

void printBigBigInt(  BigBigInt *bigbiggy,int pos) {
    //printf("pos of bigint is : %d\n ",pos);
    if(!pos){
        printf("-");
    }
    for (int i = bigbiggy->length - 1; i >= 0; --i) {
        printf("%d", bigbiggy->digits[i]);
    }
    printf("\n");
}

void addBigInt(  BigInt *a,   BigInt *b, BigInt *result) {
    int carry = 0;
    int length = a->length > b->length ? a->length : b->length;
    for (int i = 0; i < length; ++i) {
            int a_digit = i < a->length ? a->digits[i] : 0;
            int b_digit = i < b->length ? b->digits[i] : 0;
            int sum = a_digit + b_digit + carry;
            result->digits[i] = sum % 10;
            carry = sum / 10;
        }
        if (carry) {
            result->digits[length++] = carry;
        }
        result->length = length;
   


}

int subtractBigInt(  BigInt *a,   BigInt *b, BigInt *result) {
    int borrow = 0;
    int max=0;
    if(a->length>b->length)
        max=a->length;
    else
        max=b->length;
    for (int i = 0; i < max; ++i) {
        int a_digit = i<a->length? a->digits[i]:0;
        int b_digit = i < b->length ? b->digits[i] : 0;
        int diff = a_digit - b_digit - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = diff;
    }
    if (borrow==0)
    {
        result->length = a->length;
    
        while (result->length > 1 && result->digits[result->length - 1] == 0) {
            result->length--;
        
        return 1;//positive
    }
    }
    else
    {    subtractBigInt(b,a,result);
        return 0;//negative
    }
    
}

void multiplyBigInt(  BigInt *a,   BigInt *b, BigBigInt *result) {
    BigBigInt temp = {0};
    for (int i = 0; i < a->length; ++i) {
        for (int j = 0; j < b->length; ++j) {
            temp.digits[i + j] += a->digits[i] * b->digits[j];
            temp.digits[i + j + 1] += temp.digits[i + j] / 10;
            temp.digits[i + j] %= 10;
        }
    }
    temp.length = a->length + b->length;
    while (temp.length > 1 && temp.digits[temp.length - 1] == 0) {
        temp.length--;
    }
    *result = temp;
}

int main() {
    char input1[DIGITS];
    char input2[DIGITS];
    char operation;

    printf("Enter the first 1024-bit number: ");
    scanf("%s", input1);

    printf("Enter the operation (+, -, *): ");
    scanf(" %c", &operation);

    printf("Enter the second 1024-bit number: ");
    scanf("%s", input2);

    BigInt biggy1, biggy2;
    initializeBigInt(&biggy1, input1);//put string input1 into the structure biggy
    initializeBigInt(&biggy2, input2);
   
    int pos=1;

    if(operation == '+')
    {
        BigInt result;
        
        if(biggy1.pos && biggy2.pos)
        {
            addBigInt(&biggy1, &biggy2, &result);
            result.pos=1;
        }
        else if (!biggy1.pos && !biggy2.pos)
        {
            addBigInt(&biggy1, &biggy2, &result);
            result.pos=0;
        }
        else
        {
            subtractBigInt(&biggy1,&biggy2,&result);
            if(biggy1.pos && !biggy2.pos)
            {
                if (biggy1.length>=biggy2.length)
                    result.pos=1;
                else
                {
                    result.pos=0;
                }

            }
            else{
                 if (biggy1.length>biggy2.length)
                    result.pos=0;
                else
                {
                    result.pos=1;
                }
            }
        }

        
        printf("Result: ");
        printBigInt(&result,result.pos);
    }
    else if (operation == '-')
    {
        BigInt result;
        if(biggy1.pos && biggy2.pos)
        {
            result.pos=subtractBigInt(&biggy1, &biggy2, &result);
            //result.pos=1;
        }
        else if (!biggy1.pos && !biggy2.pos)
        {
            pos=subtractBigInt(&biggy1, &biggy2, &result);
            if(pos)
                result.pos=0;
            else    
                result.pos=1;
        }
        else
        {
            addBigInt(&biggy1,&biggy2,&result);
            if(biggy1.pos && !biggy2.pos)
            {
                result.pos=1;

            }
            else{
                result.pos=0;
                
            }

        }

       // pos=subtractBigInt(&biggy1, &biggy2, &result);
        printf("Result: ");
        printBigInt(&result,result.pos);
    }
    else if (operation == '*')
    {
        BigBigInt result;
        pos=1;
        if((!biggy1.pos && biggy2.pos) || (biggy1.pos && !biggy2.pos))
        { 
             //printf("biggy1.pos= %d \t biggy2.pos= %d\n ",biggy1.pos,biggy2.pos);

            pos=0;
        }
        multiplyBigInt(&biggy1, &biggy2, &result);
        
        printf("Result: ");
        printBigBigInt(&result,pos);
    }
    else
    {
        printf("Invalid operation!\n");
    }

    
    return 0;
}