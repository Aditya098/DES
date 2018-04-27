#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "des_var.h"

int afterShift[28];
int l[17][32],r[17][32],xor_key[16][48];
int expandedBlock[48],output_v[32],output_val[4],count = 0;

void CircularLeftShift(int HKey[28],int round_no){
  int i,FirstBit;
  int same[28];
  for(int i=0;i<28;i++){
          same[i] = HKey[i];
  }
	while(round_no >= 1){
                FirstBit = same[0];
                for (i = 0; i < 27; i++){
                        same[i] = same[i + 1];
                }
                same[27] = FirstBit;
                round_no--;
	}
	for(i=0;i<28;i++){
		afterShift[i] = same[i];
	}
}
/* Expanded Permutation */
void ExpansionPerm(int data[32]) {
  int i;

  for (i = 0; i < 48; i++)
    expandedBlock[i] = data[DesExpansion[i]-1];
}

void decToBinary(int n){
    // array to store binary number
    int binaryNum[100];

    // counter for binary array
    int i = 0,k = 0;
    if(n==0){
            i=1;
            binaryNum[0]=0;
    }
    while (n > 0) {
        // soring remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    if (i==3){
            output_val[0] = 0;
            k=1;
            }
    else if (i==2){
            output_val[0] = 0;
            output_val[1] = 0;
            k=2;
            }
   else if (i==1){
            output_val[0] = 0;
            output_val[1] = 0;
            output_val[2] = 0;
            k=3;
            }
    for (int j = i - 1; j >= 0; j--){
        output_val[k] = binaryNum[j];
        k++;
       }
       k=0;
       count++;
}

void ComputeS_Lookup(int s_inp[48]) {
	int input[8][6],i;
	int row,col,m;
	for(i=0;i<48;i++){
	        m = i/6;
	        input[m][i-(6*m)] = s_inp[i];
	}
	i=0;
        //printf("\n");
	while(i<8){
	        row = input[i][0]*2 + input[i][5]*1;
	        col = input[i][1]*8 + input[i][2]*4 +input[i][3]*2 + input[i][4]*1;
	        int ov = DesSbox[i][row][col];
	        decToBinary(ov);
	        int c;
	        for(c=0;c<4;c++)
	                output_v[(4*i)+c] = output_val[c];   // output_v is 32 bit generated after S-Box test it s
	        //i++;
                // printf("%d %d %d ",row,col,ov);
                // for(int c=0;c<4;c++){
                //         printf("%d", output_val[c]);
                // }
                // printf("\t");
                i++;

	}
        //printf("\n");
}


void F(int lc[32],int rc[32]){
        int after_48EP[48],after_48xor[48],after_32SP[32],after_32SPX[32];
        int k1=0,i;

        for(k1 = 0 ; k1 <16 ; k1++){
                // printf("\n rc \n");
                // for(int i=0;i<32;i++){
                //         printf("%d",rc[i]);
                // }
                ExpansionPerm(rc);
                // printf("\nafter Expanded permutation \n");
                // for (i = 0; i < 48; i++){
                //         printf("%d",expandedBlock[i]);
                // }
                //printf("\nafter 48xor with key \n");
                for (i = 0; i < 48; i++){
                        after_48EP[i] = expandedBlock[i];
                        after_48xor[i] = xor_key[k1][i] ^ after_48EP[i];    //k1=0
                        //printf("%d",after_48xor[i]);
                }
                //printf("\n\n");
                ComputeS_Lookup(after_48xor);    // S box
                //printf("\nafter S box\n");
                // for(i=0;i<32;i++){
                //         printf("%d",output_v[i]);
                // }
                //printf("\nafter permutation \n");
                for(i=0;i<32;i++){
                        after_32SP[i]= output_v[message_permutation[i]-1];  // permutation after xor
                        //printf("%d",after_32SP[i]);
                }
                //printf("\nafter xor with left key\n");
                for(i=0;i<32;i++){
                        after_32SPX[i] = lc[i] ^ after_32SP[i];   // xor after Sbox, perm
                        //printf("%d",after_32SPX[i]);
                }
                for(i=0;i<32;i++){
                        r[k1+1][i] =  after_32SPX[i];
                        rc[i] =  after_32SPX[i];         // next set of r and l values
                        l[k1+1][i] = r[k1][i];
                        lc[i] = r[k1][i];
                        }
                int k2 = k1+1;
                printf("\nL %d\n",k2);
                for(int i=0;i<32;i++){
                        printf("%d",lc[i]);
                }
                printf("\nR %d\n",k2);
                for(int i=0;i<32;i++){
                        printf("%d",rc[i]);
                }
                printf("\n------------------------------------------\n");
        }
        }

