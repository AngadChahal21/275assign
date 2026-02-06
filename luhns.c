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
                sum+=(digit*2)
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