//
// Created by Jack Armstrong on 11/17/19.
//

#include "noise.h"

namespace utils {

    namespace noise {

        IntTwister::IntTwister(uint32_t seed) {
            state[0]=seed;
            for(int i=1;i<statesize;i++){
                state[i]=1812433253UL*(state[i-1]^(state[i-1]>>30))+i;
            }
            twist();
        }

        void IntTwister::twist() {
            const int M=397;
            const int FirstHalf=statesize-M;
            int i;
            for(i=0;i<FirstHalf;i++){
                uint32_t bits=(state[i]&0x80000000)|(state[i+1]&0x7fffffff);
                state[i]=state[i+M]^(bits>>1)^((bits&1)*0x9908b0df);
            }
            for(;i<statesize-1;i++){
                uint32_t bits=(state[i]&0x80000000)|(state[i+1]&0x7fffffff);
                state[i]=state[i-FirstHalf]^(bits>>1)^((bits&1)*0x9908b0df);
            }

            uint32_t bits=(state[i]&0x80000000)|(state[0]&0x7fffffff);
            state[i]=state[M-1]^(bits>>1)^((bits&1)*0x9908b0df);
            next=0;
        }

        int IntTwister::get() {
            if(next>=statesize){
                twist();
            }
            uint32_t x=state[next++];
            x^=x>>11;
            x^=(x<<7)&0x9d2c5680;
            x^=(x<<15)&0xefc60000;
            x^=x>>18;
            int y=(int)x;
            return y<0?-y:y;
        }

        double Perlin::lerp(double a,double b,double x){
            return a+x*(b-a);
        }

        double Perlin::fade(double t){
            return t*t*t*(t*(t*6-15)+10);
        }

        double Perlin::grad(int hash,double x,double y,double z){
            int h=hash&15;
            double u=h<8?x:y;
            double v;
            if(h<4)v=y;
            else if(h==12||h==14)v=x;
            else v=z;
            return ((h&1)==0?u:-u)+((h&2)==0?v:-v);
        }

        double Perlin::perlin(double x,double y,double z){
            int xi=(int)x%255;
            int yi=(int)y%255;
            int zi=(int)z%255;
            double xf=x-(int)x;
            double yf=y-(int)y;
            double zf=z-(int)z;
            double u=fade(xf);
            double v=fade(yf);
            double w=fade(zf);
            int aaa=p[p[p[xi+0]+yi+0]+zi+0];
            int baa=p[p[p[xi+1]+yi+0]+zi+0];
            int aba=p[p[p[xi+0]+yi+1]+zi+0];
            int aab=p[p[p[xi+0]+yi+0]+zi+1];
            int abb=p[p[p[xi+0]+yi+1]+zi+1];
            int bab=p[p[p[xi+1]+yi+0]+zi+1];
            int bba=p[p[p[xi+1]+yi+1]+zi+0];
            int bbb=p[p[p[xi+1]+yi+1]+zi+1];
            double x1,x2,y1,y2;

            x1=lerp(grad(aaa,xf,yf,zf),grad(baa,xf-1,yf,zf),u);
            x2=lerp(grad(aba,xf,yf-1,zf),grad(bba,xf-1,yf-1,zf),u);
            y1=lerp(x1,x2,v);
            x1=lerp(grad(aab,xf,yf,zf-1),grad(bab,xf-1,yf,zf-1),u);
            x2=lerp(grad(abb,xf,yf-1,zf-1),grad(bbb,xf-1,yf-1,zf-1),u);
            y2=lerp(x1,x2,v);

            return (lerp(y1,y2,w)+1)/2;
        }

        Perlin::Perlin(uint32_t seed) {

            IntTwister*twister=new IntTwister(seed);

            initialize_permutation(twister);

            delete twister;

        }

        void Perlin::initialize_permutation(IntTwister *twister) {
            for(int i=0;i<256;i++){
                p[i]=i;
            }

            for(int i=256-1;i>=1;i--){
                int j=twister->get()%(i+1);
                printf("swap %i %i\n",i,j);
                int temp=p[i];p[i]=p[j];p[j]=temp;
            }

            for(int i=0;i<256;i++){
                printf("%i, ",p[i]);
            }
            printf("\n");

            for(int i=256;i<512;i++){
                p[i]=p[i-256];
            }
        }

        Perlin::Perlin(IntTwister *twister) {
            initialize_permutation(twister);
        }

    }

}