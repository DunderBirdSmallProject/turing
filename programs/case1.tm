#Q = {start, halt_accept, reject, read_a, read_b, back, match, ac1, ac2, ac3, ac4, erase_l, erase_r, rj1, rj2, rj3, rj4, rj5}

#S = {a, b}
#G = {a, b, _, t, r, u, e, f, a, l, s, e}
#q0 = start
#B = _

#F = {halt_accept}
#N = 2

start a_ _a rr read_a
start b_ b_ ** erase_l
start __ __ ** erase_l

read_a a_ _a rr read_a
read_a b_ _b rr read_b
read_a __ __ l* erase_l

read_b b_ _b rr read_b
read_b a_ a_ *l back
read_b __ __ l* erase_l

back aa aa *l back
back ab ab *l back
back a_ a_ *r match

match aa __ rr match
match bb __ rr match
match ab ab ** erase_l
match ba ba ** erase_l
match __ __ ** ac1
match _a _a ** erase_l
match _b _b ** erase_l
match a_ a_ ** erase_l
match b_ b_ ** erase_l

;; print true
ac1 __ t_ r* ac2
ac2 __ r_ r* ac3
ac3 __ u_ r* ac4
ac4 __ e_ ** halt_accept

;; go to the leftmost position
erase_l aa aa ll erase_l
erase_l ab ab ll erase_l
erase_l ba ba ll erase_l
erase_l bb bb ll erase_l
erase_l a_ a_ l* erase_l
erase_l b_ b_ l* erase_l
erase_l _a _a *l erase_l
erase_l _b _b *l erase_l
erase_l __ __ rr erase_r

;; clear all symbols on the tape
erase_r aa __ rr erase_r
erase_r ab __ rr erase_r
erase_r ba __ rr erase_r
erase_r bb __ rr erase_r
erase_r a_ __ r* erase_r
erase_r b_ __ r* erase_r
erase_r _a __ *r erase_r
erase_r _b __ *r erase_r
erase_r __ __ ** rj1

;; print false
rj1 __ f_ r* rj2
rj2 __ a_ r* rj3
rj3 __ l_ r* rj4
rj4 __ s_ r* rj5
rj5 __ e_ ** reject
