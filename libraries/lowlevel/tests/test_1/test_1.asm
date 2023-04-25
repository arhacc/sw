

  ;test ignored comment
include "configurations.asm"

func test_asm
label1337:        ;test ignored comment
vload 2222							vload 2222
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop             ;test ignored comment
nop									nop
nop									nop
nop									nop
nop                 addrinc 0
nop								  addrinc 10
nop								  addrinc 213
nop								  addrinc -10
nop								  addrinc -213
nop								  caddrinc
nop								  addrinc_acc
nop								  addr_reg_stack_duplicate
nop								  addr_reg_stack_pop
nop									nop
nop									nop

  ;test ignored comment

sel_addrreg 0						nop
sel_addrreg 1						nop
sel_addrreg 3						nop
addrinc 0							nop
addrinc 10							nop
addrinc 32							nop
addrinc -10							nop
addrinc -32							nop
cc_start_w_halt						nop
cc_start_wo_halt					nop
cc_stop								nop
cc_reset							nop
  ;test ignored comment
nop									vand 0
nop									vand 15
nop									vand 312
nop									and 0
nop									and 15
nop									and 312
nop									rand 0
nop									rand 15
nop									rand 312
nop									riand 0
nop									riand 15
nop									riand 312
nop									cand
nop									sand
nop									crand
vand 0 								nop
vand 15 							nop
vand 312 							nop
and 0 								nop
and 15 								nop
and 312 							nop
rand 0 								nop
rand 15 							nop
rand 312 							nop
riand 0 							nop
riand 15 							nop
riand 312 							nop
cand  								nop
sand  								nop
crand  								nop
nop 								vor 0
nop 								vor 15
nop 								vor 312
nop 								or 0
nop 								or 15
nop 								or 312
nop 								ror 0
nop 								ror 15
nop 								ror 312
nop 								rior 0
nop 								rior 15
nop 								rior 312
nop 								cor
nop 								sor
nop 								cror
vor 0 								nop
vor 15 								nop
vor 312 							nop
or 0 								nop
or 15 								nop
or 312 								nop
ror 0 								nop
ror 15 								nop
ror 312 							nop
rior 0 								nop
rior 15 							nop
rior 312 							nop
cor  								nop
sor  								nop
cror  								nop
nop 								vxor 0
nop 								vxor 15
nop 								vxor 312
nop 								xor 0
nop 								xor 15
nop 								xor 312
nop 								rxor 0
nop 								rxor 15
nop 								rxor 312
nop 								rixor 0
nop 								rixor 15
nop 								rixor 312
nop 								cxor
nop 								sxor
nop 								crxor
vxor 0 								nop
vxor 15 							nop
vxor 312 							nop
xor 0 								nop
xor 15 								nop
xor 312 							nop
rxor 0 								nop
rxor 15 							nop
rxor 312 							nop
rixor 0 							nop
rixor 15 							nop
rixor 312 							nop
cxor  								nop
sxor  								nop
crxor  								nop
nop 								vsub 0
nop 								vsub 15
nop 								vsub 144
nop 								vsub -15
nop 								vsub -144
nop 								sub 0
nop 								sub 15
nop 								sub 144
nop 								rsub 0
nop 								rsub 15
nop 								rsub 144
nop 								risub 0
nop 								risub 15
nop 								risub 144
nop 								csub
nop 								ssub
nop 								crsub
vsub 0 								nop
vsub 15 							nop
vsub 144 							nop
vsub -15 							nop
vsub -144 							nop
sub 0 								nop
sub 15 								nop
sub 144 							nop
rsub 0 								nop
rsub 15 							nop
rsub 144 							nop
risub 0 							nop
risub 15 							nop
risub 144 							nop
csub  								nop
ssub  								nop
crsub  								nop
nop 								vsubc 0
nop 								vsubc 15
nop 								vsubc 144
nop 								vsubc -15
nop 								vsubc -144
nop 								subc 0
nop 								subc 15
nop 								subc 144
nop 								rsubc 0
nop 								rsubc 15
nop 								rsubc 144
nop 								risubc 0
nop 								risubc 15
nop 								risubc 144
nop 								csubc
nop 								ssubc
nop 								crsubc
vsubc 0 							nop
vsubc 15 							nop
vsubc 144 							nop
vsubc -15 							nop
vsubc -144 							nop
subc 0 								nop
subc 15 							nop
subc 144 							nop
rsubc 0 							nop
rsubc 15 							nop
rsubc 144 							nop
risubc 0 							nop
risubc 15 							nop
risubc 144 							nop
csubc  								nop
ssubc  								nop
crsubc  							nop
nop 								vrvsub 0
nop 								vrvsub 15
nop 								vrvsub 144
nop 								vrvsub -15
nop 								vrvsub -144
nop 								rvsub 0
nop 								rvsub 15
nop 								rvsub 144
nop 								rrvsub 0
nop 								rrvsub 15
nop 								rrvsub 144
nop 								rirvsub 0
nop 								rirvsub 15
nop 								rirvsub 144
nop 								crvsub
nop 								srvsub
nop 								crrvsub
vrvsub 0 							nop
vrvsub 15 							nop
vsubc 15 							nop
vsubc 144 							nop
vsubc -15 							nop
vsubc -144 							nop
subc 0 								nop
subc 15 							nop
subc 144 							nop
rsubc 0 							nop
rsubc 15 							nop
rsubc 144 							nop
risubc 0 							nop
risubc 15 							nop
risubc 144 							nop
csubc  								nop
ssubc  								nop
crsubc  							nop
nop 								vrvsubc 0
nop 								vrvsubc 15
nop 								vrvsubc 144
nop 								vrvsubc -15
nop 								vrvsubc -144
nop 								rvsubc 0
nop 								rvsubc 15
nop 								rvsubc 144
nop 								rrvsubc 0
nop 								rrvsubc 15
nop 								rrvsubc 144
nop 								rirvsubc 0
nop 								rirvsubc 15
nop 								rirvsubc 144
nop 								crvsubc
nop 								srvsubc
nop 								crrvsubc
vrvsubc 0 							nop
vrvsubc 15 							nop
vrvsubc 144 						nop
vrvsubc -15 						nop
vrvsubc -144 						nop
rvsubc 0 							nop
rvsubc 15 							nop
rvsubc 144 							nop
rrvsubc 0 							nop
rrvsubc 15 							nop
rrvsubc 144 						nop
rirvsubc 0 							nop
rirvsubc 15 						nop
rirvsubc 144 						nop
crvsubc  							nop
srvsubc  							nop
crrvsubc  							nop
nop 								vcompare 0
nop 								vcompare 15
nop 								vcompare 631
nop 								vcompare -15
nop 								vcompare -631
nop 								compare 0
nop 								compare 15
nop 								compare 631
nop 								rcompare 0
nop 								rcompare 15
nop 								rcompare 631
nop 								ricompare 0
nop 								ricompare 15
nop 								ricompare 631
nop 								ccompare
nop 								scompare
nop 								crcompare
vcompare 0 							nop
vcompare 15 						nop
vcompare 631 						nop
vcompare -15 						nop
vcompare -631 						nop
compare 0 							nop
compare 15 							nop
compare 631 						nop
rcompare 0 							nop
rcompare 15 						nop
rcompare 631 						nop
ricompare 0 						nop
ricompare 15 						nop
ricompare 631 						nop
ccompare  							nop
scompare  							nop
crcompare  							nop
nop 								vadd 0
nop 								vadd 15
nop 								vadd 631
nop 								vadd -15
nop 								vadd -631
nop 								add 0
nop 								add 15
nop 								add 631
nop 								radd 0
nop 								radd 15
nop 								radd 631
nop 								riadd 0
nop 								riadd 15
nop 								riadd 631
nop 								cadd
nop 								sadd
nop 								cradd
vadd 0 								nop
vadd 15 							nop
vadd 631 							nop
vadd -15 							nop
vadd -631 							nop
add 0 								nop
add 15 								nop
add 631 							nop
radd 0 								nop
radd 15 							nop
radd 631 							nop
riadd 0 							nop
riadd 15 							nop
riadd 631 							nop
cadd  								nop
sadd  								nop
cradd  								nop
nop 								vaddc 0
nop 								vaddc 15
nop 								vaddc 631
nop 								vaddc -15
nop 								vaddc -631
nop 								addc 0
nop 								addc 15
nop 								addc 631
nop 								raddc 0
nop 								raddc 15
nop 								raddc 631
nop 								riaddc 0
nop 								riaddc 15
nop 								riaddc 631
nop 								caddc
nop 								saddc
nop 								craddc
vaddc 0 							nop
vaddc 15 							nop
vaddc 631 							nop
vaddc -15 							nop
vaddc -631 							nop
addc 0 								nop
addc 15 							nop
addc 631 							nop
raddc 0 							nop
raddc 15 							nop
raddc 631 							nop
riaddc 0 							nop
riaddc 15 							nop
riaddc 631 							nop
caddc  								nop
saddc  								nop
craddc  							nop
nop 								vmult 0
nop 								vmult 15
nop 								vmult 631
nop 								vmult -15
nop 								vmult -631
nop 								mult 0
nop 								mult 15
nop 								mult 631
nop 								rmult 0
nop 								rmult 15
nop 								rmult 631
nop 								rimult 0
nop 								rimult 15
nop 								rimult 631
nop 								cmult
nop 								smult
nop 								crmult
vmult 0 							nop
vmult 15 							nop
vmult 631 							nop
vmult -15 							nop
vmult -631 							nop
mult 0 								nop
mult 15 							nop
mult 631 							nop
rmult 0 							nop
rmult 15 							nop
rmult 631 							nop
rimult 0 							nop
rimult 15 							nop
rimult 631 							nop
cmult  								nop
smult  								nop
crmult  							nop
nop 								wherezero
nop 								wherepositive
nop 								wherecarry
nop 								wherefirst
nop 								wherenext
nop 								whereprev
nop 								wherenzero
nop 								wherenegative
nop 								wherencarry
nop 								wherenfirst
nop 								wherennext
nop 								wherenprev
nop 								elsewhere
nop 								endwhere
nop 								activate
nop 								scannop
nop 								scanadd
nop 								scanmin
nop 								scanmax
nop 								scansplit 0
nop 								scansplit 1
nop 								scansplit 2
nop 								scansplit 3
nop 								scanprefix_add
nop 								scanprefix_bitwise_xor
nop 								scanpermute 0
nop 								scanpermute 1
nop 								scanpermute 2
nop 								scanpermute 3
nop 								scansub
nop 								scan_bitwise_or
nop 								scan_bitwise_xor
nop 								scan_bitwise_and
nop 								swap_acc_mem 0
nop 								swap_acc_mem 15
nop 								swap_acc_mem 313
nop 								rswap_acc_mem 0
nop 								rswap_acc_mem 15
nop 								rswap_acc_mem 313
nop 								riswap_acc_mem 0
nop 								riswap_acc_mem 15
nop 								riswap_acc_mem 313
nop 								crswap_acc_mem
nop 								vload 0
nop 								vload 15
nop 								vload 313
nop 								load 0
nop 								load 15
nop 								load 313
nop 								rload 0
nop 								rload 15
nop 								rload 313
nop 								riload 0
nop 								riload 15
nop 								riload 313
nop 								cload
nop 								sload
nop 								crload
nop 								vstack_push_load 0
nop 								vstack_push_load 15
nop 								vstack_push_load 313
nop 								vstack_push_load -15
nop 								vstack_push_load -313
nop 								stack_push_load 0
nop 								stack_push_load 15
nop 								stack_push_load 313
nop 								rstack_push_load 0
nop 								rstack_push_load 15
nop 								rstack_push_load 313
nop 								ristack_push_load 0
nop 								ristack_push_load 15
nop 								ristack_push_load 313
nop 								cstack_push_load
nop 								sstack_push_load
nop 								crstack_push_load
nop 								ixload
nop 								srload
nop 								scanload
addrload  							nop
swap_acc_mem 0 						nop
swap_acc_mem 15 					nop
swap_acc_mem 313 					nop
rswap_acc_mem 0 					nop
rswap_acc_mem 15 					nop
rswap_acc_mem 313 					nop
riswap_acc_mem 0 					nop
riswap_acc_mem 15 					nop
riswap_acc_mem 313 					nop
crswap_acc_mem  					nop
vload 0 							nop
vload 15 							nop
vload 313 							nop
load 0 								nop
load 15 							nop
load 313 							nop
rload 0 							nop
rload 15 							nop
rload 313 							nop
riload 0 							nop
riload 15 							nop
riload 313 							nop
sload  								nop
cload  								nop
vstack_push_load 0 					nop
vstack_push_load 15 				nop
vstack_push_load 313 				nop
vstack_push_load -15 				nop
vstack_push_load -313 				nop
stack_push_load 0 					nop
stack_push_load 15 					nop
stack_push_load 313 				nop
rstack_push_load 0 					nop
rstack_push_load 15 				nop
rstack_push_load 313 				nop
ristack_push_load 0 				nop
ristack_push_load 15 				nop
ristack_push_load 313 				nop
cstack_push_load  					nop
sstack_push_load  					nop
crstack_push_load  					nop
nop 								shright
nop 								shrightc
nop 								sharight
nop 								shright_fixed_amount
nop 								sharight_fixed_amount
nop 								rotright_local 0
nop 								rotright_local 1
nop 								rotright_local 2
nop 								rotright_local 3
nop 								rotright_local 5
nop 								rotright_local 8
nop 								rotright_local 14
nop 								rotright_local 31
nop 								rotleft_local 0
nop 								rotleft_local 1
nop 								rotleft_local 2
nop 								rotleft_local 3
nop 								rotleft_local 5
nop 								rotleft_local 8
nop 								rotleft_local 14
nop 								rotleft_local 31
nop 								shift_right 0
nop 								shift_right 1
nop 								shift_right 2
nop 								shift_right 3
nop 								shift_right 5
nop 								shift_right 8
nop 								shift_right 14
nop 								shift_right 31
nop 								shift_left 0
nop 								shift_left 1
nop 								shift_left 2
nop 								shift_left 3
nop 								shift_left 5
nop 								shift_left 8
nop 								shift_left 14
nop 								shift_left 31
shright  							nop
shrightc  							nop
sharight  							nop
shright_fixed_amount  				nop
sharight_fixed_amount  				nop
insval 0 							nop
insval 15 							nop
insval 412 							nop
rotright_local 0 					nop
rotright_local 1 					nop
rotright_local 2 					nop
rotright_local 3 					nop
rotright_local 5 					nop
rotright_local 8 					nop
rotright_local 14 					nop
rotright_local 31 					nop
rotleft_local 0 					nop
rotleft_local 1 					nop
rotleft_local 2 					nop
rotleft_local 3 					nop
rotleft_local 5 					nop
rotleft_local 8 					nop
rotleft_local 14 					nop
rotleft_local 31 					nop
shift_right 0 						nop
shift_right 1 						nop
shift_right 2 						nop
shift_right 3 						nop
shift_right 5 						nop
shift_right 8 						nop
shift_right 14 						nop
shift_right 31 						nop
shift_left 0 						nop
shift_left 1 						nop
shift_left 2 						nop
shift_left 3 						nop
shift_left 5 						nop
shift_left 8 						nop
shift_left 14 						nop
shift_left 31 						nop
nop 								addrstore
nop 								caddrstore
nop 								store 0
nop 								store 15
nop 								store 714
nop 								rstore 0
nop 								rstore 15
nop 								rstore 714
nop 								ristore 0
nop 								ristore 15
nop 								ristore 714
nop 								cstore
nop 								sstore 0
nop 								sstore 15
nop 								sstore 714
nop 								crstore
srstore  							nop
nop 								insertio
nop 								stack_store_pop 0
nop 								stack_store_pop 15
nop 								stack_store_pop 714
nop 								rstack_store_pop 0
nop 								rstack_store_pop 15
nop 								rstack_store_pop 714
nop 								ristack_store_pop 0
nop 								ristack_store_pop 15
nop 								ristack_store_pop 714
nop 								cstack_store_pop
nop 								crstack_store_pop
addrstore  							nop
store 0 							nop
store 15 							nop
store 714 							nop
rstore 0 							nop
rstore 15 							nop
rstore 714 							nop
ristore 0 							nop
ristore 15 							nop
ristore 714 						nop
sstore 0 							nop
sstore 15 							nop
sstore 714 							nop
crstore  							nop
stack_store_pop 0 					nop
stack_store_pop 15 					nop
stack_store_pop 714 				nop
rstack_store_pop 0 					nop
rstack_store_pop 15 				nop
rstack_store_pop 714 				nop
ristack_store_pop 0 				nop
ristack_store_pop 15 				nop
ristack_store_pop 714 				nop
cstack_store_pop  					nop
crstack_store_pop  					nop
setdec  							nop
setval 0 							nop
setval 1 							nop
setval 2 							nop
setval 3 							nop
nop 								fadd 0
nop 								fadd 15
nop 								fadd 673
nop 								fradd 0
nop 								fradd 15
nop 								fradd 673
nop 								fsub 0
nop 								fsub 15
nop 								fsub 673
nop 								frsub 0
nop 								frsub 15
nop 								frsub 673
nop 								fmult 0
nop 								fmult 15
nop 								fmult 673
nop 								frmult 0
nop 								frmult 15
nop 								frmult 673
nop 								frtmult 0
nop 								frtmult 15
nop 								frtmult 673
nop 								rfrtmult 0
nop 								rfrtmult 15
nop 								rfrtmult 673
nop 								scdmult 0
nop 								scdmult 15
nop 								scdmult 673
nop 								rscdmult 0
nop 								rscdmult 15
nop 								rscdmult 673
nop 								fdiv 0
nop 								fdiv 15
nop 								fdiv 673
nop 								frdiv 0
nop 								frdiv 15
nop 								frdiv 673
nop 								loop1 0
nop 								loop1 15
nop 								loop1 673
nop 								rloop1 0
nop 								rloop1 15
nop 								rloop1 673
nop 								loop2 0
nop 								loop2 15
nop 								loop2 673
nop 								rloop2 0
nop 								rloop2 15
nop 								rloop2 673
nop 								float 0
nop 								float 1
nop 								float 2
nop 								float 3
nop 								float 4
nop 								float 5
fadd 0 								nop
fadd 15 							nop
fadd 673 							nop
fradd 0 							nop
fradd 15 							nop
fradd 673 							nop
fsub 0 								nop
fsub 15 							nop
fsub 673 							nop
frsub 0 							nop
frsub 15 							nop
frsub 673 							nop
fmult 0 							nop
fmult 15 							nop
fmult 673 							nop
frmult 0 							nop
frmult 15 							nop
frmult 673 							nop
frtmult 0 							nop
frtmult 15 							nop
frtmult 673 						nop
rfrtmult 0 							nop
rfrtmult 15 						nop
rfrtmult 673 						nop
scdmult 0 							nop
scdmult 15 							nop
scdmult 673 						nop
rscdmult 0 							nop
rscdmult 15 						nop
rscdmult 673 						nop
fdiv 0 								nop
fdiv 15 							nop
fdiv 673 							nop
frdiv 0 							nop
frdiv 15 							nop
frdiv 673 						nop
loop1 0 							nop
loop1 15 							nop
loop1 673 							nop
rloop1 0 							nop
rloop1 15 							nop
rloop1 673 							nop
loop2 0 							nop
loop2 15 							nop
loop2 673 							nop
rloop2 0 							nop
rloop2 15 							nop
rloop2 673 							nop
float 0 							nop
float 1 							nop
float 2 							nop
float 3 							nop
float 4 							nop
float 5 							nop
send 0 								nop
send 15 							nop
send 673 							nop
rsend 0 							nop
rsend 15 							nop
rsend 673 							nop
risend 0 							nop
risend 15 							nop
risend 673 							nop
csend 0 							nop
csend 15 							nop
csend 673 							nop
ssend 0 							nop
ssend 15 							nop
ssend 673 							nop
crsend 0 							nop
crsend 15 							nop
crsend 673 							nop
nop 								srcall
nop 								vsrcall 0
nop 								vsrcall 15
nop 								vsrcall 631
nop 								vsrcall -15
nop 								vsrcall -631
nop 								search
nop 								vsearch 0
nop 								vsearch 15
nop 								vsearch 631
nop 								vsearch -15
nop 								vsearch -631
nop 								csearch
nop 								vcsearch 0
nop 								vcsearch 15
nop 								vcsearch 631
nop 								vcsearch -15
nop 								vcsearch -631
nop 								selshift
nop 								delete
nop 								stack_pop
nop 								stack_duplicate
nop 								stack_over
nop 								stack_swap
nop 								stack_load_layer1
stack_pop  							nop
stack_duplicate  					nop
stack_over  						nop
stack_swap  						nop
stack_load_layer1  					nop
jmp label1337 						nop
jmp label1338 						nop
brz label1337 						nop
brz label1338 						nop
brnz label1337 						nop
brnz label1338 						nop
brsgn label1337 					nop
brsgn label1338 					nop
brnsgn label1337 					nop
brnsgn label1338 					nop
brzdec label1337 					nop
brzdec label1338 					nop
brnzdec label1337 					nop
brnzdec label1338 					nop
brbool label1337 					nop
brbool label1338 					nop
brnbool label1337 					nop
brnbool label1338 					nop
brcr label1337 						nop
brcr label1338 						nop
brncr label1337 					nop
brncr label1338 					nop
brcr_delayed label1337 				nop
brcr_delayed label1338 				nop
brncr_delayed label1337 			nop
brncr_delayed label1338 			nop
brvalz label1337, 0 					nop
brvalz label1338, 0 					nop
brvalnz label1337, 0 				nop
brvalnz label1338, 0 				nop
brvalsgn label1337, 0 				nop
brvalsgn label1338, 0 				nop
brvalnsgn label1337, 0 				nop
brvalnsgn label1338, 0 				nop
brvalzdec label1337, 0 				nop
brvalzdec label1338, 0 				nop
brvalnzdec label1337, 0 				nop
brvalnzdec label1338, 0 				nop
brcmpval label1337, 0 				nop
brcmpval label1338, 0 				nop
brcmpnval label1337, 0 				nop
brcmpnval label1338, 0 				nop
brcmpvaldec label1337, 0 			nop
brcmpvaldec label1338, 0 			nop
brcmpnvaldec label1337, 0 			nop
brcmpnvaldec label1338, 0 			nop
brvalz label1337, 1 					nop
brvalz label1338, 1 					nop
brvalnz label1337, 1 				nop
brvalnz label1338, 1 				nop
brvalsgn label1337, 1 				nop
brvalsgn label1338, 1 				nop
brvalnsgn label1337, 1 				nop
brvalnsgn label1338, 1 				nop
brvalzdec label1337, 1 				nop
brvalzdec label1338, 1 				nop
brvalnzdec label1337, 1 				nop
brvalnzdec label1338, 1 				nop
brcmpval label1337, 1 				nop
brcmpval label1338, 1 				nop
brcmpnval label1337, 1 				nop
brcmpnval label1338, 1 				nop
brcmpvaldec label1337, 1 			nop
brcmpvaldec label1338, 1 			nop
brcmpnvaldec label1337, 1 			nop
brcmpnvaldec label1338, 1 			nop
brvalz label1337, 2 					nop
brvalz label1338, 2 					nop
brvalnz label1337, 2 				nop
brvalnz label1338, 2 				nop
brvalsgn label1337, 2 				nop
brvalsgn label1338, 2 				nop
brvalnsgn label1337, 2 				nop
brvalnsgn label1338, 2 				nop
brvalzdec label1337, 2 				nop
brvalzdec label1338, 2 				nop
brvalnzdec label1337, 2 				nop
brvalnzdec label1338, 2 				nop
brcmpval label1337, 2 				nop
brcmpval label1338, 2 				nop
brcmpnval label1337, 2 				nop
brcmpnval label1338, 2 				nop
brcmpvaldec label1337, 2 			nop
brcmpvaldec label1338, 2 			nop
brcmpnvaldec label1337, 2 			nop
brcmpnvaldec label1338, 2 			nop
brvalz label1337, 3 					nop
brvalz label1338, 3 					nop
brvalnz label1337, 3 				nop
brvalnz label1338, 3 				nop
brvalsgn label1337, 3 				nop
brvalsgn label1338, 3 				nop
brvalnsgn label1337, 3 				nop
brvalnsgn label1338, 3 				nop
brvalzdec label1337, 3 				nop
brvalzdec label1338, 3 				nop
brvalnzdec label1337, 3 				nop
brvalnzdec label1338, 3 				nop
brcmpval label1337, 3 				nop
brcmpval label1338, 3 				nop
brcmpnval label1337, 3 				nop
brcmpnval label1338, 3 				nop
brcmpvaldec label1337, 3 			nop
brcmpvaldec label1338, 3 			nop
brcmpnvaldec label1337, 3 			nop
brcmpnvaldec label1338, 3 			nop
halt  								nop
grshift_wob  						nop
grshift_wbz  						nop
grshift_wbh  						nop
vleftins 0 							nop
vleftins 15 						nop
vleftins 623 						nop
crightins  							nop
cleftins  							nop
right_redins  						nop
left_redins  						nop
glshift_wob  						nop
glshift_wbz  						nop
glshift_wbh  						nop
rotateright  						nop
rotateleft  						nop
param  								nop
setint  							nop
waitmatw 0 							nop
waitmatw 1 							nop
waitmatw 2 							nop
waitmatw 3 							nop
waitmatw 8 							nop
waitmatw 15 						nop
resready  							nop

nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
nop									nop
vload 2223							vload 2223
label1338:
endfunc
