#include "k.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>

ZI L='q';ZS l[]={
  "{[x];a:.q.show;b:neg .z.w;.q.show::{x .Q.s y;}[b];@[{.q.show value x;};x;{x\"'\",y,\"\\n\"}[b]];.q.show::a;}",
  "k){a:.q.show;b:-.z.w;.q.show::{x .Q.s y;}[b];@[{x (-3!y),\"\\n\"}[b];x;{x\"'\",y,\"\\n\"}[b]];.q.show::a;}"
};
ZI i0(V){C b[4];if(read(0,b,1)==1)R *b;R -1;}
ZI ox(I f,S x,I n){I r;while(n>0)if((r=write(1,x,n))<=0)break;else x+=r,n-=r;R 1;}ZI o1(S x,I n){ox(1,x,n);}ZI o1c(I c){C b[4]={c,0,0,0};R o1(b,1);}
ZI oxs(I f,S x){R ox(f,x,strlen(x));}ZI o1s(S x){R oxs(1,x);}ZI o1r(V){R o1s("\r\x1b[K");}
ZK nk(V){K x=ktn(KC,2);kC(x)[1]=')';if('k'==(xC[0]=L))o1("  ",2);else o1(xC,2);R x;}ZV rk(K x){o1r();o1(xC,xn);}

int main(int argc,char *argv[]){I f,c,d=0;C b[4096];K x,y;struct pollfd p[2]={0};struct termios G0,G1;
  if(argc == 1){oxs(2,"Usage: ");oxs(2,*argv);oxs(2," [host] port\n");R 1;}
  if(-1==(f=khpu(argc==2?"127.0.0.1":argv[1],atoi(argv[argc-1]),getenv("USER")))){oxs(2,"kdb: ");oxs(2,strerror(errno));oxs(2,"\n");R 2;}
  tcgetattr(0,&G0);G1=G0;G1.c_iflag&=~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);G1.c_oflag&=~OPOST;G1.c_cflag|=CS8;G1.c_lflag&=~(ECHO|ICANON|IEXTEN|ISIG);G1.c_cc[VMIN]=1;G1.c_cc[VTIME]=0;
  tcsetattr(0,TCSAFLUSH,&G1);

  p->fd=0;p[1].fd=f;DO(2,p[i].events=POLLIN|POLLHUP);
  x=nk();while(-1!=poll(p,2,-1))
    if(p[0].revents&POLLIN)switch(c=i0()){
      case 3:case 4:case -1:shutdown(f,SHUT_WR);break;case 12:rk(x),d=0;break;case 8:case 127:if(xn>2)o1s("\b \b"),xn--;break;
      case '\r':case '\n':if(xn>2)k(-f,l[L=='k'],x,0,0);else r0(x); o1s("\r\n");x=nk();break;
      default:ja(&x,&c);/*littleendian*/o1c(c);break;
    } else if(p[1].revents&POLLIN){if(0>=(c=read(f,b,4096)))break;if(!d)o1r();d=1;o1(b,c);if(b[c-1]=='\n')rk(x),d=0;}
  tcsetattr(0,TCSAFLUSH,&G0);
  R 0;
}
