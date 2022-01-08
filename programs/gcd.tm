; check gcd of 2 numbers
; Input: a string of 0's and 1's, eg '1110111111'

; the finite set of states
#Q = {q0,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {q2}

; the number of tapes
#N = 2

; the transition functions

; q0: start
q0 0* _* r* q1
q0 1_ 11 rr q3

; q1: if input is 01*, delete 0 and go to final state
q1 1* ** r* q1
q1 _* _* l* q2

; q2: final state

; q3: copy left number to tape1
q3 1_ 11 rr q3
q3 0_ 0_ rl q4

; q4: if left is bigger than right
q4 11 11 rl q4
q4 1_ 1_ ** q6
q4 _1 ** lr q7
q4 __ __ lr q5

; q5: left == right
q5 0* _* l* q2
q5 1* _* l* q5

; q6: right > left
q6 1* ** rl q6
q6 _* ** l* q12

; q7: left > right
q7 0* _* ll q2
q7 *1 *_ *r q7
q7 *_ *_ *l q8

; q8, q9, q10, q11:
; process when left > right
; tape1(copy of left) - right if tape1 > right update
; left = tape1
q8 *_ *_ *l q8
q8 *1 *1 ** q9

q9 1* 1* l* q9
q9 0* 0* l* q10

q10 11 11 ll q10
q10 1* _* l* q10
q10 _* _* rr q11

q11 _* _* r* q11
q11 11 11 rr q11
q11 0_ 0_ rl q4

; q12, q13, q14: switch between left and right
q12 1* ** l* q12
q12 0* 1* l* q13

q13 1_ ** rr q13
q13 11 01 l* q14

q14 11 1_ lr q14
q14 1* ** l* q14
q14 _* ** r* q0