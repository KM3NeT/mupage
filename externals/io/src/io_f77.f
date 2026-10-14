      SUBROUTINE EVREAD(LIN,IERR)
C     ---------------------------
C      read in next event from input stream
C      input LIN : unit number of input stream
C      output  IERR    : error flag
C                       0 - o.k.
C                       1 - read error at LIN
C                       2 - unexpected EoF
C                       3 - unexpected input tag
C                       4 - too many data words or tags
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LIN,IERR
C
      CALL READHEAD(LIN,IERR)
      IF (IERR.NE.0) GOTO 99
      CALL READBODY(LIN,IERR)
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE EVDFRU(NRUN,IERR)
C     ----------------------------
C      define run number, set start-of-run condition for
C      next EVWRIT
C      clear chtag and chdat buffers
C      input NRUN  : run number
C            IFLAG : 0 replace highest stack event (default)
C                    1 add stack
C      output IERR : error flag
C                       0 - o.k.
C                       1 - invalid run number
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER NRUN,IERR
C
      IERR  = 0
      NTAGS(0) = 0
      NDATS(0) = 0
      IF (IFLAG.EQ.0) THEN
       IF (ISTACK.EQ.0) ISTACK = 1 
      ELSEIF (IFLAG.EQ.1) THEN
       ISTACK = ISTACK + 1
       IF (ISTACK.GT.MAXSTK) THEN
        IERR = 5
        GOTO 99
       ENDIF
      ELSE
        IERR = 6
        GOTO 99
      ENDIF
      NTAGS(ISTACK)   = NTAGS(ISTACK-1)
      NDATS(ISTACK)   = NDATS(ISTACK-1)
      NEW_RUN(ISTACK) = .FALSE.
      NEW_EVE(ISTACK) = .FALSE.
C
      IF (NRUN.LE.0) THEN
       IERR = 1
       GOTO 99
      ENDIF
C
      NDRUN(ISTACK) = NRUN
      IEVTP(ISTACK) = 0
      NDEVT(ISTACK) = 0
      NEW_RUN(ISTACK) = .TRUE.
      NEW_EVE(ISTACK) = .TRUE.
C
 99   CONTINUE
      IFLAG = 0
      RETURN
      END
      SUBROUTINE EVDFEV(NEV,IEV,IERR)
C     -------------------------------
C      define event number, event type,
C      clear chtag and chdat buffers
C      input NEV   : event number
C      input IEV   : event type
C            IFLAG : 0 replace highest stack event (default)
C                    1 add stack
C      output IERR : error flag
C                       0 - o.k.
C                       1 - no run number defined
C                       2 - invalid event number
C                       3 - invalid event type
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER NEV,IEV,IERR
C
      IERR  = 0
      NTAGS(0) = 0
      NDATS(0) = 0
      IF (IFLAG.EQ.0) THEN
       IF (ISTACK.EQ.0) ISTACK = 1 
      ELSEIF (IFLAG.EQ.1) THEN
       ISTACK = ISTACK + 1
       IF (ISTACK.GT.MAXSTK) THEN
        IERR = 5
        GOTO 99
       ENDIF
      ELSE
        IERR = 6
        GOTO 99
      ENDIF
      NTAGS(ISTACK)   = NTAGS(ISTACK-1)
      NDATS(ISTACK)   = NDATS(ISTACK-1)
      NEW_RUN(ISTACK) = .FALSE.
      NEW_EVE(ISTACK) = .FALSE.
C
      IF (NDRUN(ISTACK).LE.0) THEN
       IERR = 1
       GOTO 99
      ENDIF
      IF (NEV.LE.0) THEN
       IERR = 2
       GOTO 99
      ENDIF
      IF (IEV.LE.0) THEN
       IERR = 3
       GOTO 99
      ENDIF
C
      IEVTP(ISTACK) = IEV
      NDEVT(ISTACK) = NEV
      NEW_EVE(ISTACK) = .TRUE.
C
 99   CONTINUE
      IFLAG = 0
      RETURN
      END
      SUBROUTINE EVSETSTK(IFL)
C     ------------------------
C      set stack definition flag for next READHEAD,EVDEFRU,EVDEFEV
C      IFL : 0 replace highest stack event (default)
C            1 add stack
C            2 merge event with existing one
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER IFL
C
      IFLAG = IFL
C
      RETURN
      END
      SUBROUTINE EVDELETE(IFLDEL)
