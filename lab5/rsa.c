//RSA Algorithm
//-----------------------------------------------------------------------------

#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

long int gcd(long int a,long int h)
{
    if(h == 0) return a;
    return gcd(h,a%h);
}

long int inverse(long int a, long int b)
{
    long int inv;
    long int q, r, r1 = a, r2 = b, t;
    long int t1 = 0,t2 = 1;

    while (r2 > 0) {
        q = r1 / r2;
        r = r1 - q * r2;
        r1 = r2;
        r2 = r;

        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
    }

    if (r1 == 1)
        inv = t1;

    if (inv < 0)
        inv = inv + a;

    return inv;
}


double encrypt(double msg,long int e, long int n)
{
    return fmod(pow(msg,e),n);
}



long int main()
{
    srand(time(NULL));

    //Two prime numbers
    long int p = 5;
    long int q = 7;
    
    //Generating other variables from p and q
    long int n = p*q;
    long int phi = (p-1)*(q-1);

    long int d,e;

    //Generate e
    do
        e = rand()%(phi-2)+2;   //random e such that 1 < e < phi.
    while(gcd(e,phi) != 1);     //find another e if e,phi are not coprime
    printf("Generated Public Encryption Key (e,n)  (%ld,%ld)\n",e,n);
    
    //Generate d 
    d = inverse(phi,e);
    printf("Generated Private Decryption Key (d,n)  (%ld,%ld)\n",d,n);

    //printf("Test %d\n", (d*e)%phi);
    //
    
    double message = 3;
    printf("Message is %lf\n",message);

    double encrpt_msg = encrypt(message,e,n);
    printf("Encrypted Message is %lf\n",encrpt_msg);

    double decrpt_msg = encrypt(encrpt_msg,d,n);
    printf("Decrypted Message is %lf\n",decrpt_msg);

    
    return 0;

}
