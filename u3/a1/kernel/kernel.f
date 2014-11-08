      program livermore
      include 'kernel.inc'

C get data size selection from command line      
      CHARACTER(len=32) :: arg
      if(COMMAND_ARGUMENT_COUNT() .ne. 1) then
          write(*,*) 'usage: prog kernel_size_selection'
          stop
      endif
      call GET_COMMAND_ARGUMENT(1, arg)
      read(arg, '(i10)' )  ikernel
      iargset = IMOD(ikernel, 10)
      ikernel = ikernel / 10
      write(*,*) 'kernel=', ikernel, ', argument set=',iargset
      call init(ikernel, iargset, n)
      if(ikernel .lt. 1 .or. ikernel .gt. 24) then
          write(*,*) 'kernel number wrong'
          stop
      endif
      if(iargset .lt. 1 .or. iargset .gt. 3) then
          write(*,*) 'argument set selction wrong'
          stop
      endif

c main loop
      DO 100 i=1,10

         if(ikernel .eq. 1) call kernel1()
         if(ikernel .eq. 2) call kernel2()
         if(ikernel .eq. 3) call kernel3()
         if(ikernel .eq. 4) call kernel4()
         if(ikernel .eq. 5) call kernel5()
         if(ikernel .eq. 6) call kernel6()
         if(ikernel .eq. 7) call kernel7()
         if(ikernel .eq. 8) call kernel8()
         if(ikernel .eq. 9) call kernel9()
         if(ikernel .eq. 10) call kernel10()
         if(ikernel .eq. 11) call kernel11()
         if(ikernel .eq. 12) call kernel12()
         if(ikernel .eq. 13) call kernel13()
         if(ikernel .eq. 14) call kernel14()
         if(ikernel .eq. 15) call kernel15()
         if(ikernel .eq. 16) call kernel16()
         if(ikernel .eq. 17) call kernel17()
         if(ikernel .eq. 18) call kernel18()
         if(ikernel .eq. 19) call kernel19()
         if(ikernel .eq. 20) call kernel20()
         if(ikernel .eq. 21) call kernel21()
         if(ikernel .eq. 22) call kernel22()
         if(ikernel .eq. 23) call kernel23()
         if(ikernel .eq. 24) call kernel24()

 100  CONTINUE
      end


C initialize dimensions for vector/arrays
      subroutine init(ikernel, iargset, n)
C special kernels
      if(ikernel .eq. 4) then
        n1=1024*32
        n2=1024*128
        n3=1024*512
      elseif(ikernel .eq. 6) then
        n1=128
        n2=512
        n3=1000
      elseif(ikernel .eq. 21) then
        n1=1024*32
        n2=1024*64
        n3=1024*128
      else
C default
        n1=1024*32
        n2=1024*256
        n3=4*1000*1000
      endif
      
c iargset contains selction 1,2,or 3 for 3 different data sizes
      if(iargset .EQ. 1) then
        n=n1
      elseif(iargset .EQ. 2) then
        n=n2
      else
        n=n3
      endif

      return
      end


      subroutine kernel1()
      include 'kernel.inc'
c*******************************************************************************
c***  KERNEL 1      HYDRO FRAGMENT
c*******************************************************************************
c
      write(*,*) 'kernel 1, n=', n
cdir$ ivdep
 1001    DO 1 k = 1,n
    1       X(k)= Q + Y(k) * (R * ZX(k+10) + T * ZX(k+11))

      return
      end

c*******************************************************************************
c***  KERNEL 2      ICCG EXCERPT (INCOMPLETE CHOLESKY - CONJUGATE GRADIENT)
c*******************************************************************************
c
c
      subroutine kernel2()
      include 'kernel.inc'

      write(*,*) 'kernel 2, n=', n
 1002     II= n
       IPNTP= 0
  222   IPNT= IPNTP
       IPNTP= IPNTP+II
          II= II/2
           i= IPNTP+1
cdir$ ivdep
c:ibm_dir:ignore recrdeps (x)
c
      DO 2 k= IPNT+2,IPNTP,2
           i= i+1
    2   X(i)= X(k) - V(k) * X(k-1) - V(k+1) * X(k+1)
          IF( II.GT.1) GO TO 222

      return
      end
c
c*******************************************************************************
c***  KERNEL 3      INNER PRODUCT
c*******************************************************************************
c
c
      subroutine kernel3()
      include 'kernel.inc'

      write(*,*) 'kernel 3, n=', n

 1003      Q= 0.000d0
      DO 3 k= 1,n
    3      Q= Q + Z(k) * X(k)