C     ---------------------------
C      delete event from stack
C      clear chtag and chdat buffers
C      input IFLDEL : 0 delete all events -> ISTACK = 0
C                     1 delete last event -> ISTACK = ISTACK - 1
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER IFLDEL
C
      NTAGS(0) = 0
      NDATS(0) = 0
      IF (ISTACK.GT.0) THEN
       IF (IFLDEL.EQ.0) ISTACK = 0 
       IF (IFLDEL.EQ.1) ISTACK = ISTACK - 1
      ENDIF
C
      RETURN
      END
      INTEGER FUNCTION NDATEV(CHX)
C     ----------------------------
C     passes number of data words for tag CHX
C     resets tag counter IDATAG in io_f77.inc
C     input CHT
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      CHARACTER*(*) CHX
      CHARACTER*255 CHT
      INTEGER I,NT,NTG
C
      CHT = CHX
      NT = 0
      NTG = NTAGS(ISTACK) - NTAGS(ISTACK-1)
      IF (NTG.GT.0) THEN
       DO I=NTAGS(ISTACK-1)+1,NTAGS(ISTACK)
        IF (CHT.EQ.CHTAG(I)) THEN
         NT = NDATAG(I)
         IDATAG(I) = 0
         GOTO 99
        ENDIF
       ENDDO
      ENDIF
C
 99   CONTINUE
      NDATEV = NT
      RETURN
      END
      CHARACTER*255 FUNCTION EVTAGN(CHX)
C     -----------------------------------
C     passes next data word CHD for tag CHX
C     input CHX
C     output CHD
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      CHARACTER*(*) CHX
      CHARACTER*255 CHT,CHD
      INTEGER I,J,NTG
C
      CHT = CHX
      CHD = ' '
      NTG = NTAGS(ISTACK) - NTAGS(ISTACK-1)
      IF (NTG.GT.0) THEN
       DO I=NTAGS(ISTACK-1)+1,NTAGS(ISTACK)
        IF (CHT.EQ.CHTAG(I)) THEN
         IDATAG(I) = IDATAG(I) + 1
         J         = IDATAG(I)
         IF (J.LE.NDATAG(I)) THEN
          CHD = CHDAT(IADR(J,I))
          GOTO 99
         ENDIF 
        ENDIF
       ENDDO
      ENDIF
C
 99   CONTINUE
      EVTAGN = CHD
      RETURN
      END
      SUBROUTINE EVINFO(NEW_R,NEW_E,NRUN,NEV,IEV)
C     -------------------------------------------
C      passes event informations
C      input : none
C      output NEW_RUN : boolean, true if start-of-run
C             NEW_E   : boolean, true if valid event 
C             NRUN    : runnumber
C             NEV     : event number 
C             IEV     : event type
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      LOGICAL NEW_R,NEW_E
      INTEGER NRUN,NEV,IEV
C
      NEW_R = NEW_RUN(ISTACK)
      NEW_E = NEW_EVE(ISTACK)
      NRUN  = NDRUN(ISTACK)
      NEV   = NDEVT(ISTACK)
      IEV   = IEVTP(ISTACK)
      RETURN
      END
      SUBROUTINE EVWRIT(LOUT,IERR)
C     ---------------------------
C      write next event into output stream
C      input  LOUT    : unit number of output stream
C      output IERR    : error flag
C                       0 - o.k.
C                       1 - no event defined
C                       2 - write error at LOUT
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LOUT,I,J,N1,N2,IERR
      INTEGER LENOCC,NTG
      CHARACTER*255 CHT,CHD
      CHARACTER*2 CHCOL
      DATA CHCOL /': '/
C
      IERR  = 0
C
C--- event defined ?
C
      IF (.NOT. NEW_EVE(ISTACK)) THEN
        IERR = 1
        GOTO 99
      ENDIF
C
C--- header line
C
      IF (NEW_RUN(ISTACK)) THEN
       WRITE(LOUT,'(''start_run: '',I8)',ERR=90) NDRUN(ISTACK)
      ELSE
       WRITE(LOUT,'(''start_event: '',2I8)',ERR=90) 
     +         NDEVT(ISTACK),IEVTP(ISTACK)
      ENDIF
C
C--- loop over tags
C
      NTG = NTAGS(ISTACK) - NTAGS(ISTACK-1)
      IF (NTG.GT.0) THEN
       DO I=NTAGS(ISTACK-1)+1,NTAGS(ISTACK)
        CHT = CHTAG(I)
        DO J=1,NDATAG(I)
         CHD = CHDAT(IADR(J,I))
         N1 = LENOCC(CHT)
         N2 = LENOCC(CHD)
         WRITE(LOUT,'(3A)',ERR=90) CHT(1:N1),CHCOL,CHD(1:N2)
        ENDDO
       ENDDO
      ENDIF
