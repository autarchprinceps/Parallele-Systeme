      IMPLICIT  DOUBLE PRECISION (A-H,O-Z)

      integer ndim1, ndim2, ndim3
C parameter set 1
C     parameter(ndim1=1024*32, ndim2=128, ndim3=64)
C parameter set 2
C      parameter(ndim1=1024*256, ndim2=512, ndim3=256)
C parameter set 3
       parameter(ndim1=4*1000*1000, ndim2=1000, ndim3=1000)

      COMMON /SPACE1/ U(ndim1+1), V(ndim1+1), W(ndim1+1),
     1  X(ndim1+1), Y(ndim1+1), Z(ndim1+1), G(ndim1+1),
     2  DU1(ndim1+1), DU2(ndim1+1), DU3(ndim1+1), GRD(ndim1+1),
     2  DEX(ndim1+1),
     3  XI(ndim1+1), EX(ndim1+1), EX1(ndim1+1), DEX1(ndim1+1),
     4  VX(ndim1+1), XX(ndim1+1), RX(ndim1+1), RH(ndim1*2),
     5  VSP(ndim1+1), VSTP(ndim1+1), VXNE(ndim1+1), VXND(ndim1+1),
     6  VE3(ndim1+1), VLR(ndim1+1), VLIN(ndim1+1), B5(ndim1+1),
     7  PLAN(ndim1), D(ndim1), SA(ndim1+1), SB(ndim1+1)

      COMMON /SPACE2/ P(4,ndim1), PX(25,ndim1+1), CX(25,ndim1+1),
     1  VY(ndim1+1,25), VH(ndim1+1,7), VF(ndim1+1,7), VG(ndim1+1,7),
     1  VS(ndim1+1,7),
     2  ZA(ndim1+1,7)  , ZP(ndim1+1,7), ZQ(ndim1+1,7), ZR(ndim1+1,7),
     2  ZM(ndim1+1,7),
     3  ZB(ndim1+1,7)  , ZU(ndim1+1,7), ZV(ndim1+1,7), ZZ(ndim1+1,7),
     4  B(ndim2,ndim2), C(ndim3,ndim3), H(ndim3,ndim3),
     5  U1(5,ndim1+1,2),  U2(5,ndim1+1,2),  U3(5,ndim1+1,2)
c
c     ******************************************************************
c
      DIMENSION     ZX(NDIM1+24), XZ(NDIM1+500), TK(6)

      COMMON /ALPHA/ mk,ik,im,ml,il,Mruns,Nruns,jr,iovec,NPFS(8,3,47)
      COMMON /BETA / tic, TIMES(8,3,47), SEE(5,3,8,3),
     1              TERRS(8,3,47), CSUMS(8,3,47),
     2              FOPN(8,3,47), DOS(8,3,47)
      COMMON /SPACES/ ion,j5,k2,k3,MULTI,laps,Loop,m,kr,LP,n13h,ibuf,nx,
     1 L,npass,nfail,n,n1,n2,n13,n213,n813,n14,n16,n416,n21,nt1,nt2,
     2 last,idebug,mpy,Loops2,mucho,mpylim, intbuf(16)
      COMMON /SPACER/ A11,A12,A13,A21,A22,A23,A31,A32,A33,
     1                AR,BR,C0,CR,DI,DK,
     2  DM22,DM23,DM24,DM25,DM26,DM27,DM28,DN,E3,E6,EXPMAX,FLX,
     3  Q,QA,R,RI,S,SCALE,SIG,STB5,T,XNC,XNEI,XNM
      COMMON /SPACE0/ TIME(47), CSUM(47), WW(47), WT(47), ticks,
     1                FR(9), TERR1(47), SUMW(7), START,
     2              SKALE(47), BIAS(47), WS(95), TOTAL(47), FLOPN(47),
     3                IQ(7), NPF, NPFS1(47)
      COMMON /SPACEI/ WTP(3), MUL(3), ISPAN(47,3), IPASS(47,3)
      INTEGER    E,F,ZONE
      COMMON /ISPACE/ E(96), F(96),
     1  IX(1001), IR(1001), ZONE(300)

      INTEGER TEST, AND
      COMMON /ORDER/ inseq, match, NSTACK(20), isave, iret
      COMMON /TAU/   tclock, tsecov, testov, cumtim(4)