c
c...................

      return
      end
c
c*******************************************************************************
c***  KERNEL 4      BANDED LINEAR EQUATIONS
c*******************************************************************************
c
      subroutine kernel4()
      include 'kernel.inc'

      write(*,*) 'kernel 4, n=', n

              m= (1001-7)/2
             fw= 1.000d-25
c
 1004 DO 404  k= 7,ndim1,m
             lw= k-6
           temp= XZ(k-1)
cdir$ ivdep
      DO   4  j= 5,n,5
         temp  = temp   - XZ(lw) * Y(j)
    4        lw= lw+1
        XZ(k-1)= Y(5) * temp
 404  CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 5      TRI-DIAGONAL ELIMINATION, BELOW DIAGONAL (NO VECTORS)
c*******************************************************************************
c
c
      subroutine kernel5()
      include 'kernel.inc'

      write(*,*) 'kernel 5, n=', n

cdir$ novector
 1005 DO 5 i = 2,n
    5    X(i)= Z(i) * (Y(i) - X(i-1))
cdir$ vector
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 6      GENERAL LINEAR RECURRENCE EQUATIONS
c*******************************************************************************
c
c
      subroutine kernel6()
      include 'kernel.inc'

      write(*,*) 'kernel 6, n=', n

 1006 DO  6  i= 2,n
          W(i)= 0.0100d0
cdir$ novector
      DO  6  k= 1,i-1
          W(i)= W(i)  + B(i,k) * W(i-k)
    6 CONTINUE
cdir$ vector
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 7      EQUATION OF STATE FRAGMENT
c*******************************************************************************
c
c
      subroutine kernel7()
      include 'kernel.inc'

      write(*,*) 'kernel 7, n=', n

cdir$ ivdep
 1007 DO 7 k= 1,n
        X(k)=     U(k  ) + R*( Z(k  ) + R*Y(k  )) +
     1        T*( U(k+3) + R*( U(k+2) + R*U(k+1)) +
     2        T*( U(k+6) + Q*( U(k+5) + Q*U(k+4))))
    7 CONTINUE
c
c...................
      return
      end
c
c
c*******************************************************************************
c***  KERNEL 8      A.D.I. INTEGRATION
c*******************************************************************************
c
c
      subroutine kernel8()
      include 'kernel.inc'

      write(*,*) 'kernel 8, n=', n

 1008          nl1 = 1
               nl2 = 2
                fw= 2.000d0
      DO  8     kx = 2,3
cdir$ ivdep
      DO  8     ky = 2,n
            DU1(ky)=U1(kx,ky+1,nl1)  -  U1(kx,ky-1,nl1)
            DU2(ky)=U2(kx,ky+1,nl1)  -  U2(kx,ky-1,nl1)
            DU3(ky)=U3(kx,ky+1,nl1)  -  U3(kx,ky-1,nl1)
      U1(kx,ky,nl2)=U1(kx,ky,nl1) +A11*DU1(ky) +A12*DU2(ky) +A13*DU3(ky)
     1       + SIG*(U1(kx+1,ky,nl1) -fw*U1(kx,ky,nl1) +U1(kx-1,ky,nl1))
      U2(kx,ky,nl2)=U2(kx,ky,nl1) +A21*DU1(ky) +A22*DU2(ky) +A23*DU3(ky)
     1       + SIG*(U2(kx+1,ky,nl1) -fw*U2(kx,ky,nl1) +U2(kx-1,ky,nl1))
      U3(kx,ky,nl2)=U3(kx,ky,nl1) +A31*DU1(ky) +A32*DU2(ky) +A33*DU3(ky)
     1       + SIG*(U3(kx+1,ky,nl1) -fw*U3(kx,ky,nl1) +U3(kx-1,ky,nl1))
    8 CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 9      INTEGRATE PREDICTORS
c*******************************************************************************
c
c
      subroutine kernel9()
      include 'kernel.inc'

      write(*,*) 'kernel 9, n=', n

 1009 DO 9  k = 1,n
      PX( 1,k)= DM28*PX(13,k) + DM27*PX(12,k) + DM26*PX(11,k) +
     1          DM25*PX(10,k) + DM24*PX( 9,k) + DM23*PX( 8,k) +
     2          DM22*PX( 7,k) +  C0*(PX( 5,k) +      PX( 6,k))+ PX( 3,k)
    9 CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 10     DIFFERENCE PREDICTORS