C
C--- trailer
C
      WRITE(LOUT,'(''end_event:'')',ERR=90)
      GOTO 99
C
 90   CONTINUE
      IERR = 2
C
 99   CONTINUE
      RETURN
      END
      INTEGER FUNCTION EVTAGD(CHX)
C     ----------------------------
C     eliminates tag CHT from tag list and
C      corresponding CHD from data list
C     input  - CHX
C     output - number of entries, which have been deleted
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      CHARACTER*(*) CHX
      CHARACTER*255 CHT
      INTEGER I,J,K,ND,NTG
C
      CHT = CHX
      ND = 0
C
      NTG = NTAGS(ISTACK) - NTAGS(ISTACK-1)
      IF (NTG.GT.0) THEN
       DO I=NTAGS(ISTACK-1)+1,NTAGS(ISTACK)
        IF (CHT.EQ.CHTAG(I)) THEN
         ND = NDATAG(I)
         IF (I.LT.NTAGS(ISTACK)) THEN
          DO J=I+1,NTAGS(ISTACK)
           NDATAG(J-1) = NDATAG(J)
           IDATAG(J-1) = IDATAG(J)
           CHTAG(J-1)  = CHTAG(J)
           DO K=1,NDATAG(J)
            IADR(K,J-1) = IADR(K,J)
           ENDDO
          ENDDO
         ENDIF
         NTAGS(ISTACK) = NTAGS(ISTACK) - 1
         GOTO 99
        ENDIF
       ENDDO
      ENDIF
C
 99   CONTINUE
      EVTAGD = ND
      RETURN
      END
      SUBROUTINE EVTAGA(CHX,CHY,IERR)
C     ---------------------------------
C     add data word CHY for tag CHX to lists
C     input CHX
C     input CHY
C     output IERR = 0 , o.k.
C                   1 , no event defined 
C                   2 , too many data words or tags
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      CHARACTER*(*) CHX,CHY
      CHARACTER*255 CHT,CHD
      INTEGER IERR,I,NTG
C
      CHT = CHX
      CHD = CHY
      IERR = 0
C
C
C--- new event not defined
C
      IF (.NOT. NEW_EVE(ISTACK)) THEN
        IERR = 1
        GOTO 99
      ENDIF
C
C--- add data word to list
C
      IF (NDATS(ISTACK).GE.MAXDAT) THEN
       IERR = 2
       GOTO 99
      ELSE
       NDATS(ISTACK) = NDATS(ISTACK) + 1
       CHDAT(NDATS(ISTACK)) = CHD
      ENDIF
C
C--- try to find tag in existing list
C
      NTG = NTAGS(ISTACK) - NTAGS(ISTACK-1)
      IF (NTG.GT.0) THEN
       DO I=NTAGS(ISTACK-1)+1,NTAGS(ISTACK)
        IF (CHTAG(I).EQ.CHT) THEN
         IF (NDATAG(I).GE.MAXDTG) THEN
          IERR = 2
          GOTO 99
         ELSE
          NDATAG(I) = NDATAG(I) + 1
          IADR(NDATAG(I),I) = NDATS(ISTACK)
          GOTO 99
         ENDIF
        ENDIF
       ENDDO
      ENDIF
C
C--- tag not found in list, add to list
C
      IF (NTAGS(ISTACK).GE.MAXTAG) THEN
       IERR = 2
       GOTO 99
      ELSE
       NTAGS(ISTACK) = NTAGS(ISTACK) + 1
       CHTAG(NTAGS(ISTACK))  = CHT
       NDATAG(NTAGS(ISTACK)) = 1
       IDATAG(NTAGS(ISTACK)) = 0
       IADR(1,NTAGS(ISTACK)) = NDATS(ISTACK)
      ENDIF
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE EVSKIPEV(LIN,NSKIP,IERR)
C     -----------------------------------
C      read in  event from input stream
C      after skipping NSKIP events
C      input LIN : unit number of input stream
C          NSKIP : number of events to skip
C      output  IERR    : error flag
C                       0 - o.k.
C                       1 - read error at LIN
C                       2 - unexpected EoF
C                       3 - unexpected input tag
C                       4 - too many data words or tags
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LIN,IERR,NSKIP,MSKIP
C
      MSKIP = 0
C
 10   CONTINUE
      CALL READHEAD(LIN,IERR)
      IF (IERR.EQ.3) GOTO 10
      IF (IERR.NE.0) GOTO 99
