; simple turing description

#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

#S = {0,1} ; input alphabet
#G = {0,1,_,t,r,u,e,f,a,l,s}  ; tape alphabet

#q0 = 0
#B = _  ; blank line
#F = {halt_accept}
#N = 2  ; number of tapes

; the transition function is a little
; different

; take care of that!!!
cmp 01 __ rl reject  ; transition function