c*******************************************************************************
c
c
      subroutine kernel10()
      include 'kernel.inc'

      write(*,*) 'kernel 10, n=', n

 1010 DO 10  k= 1,n
      AR      =      CX(5,k)
      BR      = AR - PX(5,k)
      PX(5,k) = AR
      CR      = BR - PX(6,k)
      PX(6,k) = BR
      AR      = CR - PX(7,k)
      PX(7,k) = CR
      BR      = AR - PX(8,k)
      PX(8,k) = AR
      CR      = BR - PX(9,k)
      PX(9,k) = BR
      AR      = CR - PX(10,k)
      PX(10,k)= CR
      BR      = AR - PX(11,k)
      PX(11,k)= AR
      CR      = BR - PX(12,k)
      PX(12,k)= BR
      PX(14,k)= CR - PX(13,k)
      PX(13,k)= CR
   10 CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 11     FIRST SUM.   PARTIAL SUMS.              (NO VECTORS)
c*******************************************************************************
c
c
      subroutine kernel11()
      include 'kernel.inc'

      write(*,*) 'kernel 11, n=', n

 1011     X(1)= Y(1)
cdir$ novector
      DO 11 k = 2,n
   11     X(k)= X(k-1) + Y(k)
cdir$ vector
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 12     FIRST DIFF.
c*******************************************************************************
c
c
      subroutine kernel12()
      include 'kernel.inc'

      write(*,*) 'kernel 12, n=', n

cdir$ ivdep
 1012 DO 12 k = 1,n
   12     X(k)= Y(k+1) - Y(k)
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 13      2-D PIC   Particle In Cell
c*******************************************************************************
c
      subroutine kernel13()
      include 'kernel.inc'

      write(*,*) 'kernel 13, n=', n

                fw= 1.000d0
c
 1013 DO  13     k= 1,n
                i1= P(1,k)
                j1= P(2,k)
                i1=       1 + MOD2N(i1,64)
                j1=       1 + MOD2N(j1,64)
            P(3,k)= P(3,k)  + B(i1,j1)
            P(4,k)= P(4,k)  + C(i1,j1)
            P(1,k)= P(1,k)  + P(3,k)
            P(2,k)= P(2,k)  + P(4,k)
                i2= P(1,k)
                j2= P(2,k)
                i2=            MOD2N(i2,64)
                j2=            MOD2N(j2,64)
            P(1,k)= P(1,k)  + Y(i2+32)
            P(2,k)= P(2,k)  + Z(j2+32)
                i2= i2      + E(i2+32)
                j2= j2      + F(j2+32)
          H(i2,j2)= H(i2,j2) + fw
   13 CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 14      1-D PIC   Particle In Cell
c*******************************************************************************
c
c
      subroutine kernel14()
      include 'kernel.inc'

      write(*,*) 'kernel 14, n=', n

               fw= 1.000d0
c
 1014 DO   141  k= 1,n
            VX(k)= 0.0d0
            XX(k)= 0.0d0
            IX(k)= INT(  GRD(k))
            XI(k)= REAL( IX(k))
           EX1(k)= EX   ( IX(k))
          DEX1(k)= DEX  ( IX(k))
 141  CONTINUE
c
      DO   142  k= 1,n
            VX(k)= VX(k) + EX1(k) + (XX(k) - XI(k))*DEX1(k)
            XX(k)= XX(k) + VX(k)  + FLX
            IR(k)= XX(k)
            RX(k)= XX(k) - IR(k)
            IR(k)= MOD2N(  IR(k),2048) + 1
            XX(k)= RX(k) + IR(k)
 142  CONTINUE
c
      DO  14    k= 1,n
      RH(IR(k)  )= RH(IR(k)  ) + fw - RX(k)
      RH(IR(k)+1)= RH(IR(k)+1) + RX(k)
  14  CONTINUE
c
c...................
      return
      end