C
      IF (NEW_RUN(ISTACK)) THEN
       MSKIP = NSKIP + 1
      ELSEIF (NEW_EVE(ISTACK)) THEN
       MSKIP = MSKIP + 1
      ELSE
       IERR = 3
       GOTO 99
      ENDIF
C
      IF (MSKIP.LE.NSKIP) GOTO 10
C
      CALL READBODY(LIN,IERR)
      GOTO 99
C
 90   CONTINUE
      IERR = 3
      GOTO 99
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE EVSKIPRU(LIN,NSKIP,IERR)
C     -----------------------------------
C      read in start-of-run event from input stream
C      after skipping NSKIP runs
C      input LIN : unit number of input stream
C          NSKIP : number of events to skip
C      output  IERR    : error flag
C                       0 - o.k.
C                       1 - read error at LIN
C                       2 - unexpected EoF
C                       3 - unexpected input tag
C                       4 - too many data words or tags
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LIN,IERR,NSKIP,MSKIP
C
      MSKIP = 0
C
 10   CONTINUE
      CALL READHEAD(LIN,IERR)
      IF (IERR.EQ.3) GOTO 10
      IF (IERR.NE.0) GOTO 99
C
      IF (NEW_RUN(ISTACK)) THEN
        MSKIP = MSKIP + 1
        IF (MSKIP.GT.NSKIP) GOTO 90
      ENDIF
      GOTO 10
C
 90   CONTINUE
      CALL READBODY(LIN,IERR)
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE EVFINDEV(LIN,NEV,IERR)
C     ---------------------------------
C      read in  event from input stream
C      with specific ID NEV
C      input LIN : unit number of input stream
C          NEV   : event ID to be found
C      output  IERR    : error flag
C                       0 - o.k.
C                       1 - read error at LIN
C                       2 - unexpected EoF
C                       3 - unexpected input tag
C                       4 - too many data words or tags
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LIN,IERR,NEV
C
 10   CONTINUE
      CALL READHEAD(LIN,IERR)
      IF (IERR.EQ.3) GOTO 10
      IF (IERR.NE.0) GOTO 99
C
      IF (NEW_RUN(ISTACK)) GOTO 90
      IF (NDEVT(ISTACK).EQ.NEV) GOTO 90
      GOTO 10
C
 90   CONTINUE
      CALL READBODY(LIN,IERR)
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE EVFINDRU(LIN,NRU,IERR)
C     ---------------------------------
C      read in  start-of-run event from input stream
C      with specific ID NRU
C      input LIN : unit number of input stream
C            NRU : run ID to be found
C      output  IERR    : error flag
C                       0 - o.k.
C                       1 - read error at LIN
C                       2 - unexpected EoF
C                       3 - unexpected input tag
C                       4 - too many data words or tags
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LIN,IERR,NRU
C
 10   CONTINUE
      CALL READHEAD(LIN,IERR)
      IF (IERR.EQ.3) GOTO 10
      IF (IERR.NE.0) GOTO 99
C
      IF (NEW_RUN(ISTACK).AND.NDRUN(ISTACK).EQ.NRU) GOTO 90
      GOTO 10
C
 90   CONTINUE
      CALL READBODY(LIN,IERR)
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE READHEAD(LIN,IERR)
C     -----------------------------
C      read in next header line from input stream
C      input LIN : unit number of input stream
C            IFLAG : 0 replace highest stack event
C                    1 add stack
C                    2 merge event with existing one
C      output IERR    : error flag
C                       0 - o.k.
C                       1 - read error at LIN
C                       2 - unexpected EoF
C                       3 - corrupted 'start_*' line
C                       5 - stack overflow (for IFLAG = 1)
C                       6 - invalid choice for IFLAG
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      INTEGER LIN,NT,ND,IERR
      CHARACTER*255 CHT,CHD
C
      IERR  = 0
      NTAGS(0) = 0
      NDATS(0) = 0
      IF (IFLAG.NE.2) THEN
       IF (IFLAG.EQ.0) THEN
        IF (ISTACK.EQ.0) ISTACK = 1 
       ELSEIF (IFLAG.EQ.1) THEN
        ISTACK = ISTACK + 1
        IF (ISTACK.GT.MAXSTK) THEN
         IERR = 5
         GOTO 99
        ENDIF
       ELSE
         IERR = 6
         GOTO 99
       ENDIF
       NTAGS(ISTACK)   = NTAGS(ISTACK-1)
       NDATS(ISTACK)   = NDATS(ISTACK-1)
       NEW_RUN(ISTACK) = .FALSE.
       NEW_EVE(ISTACK) = .FALSE.
      ENDIF
