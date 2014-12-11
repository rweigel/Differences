c
c
c This is a sample main program, calculating the T01 model field produced by the
c external (magnetospheric) sources of the geomagnetic field at a specified point
c of space {XGSM,YGSM,ZGSM}.  The earth's dipole tilt angle PS should be specified
c in radians, the solar wind ram pressure PDYN in nanoPascals, Dst index, IMF By
c and Bz components in nT. The two IMF-related indices G1 and G2 take into account
c the IMF and solar wind conditions during the preceding 1-hour interval; their
c exact definition is given in the paper "A new data-based model of the near
c magnetosphere magnetic field. 2. Parameterization and fitting to observations".
c The paper is available online from anonymous ftp-area www-istp.gsfc.nasa.gov,
c /pub/kolya/T01.
c
c
 1    PRINT *, '  XGEO,YGEO,ZGEO,J,IYR'
      READ *, XGEO,YGEO,ZGEO,J,IYR

C
      J        = 0
      IYR      = 1998
      
      CALL GEOMAG(XGEO,YGEO,ZGEO,XMAG,YMAG,ZMAG,J,IYR)

      PRINT *, '  XGEO,YGEO,ZGEO=',XMAG,YMAG,ZMAG
      GOTO 1

      END
c
c
