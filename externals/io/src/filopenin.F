      SUBROUTINE FILOPENIN(LUN)
C     -------------------------
      IMPLICIT NONE
C
      INTEGER LUN
      CHARACTER*255 CHEVIFIL
C
      CALL GETENV('EVIFIL',CHEVIFIL)
#if defined(AIX)
      OPEN(UNIT=LUN,FILE=CHEVIFIL,STATUS='OLD',ACTION='READ',ERR=100)
#else
#if defined(Linux)
      OPEN(UNIT=LUN,FILE=CHEVIFIL,STATUS='OLD',ERR=100)
#else
      OPEN(UNIT=LUN,FILE=CHEVIFIL,STATUS='OLD',READONLY,ERR=100)
#endif
#endif
      RETURN
C
 100  CONTINUE
      WRITE(6,'('' error opening event-input file - stop '')')
      STOP
      END