C
 10   CONTINUE
      CALL READLINE(LIN,CHT,NT,CHD,ND,IERR)
      IF (IERR.NE.0) GOTO 99
C
C--- new start-of-run event
C
      IF (CHT(1:NT).EQ.'start_run') THEN
       IF (IFLAG.NE.2) THEN
        READ(CHD,*,ERR=90) NDRUN(ISTACK)
        NDEVT(ISTACK) = 0
        IEVTP(ISTACK) = 0
        NEW_RUN(ISTACK) = .TRUE.
        NEW_EVE(ISTACK) = .TRUE.
       ENDIF
C
C--- new event
C
      ELSEIF (CHT(1:NT).EQ.'start_event') THEN
       IF (IFLAG.NE.2) THEN
        NEW_RUN(ISTACK) = .FALSE.
        READ(CHD,*,ERR=90) NDEVT(ISTACK),IEVTP(ISTACK)
        NEW_RUN(ISTACK) = .FALSE.
        NEW_EVE(ISTACK) = .TRUE.
       ENDIF
C
C--- unexpected tag, try next line
C
      ELSE
       GOTO 10
      ENDIF
      GOTO 99
C
 90   CONTINUE
      IERR = 3
C
 99   CONTINUE
      IFLAG = 0
      RETURN
      END
      SUBROUTINE READBODY(LIN,IERR)
C     -----------------------------
C     reads event body after header line on input file at unit LIN
C     fills data into common IOEVENT
C     error codes : 0 - o.k.
C                   1 - read error on LIN
C                   2 - unexpected EoF on LIN
C                   3 - unexpected tag in event body
C                   4 - too many data words or tags
C
      IMPLICIT NONE
C
      INCLUDE 'io_f77.inc'
C
      CHARACTER*255 CHT,CHD
      INTEGER LIN,NT,ND,IERR
C
 10   CONTINUE
      CALL READLINE(LIN,CHT,NT,CHD,ND,IERR)
      IF (IERR.NE.0) GOTO 99
C
C--- unexpected tag
C
      IF (CHT.EQ.'start_run' .OR. CHT.EQ.'start_event') THEN
       IERR = 3
       GOTO 99
      ENDIF
C
C--- read event finished
C
      IF (CHT.EQ.'end_event') goto 99
C
C--- normal data line, store CHT, CHD
C
      CALL EVTAGA(CHT,CHD,IERR)
      IF (IERR.GT.0) THEN
       IERR = IERR + 2
       GOTO 99
      ENDIF
      GOTO 10
C
 99   CONTINUE
      RETURN
      END
      SUBROUTINE READLINE(LIN,CHT,NT,CHD,ND,IERR)
C     -------------------------------------------
C     reads next line on input file at unit LIN
C     fills character CHT and character CHD
C     indicating the length of CHT and CHD in NT and ND
C     without trailing blanks
C     error codes : 0 - o.k.
C                   1 - read error on LIN
C                   2 - unexpected EoF on LIN
C
      IMPLICIT NONE
C
      CHARACTER*255 CHL,CHT,CHD
      INTEGER LIN,NT,ND,NL,IERR,I,J
      INTEGER LENOCC
C
      CHD  = ' '
      CHT  = ' '
      NT   = 0
      ND   = 0
      IERR = 0
 10   CONTINUE
      READ (LIN,'(A)',ERR=90,END=95) CHL
      NL = LENOCC(CHL)
C
C--- ignore empty lines
C
      IF (NL.LE.1) GOTO 10
C
C--- find split into tag and data word
C--- if not successful read next line
C
      DO I=2,NL
       IF (CHL(I:I).EQ.':') THEN
        CHT = CHL(1:I-1)
        NT  = LENOCC(CHT)
        IF (I.LT.NL) THEN
         CHD = CHL(I+1:NL)
         DO J=1,LEN(CHD)
          IF (CHD(J:J).NE.' ') GOTO 20
         ENDDO
 20      CONTINUE
         IF (J.GT.1)
     +    CALL CCOPYL (CHD(J:J),CHD,LEN(CHD)-J+1)
         ND  = LENOCC(CHD)
        ENDIF
        GOTO 99 
       ENDIF
      ENDDO
      GOTO 10
C
 90   CONTINUE
      IERR = 1
      GOTO 99
C
 95   CONTINUE
      IERR = 2
      GOTO 99
C
 99   CONTINUE
      RETURN
      END
      
