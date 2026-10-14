C
      INTEGER MAXSTK             ! max. number of events kept in buffer
      PARAMETER (MAXSTK=10)      ! 
      INTEGER MAXTAG             ! max. number of tags in buffer
      PARAMETER (MAXTAG=100)     !
      INTEGER MAXDAT             ! max. number of data (lines) in buffer
      PARAMETER (MAXDAT=2000000)   !
      INTEGER MAXDTG             ! max. number of data (lines) per tag
      PARAMETER (MAXDTG=2000000)   !
      CHARACTER*255 CHTAG(MAXTAG)! list of tags
      CHARACTER*255 CHDAT(MAXDAT)! list of data words
      INTEGER NTAGS(0:MAXSTK)    ! adress of last tag for this stack in buffer
      INTEGER NDATS(0:MAXSTK)    ! adress of last data word for this stack in buffer
      INTEGER NDATAG(MAXTAG)     ! number of data words per tag
      INTEGER IDATAG(MAXTAG)     ! current data word read per tag
      INTEGER IADR(MAXDTG,MAXTAG)! adress list of data words for specific
                                 ! tag for chdat array
      INTEGER NDRUN(MAXSTK)      ! run numbers
      INTEGER NDEVT(MAXSTK)      ! event numbers
      INTEGER IEVTP(MAXSTK)      ! event typs
      LOGICAL NEW_RUN(MAXSTK)    ! flags for start-of-run
      LOGICAL NEW_EVE(MAXSTK)    ! flags for event
      INTEGER ISTACK             ! current stack counter
      INTEGER IFLAG              ! controls stack handling for
C                                ! next event (EVREAD,EVDEF..,
C                                !             EVSKIP..,EVFIND..)
C                                ! IFLAG = 0 overwrite old event
C                                !         1 add event to stack
C                                !         2 merge with old event
C
      COMMON /IOEVENT/ NDRUN,NDEVT,IEVTP,NTAGS,NDATS,
     +                 NDATAG,IDATAG,IADR,CHTAG,CHDAT,NEW_RUN,NEW_EVE,
     +                 ISTACK,IFLAG
