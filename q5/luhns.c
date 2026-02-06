#include <stdio.h>
int main(){
    int n;
    int rc = scanf("%d", &n);
    printf("%d", n);

    int sum = 0;
    int a = 1;
    const int ACC = n;
    while(n != 0) {
        digit = n%10;
        if(a == 1){
            if(digit*2 >= 10){
                sum+=((digit*2) - 9);
            }
            else{
                sum+=(digit*2);
            }
            a*=-1;
        }
        else{
            sum+=digit;
            a*=-1
        }

        num = num/10;
    }

    if (sum%10 == ACC){
        printf("Valid");
    }
    else{
        printf("Invalid");
    }
}


int main(){
    int len = 0;
    int oddLenSum = 0;
    int evenLenSum = 0;
    int finalSum = 0;
    int doubled = 0;

    int prev = -1;

    int c;
    int digit;

    while ((c = getchar()) >= '0' && c <= '9'){
        digit = c - '0';

        if (prev >= 0){
            doubled = prev*2;
            if (doubled >=10){
                doubled-=9;
            }
        

            if (len%2 == 1){
                oddLenSum += doubled;
                evenLenSum += prev;
            }
            else{
                oddLenSum += prev;
                evenLenSum += doubled;
            }
        }

        prev = digit;
        len++;
    }
    if (len%2 == 0){
        finalSum = evenLenSum;
    }
    else{
        finalSum = oddLenSum;
    }

    if ((finalSum*9)%10 == prev){
        printf("Valid\n");
    }
    else{
        printf("Invalid\n");
    }

}