c
c
c*******************************************************************************
c***  KERNEL 15     CASUAL FORTRAN.  DEVELOPMENT VERSION.
c*******************************************************************************
c
c
c       CASUAL ORDERING OF SCALAR OPERATIONS IS TYPICAL PRACTICE.
c       THIS EXAMPLE DEMONSTRATES THE NON-TRIVIAL TRANSFORMATION
c       REQUIRED TO MAP INTO AN EFFICIENT MACHINE IMPLEMENTATION.
c
c
      subroutine kernel15()
      include 'kernel.inc'

      write(*,*) 'kernel 15, n=', n

 1015          NG= 7
               NZ= n
               AR= 0.05300d0
               BR= 0.07300d0
        DO 45  j = 2,NG
        DO 45  k = 2,NZ
               IF( j-NG) 31,30,30
   30     VY(k,j)= 0.0d0
                   GO TO 45
   31          IF( VH(k,j+1) -VH(k,j)) 33,33,32
   32           T= AR
                   GO TO 34
   33           T= BR
   34          IF( VF(k,j) -VF(k-1,j)) 35,36,36
   35           R= MAX( VH(k-1,j), VH(k-1,j+1))
                S= VF(k-1,j)
                   GO TO 37
   36           R= MAX( VH(k,j),   VH(k,j+1))
                S= VF(k,j)
   37     VY(k,j)= SQRT( VG(k,j)**2 +R*R)*T/S
               IF( k-NZ) 40,39,39
   39     VS(k,j)= 0.0d0
                   GO TO 45
   40          IF( VF(k,j) -VF(k,j-1)) 41,42,42
   41           R= MAX( VG(k,j-1), VG(k+1,j-1))
                S= VF(k,j-1)
                T= BR
                   GO TO 43
   42           R= MAX( VG(k,j),   VG(k+1,j))
                S= VF(k,j)
                T= AR
   43     VS(k,j)= SQRT( VH(k,j)**2 +R*R)*T/S
   45    CONTINUE
c
c...................
      return
      end
c
c
c
c*******************************************************************************
c***  KERNEL 16     MONTE CARLO SEARCH LOOP
c*******************************************************************************
c
      subroutine kernel16()
      include 'kernel.inc'

      write(*,*) 'kernel 16, n=', n

            II= n/3
            LB= II+II
            k2= 0
            k3= 0
c
c
 1016        m= 1
            i1= m
  410       j2= (n+n)*(m-1)+1
      DO 470 k= 1,n
            k2= k2+1
            j4= j2+k+k
            j5= ZONE(j4)
            IF( j5-n      ) 420,475,450
  415       IF( j5-n+II   ) 430,425,425
  420       IF( j5-n+LB   ) 435,415,415
  425       IF( PLAN(j5)-R) 445,480,440
  430       IF( PLAN(j5)-S) 445,480,440
  435       IF( PLAN(j5)-T) 445,480,440
  440       IF( ZONE(j4-1)) 455,485,470
  445       IF( ZONE(j4-1)) 470,485,455
  450       k3= k3+1
            IF( D(j5)-(D(j5-1)*(T-D(j5-2))**2+(S-D(j5-3))**2
     1                        +(R-D(j5-4))**2)) 445,480,440
  455        m= m+1
            IF( m-ZONE(1) ) 465,465,460
  460        m= 1
  465       IF( i1-m) 410,480,410
  470 CONTINUE
  475 CONTINUE
  480 CONTINUE
  485 CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 17     IMPLICIT, CONDITIONAL COMPUTATION       (NO VECTORS)
c*******************************************************************************
c
c         RECURSIVE-DOUBLING VECTOR TECHNIQUES CAN NOT BE USED
c         BECAUSE CONDITIONAL OPERATIONS APPLY TO EACH ELEMENT.
c
      subroutine kernel17()
      include 'kernel.inc'

      write(*,*) 'kernel 17, n=', n

                 dw= 5.0000d0/3.0000d0
                 fw= 1.0000d0/3.0000d0
                 tw= 1.0300d0/3.0700d0
cdir$ novector
c
 1017             k= n
                  j= 1
                ink= -1
              SCALE= dw
                XNM= fw
                 E6= tw
                     GO TO 61
c                                            STEP MODEL
  60             E6= XNM*VSP(k)+VSTP(k)
            VXNE(k)= E6
                XNM= E6
             VE3(k)= E6
                  k= k+ink
                 IF( k.EQ.j) GO TO  62
  61             E3= XNM*VLR(k) +VLIN(k)
               XNEI= VXNE(k)
            VXND(k)= E6
                XNC= SCALE*E3
c                                            SELECT MODEL
                 IF( XNM .GT.XNC) GO TO  60
                 IF( XNEI.GT.XNC) GO TO  60
c                                            LINEAR MODEL
             VE3(k)= E3
                 E6= E3+E3-XNM
            VXNE(k)= E3+E3-XNEI
                XNM= E6
                  k= k+ink
                 IF( k.NE.j) GO TO 61
   62 CONTINUE
