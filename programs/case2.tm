#Q = {start, left, right_start, right,  back1, copy, back2, match, halt_accept, reject, ac1, ac2, ac3, ac4, rj1, rj2, rj3, rj4, rj5, erase_l, erase_r, clear_tape2}
#N = 3
#B = _
#S = {1, x, =}
#G = {1, x, =, _, t, r, u, e, f, a, l, s}
#q0 = start
#F = {halt_accept}

start 1__ _1_ rr* left
start x__ x__ *** erase_l
start =__ =__ *** erase_l
start ___ ____ *** erase_l

left 1__ _1_ rr* left
left x__ ___ r** right
left =__ ___ *** erase_l
left ___ ___ *** erase_l

right_start 1__ ___ *l* back1
right_start x__ x__ *** erase_l
right_start =__ =__ *** erase_l
right_start ___ ___ *** erase_l

right 1__ 1__ *l* back1
right =__ ___ r*l back2
right x__ x__ *ll erase_l
right ___ ___ lll erase_l

back1 11_ 11_ *l* back1
back1 1__ 1__ *r* copy

copy 11_ 111 *rr copy
copy 1__ ___ r** right

back2 1_1 1_1 **l back2
back2 1__ 1__ **r match
back2 x_1 x_1 *l* erase_l
back2 =_1 =_1 *l* erase_l

match 1_1 ___ r*r match
match __1 __1 ll* erase_l
match 1__ 1__ *ll erase_l
match x_1 x_1 *l* erase_l
match =_1 =_1 *l* erase_l
match x__ x__ *l* erase_l
match =__ =__ *l* erase_l
match ___ ___ *l* clear_tape2

;; erase_l: move the the leftmost
erase_l __1 __1 **l erase_l
erase_l _1_ _1_ *l* erase_l
erase_l _11 _11 *ll erase_l

erase_l 1__ 1__ l** erase_l
erase_l =__ =__ l** erase_l
erase_l x__ x__ l** erase_l

erase_l 1_1 1_1 l*l erase_l
erase_l =_1 =_1 l*l erase_l
erase_l x_1 x_1 l*l erase_l

erase_l 11_ 11_ ll* erase_l
erase_l x1_ x1_ ll* erase_l
erase_l =1_ =1_ ll* erase_l

erase_l 111 111 lll erase_l
erase_l x11 x11 lll erase_l
erase_l =11 =11 lll erase_l

erase_l ___ ___ rrr erase_r

;; erase_r: keep cleaning until reach the rightmost
erase_r __1 ___ **r erase_r
erase_r _1_ ___ *r* erase_r
erase_r _11 ___ *rr erase_r

erase_r 1__ ___ r** erase_r
erase_r =__ ___ r** erase_r
erase_r x__ ___ r** erase_r

erase_r 1_1 ___ r*r erase_r
erase_r =_1 ___ r*r erase_r
erase_r x_1 ___ r*r erase_r

erase_r 11_ ___ rr* erase_r
erase_r x1_ ___ rr* erase_r
erase_r =1_ ___ rr* erase_r

erase_r 111 ___ rrr erase_r
erase_r x11 ___ rrr erase_r
erase_r =11 ___ rrr erase_r

erase_r ___ ___ *** rj1

;; clear_tape2: clear symbols on the tape 2
clear_tape2 _1_ ___ *l* clear_tape2
clear_tape2 ___ ___ *** ac1

;; accept (print true)
ac1 ___ t__ r** ac2
ac2 ___ r__ r** ac3
ac3 ___ u__ r** ac4
ac4 ___ e__ *** halt_accept

;; reject (print false)
rj1 ___ f__ r** rj2
rj2 ___ a__ r** rj3
rj3 ___ l__ r** rj4
rj4 ___ s__ r** rj5
rj5 ___ e__ *** reject
