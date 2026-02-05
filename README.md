## bkgman -bhargav's package manager

### this is custom package manager for the lfs linux from scratch build inspired from arch-pacman and makepkg

#### current features
-fakeroot based installation using JPKGBUILD
-dependacy check (no automatic resolution available currently)


bkgman 

-I install                   ;
-Iy sync and install         ;   
-Iyu sync and update all     ;

-U install from tar          ; done   

-R remove                    ; done 

-Q query on local db         ; done  
-Qs search on mirrors

-h help                      ; done


jpkg                         ; done


to be :

1. implement hooks                  ;done
2. implement dependency check only  ;done
3. setup sync and mirrors
4. implement dependency resolution