cdir$ vector
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 18     2-D EXPLICIT HYDRODYNAMICS FRAGMENT
c*******************************************************************************
c
c
      subroutine kernel18()
      include 'kernel.inc'

      write(*,*) 'kernel 18, n=', n

 1018           T= 0.003700d0
                S= 0.004100d0
               KN= 6
               JN= n
         DO 70  k= 2,KN
         DO 70  j= 2,JN
          ZA(j,k)= (ZP(j-1,k+1)+ZQ(j-1,k+1)-ZP(j-1,k)-ZQ(j-1,k))
     1            *(ZR(j,k)+ZR(j-1,k))/(ZM(j-1,k)+ZM(j-1,k+1))
          ZB(j,k)= (ZP(j-1,k)+ZQ(j-1,k)-ZP(j,k)-ZQ(j,k))
     1            *(ZR(j,k)+ZR(j,k-1))/(ZM(j,k)+ZM(j-1,k))
   70    CONTINUE
c
         DO 72  k= 2,KN
         DO 72  j= 2,JN
          ZU(j,k)= ZU(j,k)+S*(ZA(j,k)*(ZZ(j,k)-ZZ(j+1,k))
     1                    -ZA(j-1,k) *(ZZ(j,k)-ZZ(j-1,k))
     2                    -ZB(j,k)   *(ZZ(j,k)-ZZ(j,k-1))
     3                    +ZB(j,k+1) *(ZZ(j,k)-ZZ(j,k+1)))
          ZV(j,k)= ZV(j,k)+S*(ZA(j,k)*(ZR(j,k)-ZR(j+1,k))
     1                    -ZA(j-1,k) *(ZR(j,k)-ZR(j-1,k))
     2                    -ZB(j,k)   *(ZR(j,k)-ZR(j,k-1))
     3                    +ZB(j,k+1) *(ZR(j,k)-ZR(j,k+1)))
   72    CONTINUE
c
         DO 75  k= 2,KN
         DO 75  j= 2,JN
          ZR(j,k)= ZR(j,k)+T*ZU(j,k)
          ZZ(j,k)= ZZ(j,k)+T*ZV(j,k)
   75    CONTINUE
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 19      GENERAL LINEAR RECURRENCE EQUATIONS    (NO VECTORS)
c*******************************************************************************
c
      subroutine kernel19()
      include 'kernel.inc'

      write(*,*) 'kernel 19, n=', n

 1019            KB5I= 0
c
c     IF( JR.LE.1 )  THEN
cdir$ novector
             DO 191 k= 1,n
           B5(k+KB5I)= SA(k) +STB5*SB(k)
                 STB5= B5(k+KB5I) -STB5
  191        CONTINUE
c     ELSE
c
             DO 193 i= 1,n
                    k= n-i+1
           B5(k+KB5I)= SA(k) +STB5*SB(k)
                 STB5= B5(k+KB5I) -STB5
  193        CONTINUE
c     ENDIF
cdir$ vector
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 20     DISCRETE ORDINATES TRANSPORT: RECURRENCE (NO VECTORS)
c*******************************************************************************
c
      subroutine kernel20()
      include 'kernel.inc'

      write(*,*) 'kernel 20, n=', n

           dw= 0.200d0
cdir$ novector
c
 1020 DO 20 k= 1,n
           DI= Y(k)-G(k)/( XX(k)+DK)
           DN= dw
           IF( DI.NE.0.0) DN= MAX( S,MIN( Z(k)/DI, T))
         X(k)= ((W(k)+V(k)*DN)* XX(k)+U(k))/(VX(k)+V(k)*DN)
      XX(k+1)= (X(k)- XX(k))*DN+ XX(k)
   20 CONTINUE
cdir$ vector
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 21     MATRIX*MATRIX PRODUCT
c*******************************************************************************
c
c
      subroutine kernel21()
      include 'kernel.inc'

      write(*,*) 'kernel 21, n=', n

 1021 DO 21 k= 1,25
      DO 21 i= 1,25
      DO 21 j= 1,n
      PX(i,j)= PX(i,j) +VY(i,k) * CX(k,j)
   21 CONTINUE
c
c...................
      return
      end
c
c
c*******************************************************************************
c***  KERNEL 22     PLANCKIAN DISTRIBUTION
c*******************************************************************************
c
c
      subroutine kernel22()
      include 'kernel.inc'

      write(*,*) 'kernel 22, n=', n

       EXPMAX= 20.0000d0
           fw= 1.00000d0
         U(n)= 0.99000d0*EXPMAX*V(n)