int main(){
        int plaintext[64] = {0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1
          ,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1};
        char k[64];
        int key[64],perm_56key[56];
        int i;
        FILE *in_file  = fopen("key.txt", "r");
        if (in_file == NULL){
                  printf("Error! Could not open file\n");
                  exit(-1); // must include stdlib.h
          }

        for(i=0;i<64;i++){
                fscanf(in_file,"%c",&k[i]);
          }

        for(i=0;i<64 ;i++){
                key[i] = k[i] - '0';
         }
         printf("key :\n");
        for(i=0;i<64 ;i++){
                printf("%d",key[i]);
        }
        printf("\nPlain text :\n");

        for(i=0;i<64;i++){
                printf("%d",plaintext[i]);
        }
        printf("\n");
        for(i=0;i<56;i++){
                 perm_56key[i] = key[PC1[i]-1];
                //printf("%d",perm_56key[i]);
         }
        // printf("\n");

         // key generation k1 k2 .....k16
         int lk[17][28],rk[17][28];
         for(i=0;i<56;i++){
                 if(i<28)
                         lk[0][i] = perm_56key[i];
                 else
                         rk[0][i-28] = perm_56key[i];
           }
         int lkc[28],rkc[28],perm_48key[48],perm_561key[56];
         for(i=0;i<28;i++){
                 lkc[i] = lk[0][i];
                 rkc[i] = rk[0][i];
           }
            // 16 rounds...
         int j;
         for(j=0;j<16;j++){

                 CircularLeftShift(lkc,DesRotations[j]);
                 for(i=0;i<28;i++){
                         lkc[i] = afterShift[i];
                         lk[j+1][i] = lkc[i];
                 }
                 CircularLeftShift(rkc,DesRotations[j]);
                 for(i=0;i<28;i++){
                         rkc[i] = afterShift[i];
                         rk[j+1][i] = rkc[i];
                 }

                 for(i=0;i<56;i++){
                         if(i < 28)
                                 perm_561key[i] = lkc[i];
                         else
                                 perm_561key[i] = rkc[i-28];
                 }

                 for(i=0;i<48;i++){
                         perm_48key[i] = perm_561key[PC2[i]-1];
                 }
                 for(i=0;i<48;i++){
                         xor_key[j][i] = perm_48key[i];
                        // printf("%d",xor_key[j][i]);
                 }

         }
         // printf("\n");


          // key generation done

          // encrypting part

          int ip[64];
         // printf("\n after initial permutation\n");
          for(i=0;i<64;i++){
                  ip[i] = plaintext[DesInitial[i]-1];
                //  printf("%d",ip[i]);
          }
         // printf("\n\n");

          for(i=0;i<64;i++){
                if(i<32)
                        l[0][i] = ip[i];
                else
                        r[0][i-32] = ip[i];
          }
          int rc[32],lc[32];
          i=0;
          for(j=0;j<32;j++){
                  lc[j] = l[i][j];
                  rc[j] = r[i][j];
          }
        F(lc,rc);

        // printf("\nL16\n");
        // for(i=0;i<32;i++)
        //         printf("%d",l[16][i]);
        // printf("\nR16\n");
        // for(i=0;i<32;i++)
        //         printf("%d",r[16][i]);

        int R16L16[64];
        for(int i=0;i<64;i++){
                if(i<32)
                        R16L16[i] = r[16][i];
                else
                        R16L16[i] = l[16][i-32];
        }
        int cipher[64];
        
        // /* Final Permutation to generate Ciphertext */
        for(i=0;i<64;i++){
                cipher[i] = R16L16[FinalPermutation[i]-1];
          }
        printf("Cipher Text after final permutation:\n");
        for(i=0;i<64;i++){
                printf("%d",cipher[i]);
        }
        printf("\n\n");
return 0;
}
