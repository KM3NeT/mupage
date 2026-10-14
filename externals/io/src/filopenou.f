      SUBROUTINE FILOPENOU(LUN)
C     -------------------------
      IMPLICIT NONE
C
      INTEGER LUN
      CHARACTER*255 CHEVOFIL
C
      CALL GETENV('EVOFIL',CHEVOFIL)
      OPEN(UNIT=LUN,FILE=CHEVOFIL,ERR=100)
      RETURN
C
 100  CONTINUE
      WRITE(6,'('' error opening event-output file - stop '')')
      STOP
      END