c
 1022 DO 22 k= 1,n
                                            Y(k)= U(k)/V(k)
         W(k)= X(k)/( EXP( Y(k)) -fw)
   22 CONTINUE
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 23     2-D IMPLICIT HYDRODYNAMICS FRAGMENT
c*******************************************************************************
c
      subroutine kernel23()
      include 'kernel.inc'

      write(*,*) 'kernel 23, n=', n

            fw= 0.17500d0
c
 1023 DO 23  j= 2,6
      DO 23  k= 2,n
            QA= ZA(k,j+1)*ZR(k,j) +ZA(k,j-1)*ZB(k,j) +
     1          ZA(k+1,j)*ZU(k,j) +ZA(k-1,j)*ZV(k,j) +ZZ(k,j)
   23  ZA(k,j)= ZA(k,j) +fw*(QA -ZA(k,j))
c
c...................
      return
      end
c
c*******************************************************************************
c***  KERNEL 24     FIND LOCATION OF FIRST MINIMUM IN ARRAY
c*******************************************************************************
c
      subroutine kernel24()
      include 'kernel.inc'

      write(*,*) 'kernel 24, n=', n

       X( n/2)= -1.000d+10
c
 1024        m= 1
      DO 24  k= 2,n
            IF( X(k).LT.X(m))  m= k
   24 CONTINUE

      return
      end
c


c*******************************************************************************


      integer function MOD2N(i,j)
      mod2n = IAND(i,j-1)
      return
      end

c*******************************************************************************
      SUBROUTINE VALUES(i)
c***********************************************
c
c            VALUES     initializes special values
c
c     i    :=  kernel number
c
c****************************************************************************
      include 'kernel.inc'
      COMMON /BASE2/ P0(4,512), PX0(25,101), CX0(25,101),
     1  VY0(101,25), VH0(101,7), VF0(101,7), VG0(101,7), VS0(101,7),
     2  ZA0(101,7)  , ZP0(101,7), ZQ0(101,7), ZR0(101,7), ZM0(101,7),
     3  ZB0(101,7)  , ZU0(101,7), ZV0(101,7), ZZ0(101,7),
     4  B0(64,64), CC0(64,64), H0(64,64),
     5  U10(5,101,2),  U20(5,101,2),  U30(5,101,2)
      COMMON /SPACE3/ CACHE(8192)
c
c     ******************************************************************
C      CALL SIZES (i)
      IP1= i
C      CALL  SUPPLY( i)
c
      IF( IP1.NE.13 ) GO TO 14
            DS= 1.000d0
            DW= 0.500d0
      DO 205 j= 1,4
      DO 205 k= 1,512
      P(j,k)  = DS
      P0(j,k) = DS
            DS= DS + DW
  205 CONTINUE
c
      DO 210 j= 1,96
      E(j) = 1
      F(j) = 1
  210 CONTINUE
c
   14 IF( IP1.NE.14) GO TO 16
c
      mmin= 1
      mmax= 1001
C      CALL IQRANF( IX, mmin, mmax, 1001)
c
            DW= -100.000d0
      DO 215 J= 1,1001
      DEX(J) =  DW*DEX(J)
      GRD(J) = IX(J)
  215 CONTINUE
      FLX= 0.00100d0
c
   16 IF( IP1.NE.16 ) GO TO 50
            MC= 2
            lr= n
            II= lr/3
            FW= 1.000d-4
          D(1)= 1.0198048642876400d0
      DO 400 k= 2,300
  400     D(k)= D(k-1) + FW/D(k-1)
             R= D(lr)
            FW= 1.000d0
      DO 403 LX= 1,MC
             m= (lr+lr)*(LX-1)
      DO 401 j= 1,2
      DO 401 k= 1,lr
             m= m+1
             S= REAL(k)
       PLAN(m)= R*((S + FW)/S)
  401  ZONE(m)= k+k
  403 CONTINUE
             k= lr+lr+1
       ZONE(k)= lr
             S= D(lr-1)
             T= D(lr-2)
c
   50 CONTINUE

             j= 0
            sc= 0.0d0
      DO 777 k= 1,8192
            IF( CACHE(k).EQ. 0.0)  THEN
             j= j + k
            sc= sc + REAL(j*k)
            ENDIF
  777 CONTINUE
c
      RETURN
      END
c
c*******************************************************************************
