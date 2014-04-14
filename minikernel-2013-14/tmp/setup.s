# 1 "boot/setup.S"
# 1 "<command-line>"
# 1 "boot/setup.S"
!
!	setup.S		Copyright (C) 1991, 1992 Linus Torvalds
!
! setup.s is responsible for getting the system data from the BIOS,
! and putting them into the appropriate places in system memory.
! both setup.s and system has been loaded by the bootblock.
!
! This code asks the bios for memory/disk/other parameters, and
! puts them in a "safe" place: 0x90000-0x901FF, ie where the
! boot-block used to be. It is then up to the protected mode
! system to read them from there before the area is overwritten
! for buffer-blocks.
!
! Move PS/2 aux init code to psaux.c
! (troyer@saifr00.cfsat.Honeywell.COM) 03Oct92
!
! some changes and additional features by Christoph Niemann,
! March 1993/June 1994 (Christoph.Niemann@1.org)
!
! add APM BIOS checking by Stephen Rothwell, May 1994
! (Stephen.Rothwell@canb.auug.org.au)
!
! High load stuff, initrd support and position independency
! by Hans Lermen & Werner Almesberger, February 1996
! <lermen@elserv.ffm.fgan.de>, <almesber@lrc.epfl.ch>
!
! Video handling moved to video.S by Martin Mares, March 1996
! <mj@k332.feld.cvut.cz>
!
! Extended memory detection scheme retwiddled by orc@pell.chi.il.us (david
! parsons) to avoid loadlin confusion, July 1997
!


# 1 "include/config.h" 1



















# 35 "boot/setup.S" 2

! Signature words to ensure LILO loaded us right



INITSEG  = 0x9000	! 0x9000, we move boot here - out of the way
SYSSEG   = 0x1000	! 0x1000, system loaded at 0x10000 (65536).
SETUPSEG = 0x9020	! 0x9020, this is the current segment
			! ... and the former contents of CS
DELTA_INITSEG = SETUPSEG - INITSEG ! 0x0020

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

entry start
start:
	jmp	start_of_setup
! ------------------------ start of header --------------------------------
!
! SETUP-header, must start at CS:2 (old 0x9020:2)
!
		.ascii	"HdrS"		! Signature for SETUP-header
		.word	0x0201		! Version number of header format
					! (must be >= 0x0105
					! else old loadlin-1.5 will fail)
realmode_swtch:	.word	0,0		! default_switch,SETUPSEG
start_sys_seg:	.word	SYSSEG
		.word	kernel_version	! pointing to kernel version string
  ! note: above part of header is compatible with loadlin-1.5 (header v1.5),
  !        must not change it

type_of_loader:	.byte	0		! = 0, old one (LILO, Loadlin,
					!      Bootlin, SYSLX, bootsect...)
					! else it is set by the loader:
					! 0xTV: T=0 for LILO
					!	T=1 for Loadlin
					!	T=2 for bootsect-loader
					!	T=3 for SYSLX
					!	T=4 for ETHERBOOT
					!       V = version
loadflags:			! flags, unused bits must be zero (RFU)
LOADED_HIGH	= 1		! bit within loadflags,
				! if set, then the kernel is loaded high
CAN_USE_HEAP	= 0x80		! if set, the loader also has set heap_end_ptr
				! to tell how much space behind setup.S
				! can be used for heap purposes.
				! Only the loader knows what is free

		.byte	0x00




setup_move_size: .word  0x8000	! size to move, when we (setup) are not
				! loaded at 0x90000. We will move ourselves
				! to 0x90000 then just before jumping into
				! the kernel. However, only the loader
				! know how much of data behind us also needs
				! to be loaded.
code32_start:				! here loaders can put a different
					! start address for 32-bit code.

		.long	0x1000		!   0x1000 = default for zImage



ramdisk_image:	.long	0	! address of loaded ramdisk image
				! Here the loader (or kernel generator) puts
				! the 32-bit address were it loaded the image.
				! This only will be interpreted by the kernel.
ramdisk_size:	.long	0	! its size in bytes
bootsect_kludge:
		.word   bootsect_helper,SETUPSEG
heap_end_ptr:	.word	modelist+1024	! space from here (exclusive) down to
				! end of setup code can be used by setup
				! for local heap purposes.
! ------------------------ end of header ----------------------------------

start_of_setup:
! Bootlin depends on this being done early
	mov	ax,#0x01500
	mov	dl,#0x81
	int	0x13








! set DS=CS, we know that SETUPSEG == CS at this point
	mov	ax,cs		! aka #SETUPSEG
	mov	ds,ax

! Check signature at end of setup
	cmp	setup_sig1,#0xAA55
	jne	bad_sig
	cmp	setup_sig2,#0x5A5A
	jne	bad_sig
	jmp	good_sig1

! Routine to print ASCIIz string at DS:SI

prtstr:	lodsb
	and	al,al
	jz	fin
	call	prtchr
	jmp	prtstr
fin:	ret

! Space printing

prtsp2:	call	prtspc		! Print double space
prtspc:	mov	al,#0x20	! Print single space (fall-thru!)

! Part of above routine, this one just prints ASCII al

prtchr:	push	ax
	push	cx
	xor	bh,bh
	mov	cx,#0x01
	mov	ah,#0x0e
	int	0x10
	pop	cx
	pop	ax
	ret

beep:	mov	al,#0x07
	jmp	prtchr
	
no_sig_mess:	.ascii	"No setup signature found ..."
		db	0x00

good_sig1:
	jmp	good_sig

! We now have to find the rest of the setup code/data
bad_sig:
	mov	ax,cs		! aka #SETUPSEG
	sub	ax,#DELTA_INITSEG ! aka #INITSEG
	mov	ds,ax
	xor	bh,bh
	mov	bl,[497]	! get setup sects from boot sector
	sub	bx,#4		! LILO loads 4 sectors of setup
	shl	bx,#8		! convert to words
	mov	cx,bx
	shr	bx,#3		! convert to segment
	add	bx,#SYSSEG
	seg cs
	mov	start_sys_seg,bx

! Move rest of setup code/data to here
	mov	di,#2048	! four sectors loaded by LILO
	sub	si,si
	mov	ax,cs		! aka #SETUPSEG
	mov	es,ax
	mov	ax,#SYSSEG
	mov	ds,ax
	rep
	movsw

	mov	ax,cs		! aka #SETUPSEG
	mov	ds,ax
	cmp	setup_sig1,#0xAA55
	jne	no_sig
	cmp	setup_sig2,#0x5A5A
	jne	no_sig
	jmp	good_sig

no_sig:
	lea	si,no_sig_mess
	call	prtstr
no_sig_loop:
	jmp	no_sig_loop

good_sig:
	mov	ax,cs		! aka #SETUPSEG
	sub	ax,#DELTA_INITSEG ! aka #INITSEG
	mov	ds,ax

! check if an old loader tries to load a big-kernel
	seg cs
	test	byte ptr loadflags,#LOADED_HIGH ! Have we a big kernel?
	jz	loader_ok	! NO, no danger even for old loaders
				! YES, we have a big-kernel
	seg cs
	cmp	byte ptr type_of_loader,#0 ! Have we one of the new loaders?
	jnz	loader_ok	! YES, OK
				! NO, we have an old loader, must give up
	push    cs
	pop	ds
	lea	si,loader_panic_mess
	call	prtstr
	jmp	no_sig_loop
loader_panic_mess: 
	.ascii	"Wrong loader:  giving up."
	db	0

loader_ok:
! Get memory size (extended mem, kB)


	push	ebx

        xor     ebx,ebx		! preload new memory slot with 0k
        mov	[0x1e0], ebx

        mov     ax,#0xe801
	int     0x15
	jc      oldstylemem

! Memory size is in 1 k chunksizes, to avoid confusing loadlin.
! We store the 0xe801 memory size in a completely different place,
! because it will most likely be longer than 16 bits.
! (use 1e0 because that's what Larry Augustine uses in his
! alternative new memory detection scheme, and it's sensible
! to write everything into the same place.)

	and     ebx, #0xffff    ! clear sign extend
	shl     ebx, 6          ! and go from 64k to 1k chunks
	mov     [0x1e0],ebx     ! store extended memory size

	and     eax, #0xffff    ! clear sign extend
 	add     [0x1e0],eax     ! and add lower memory into total size.
  
	! and fall into the old memory detection code to populate the
	! compatibility slot.

oldstylemem:
	pop	ebx



	mov	ah,#0x88
	int	0x15
	mov	[2],ax

! Set the keyboard repeat rate to the max

	mov	ax,#0x0305
	xor	bx,bx		! clear bx
	int	0x16

! Check for video adapter and its parameters and allow the
! user to browse video modes.

	call	video	! NOTE: we need DS pointing to boot sector

! Get hd0 data

	xor	ax,ax		! clear ax
	mov	ds,ax
	lds	si,[4*0x41]
	mov	ax,cs		! aka #SETUPSEG
	sub	ax,#DELTA_INITSEG ! aka #INITSEG
	push	ax
	mov	es,ax
	mov	di,#0x0080
	mov	cx,#0x10
	push	cx
	cld
	rep
	movsb

! Get hd1 data

	xor	ax,ax		! clear ax
	mov	ds,ax
	lds	si,[4*0x46]
	pop	cx
	pop	es
	mov	di,#0x0090
	rep
	movsb

! Check that there IS a hd1 :-)

	mov	ax,#0x01500
	mov	dl,#0x81
	int	0x13
	jc	no_disk1
	cmp	ah,#3
	je	is_disk1
no_disk1:
	mov	ax,cs		! aka #SETUPSEG
	sub	ax,#DELTA_INITSEG ! aka #INITSEG
	mov	es,ax
	mov	di,#0x0090
	mov	cx,#0x10
	xor	ax,ax		! clear ax
	cld
	rep
	stosb
is_disk1:

! check for Micro Channel (MCA) bus
	mov ax,cs		! aka #SETUPSEG
	sub ax,#DELTA_INITSEG	! aka #INITSEG
	mov ds,ax
	mov ds,ax
	xor ax,ax
	mov [0xa0], ax		! set table length to 0
	mov ah, #0xc0
	stc
	int 0x15		! puts feature table at es:bx
	jc no_mca
	push ds
	mov ax,es
	mov ds,ax
	mov ax,cs		! aka #SETUPSEG
	sub ax, #DELTA_INITSEG	! aka #INITSEG
	mov es,ax
	mov si,bx
	mov di,#0xa0
	mov cx,(si)
	add cx,#2		! table length is a short
	cmp cx,#0x10
	jc sysdesc_ok
	mov cx,#0x10		! we keep only first 16 bytes
sysdesc_ok:
	rep
	movsb
	pop ds

no_mca:

! Check for PS/2 pointing device

	mov	ax,cs		! aka #SETUPSEG
	sub	ax,#DELTA_INITSEG ! aka #INITSEG
	mov	ds,ax
	mov	[0x1ff],#0	! default is no pointing device
	int	0x11		! int 0x11: equipment determination
	test	al,#0x04	! check if pointing device installed
	jz	no_psmouse
	mov	[0x1ff],#0xaa	! device present
no_psmouse:

# 439 "boot/setup.S"

! Now we want to move to protected mode ...

	seg cs
	cmp	realmode_swtch,#0
	jz	rmodeswtch_normal
	seg cs
	callf	far * realmode_swtch
	jmp	rmodeswtch_end
rmodeswtch_normal:
        push	cs
	call	default_switch
rmodeswtch_end:

! we get the code32 start address and modify the below 'jmpi'
! (loader may have changed it)
	seg cs
	mov	eax,code32_start
	seg cs
	mov	code32,eax

! Now we move the system to its rightful place
! ...but we check, if we have a big-kernel.
! in this case we *must* not move it ...
	seg cs
	test	byte ptr loadflags,#LOADED_HIGH
	jz	do_move0	! we have a normal low loaded zImage
				! we have a high loaded big kernel
	jmp	end_move	! ... and we skip moving

do_move0:
	mov	ax,#0x100	! start of destination segment
	mov	bp,cs		! aka #SETUPSEG
	sub	bp,#DELTA_INITSEG ! aka #INITSEG
	seg cs
	mov	bx,start_sys_seg	! start of source segment
	cld			! 'direction'=0, movs moves forward
do_move:
	mov	es,ax		! destination segment
	inc	ah		! instead of add ax,#0x100
	mov	ds,bx		! source segment
	add	bx,#0x100
	sub	di,di
	sub	si,si
	mov 	cx,#0x800
	rep
	movsw
	cmp	bx,bp		! we assume start_sys_seg > 0x200,
				! so we will perhaps read one page more then
				! needed, but never overwrite INITSEG because
				! destination is minimum one page below source
	jb	do_move

! then we load the segment descriptors

end_move:
	mov	ax,cs ! aka #SETUPSEG	! right, forgot this at first. didn't work :-)
	mov	ds,ax

! If we have our code not at 0x90000, we need to move it there now.
! We also then need to move the parameters behind it (command line)
! Because we would overwrite the code on the current IP, we move
! it in two steps, jumping high after the first one.
	mov	ax,cs
	cmp	ax,#SETUPSEG
	je	end_move_self
	cli	! make sure we really have interrupts disabled !
		! because after this the stack should not be used
	sub	ax,#DELTA_INITSEG ! aka #INITSEG
	mov	dx,ss
	cmp	dx,ax
	jb	move_self_1
	add	dx,#INITSEG
	sub	dx,ax		! this will be SS after the move
move_self_1:
	mov	ds,ax
	mov	ax,#INITSEG	! real INITSEG
	mov	es,ax
	seg cs
	mov	cx,setup_move_size
	std		! we have to move up, so we use direction down
			! because the areas may overlap
	mov	di,cx
	dec	di
	mov	si,di
	sub	cx,#move_self_here+0x200
	rep
	movsb
	jmpi	move_self_here,SETUPSEG ! jump to our final place
move_self_here:
	mov	cx,#move_self_here+0x200
	rep
	movsb
	mov	ax,#SETUPSEG
	mov	ds,ax
	mov	ss,dx
			! now we are at the right place
end_move_self:

	lidt	idt_48		! load idt with 0,0
	lgdt	gdt_48		! load gdt with whatever appropriate

! that was painless, now we enable A20

	call	empty_8042
	mov	al,#0xD1		! command write
	out	#0x64,al
	call	empty_8042
	mov	al,#0xDF		! A20 on
	out	#0x60,al
	call	empty_8042

! wait until a20 really *is* enabled; it can take a fair amount of
! time on certain systems; Toshiba Tecras are known to have this
! problem.  The memory location used here is the int 0x1f vector,
! which should be safe to use; any *unused* memory location < 0xfff0
! should work here.  



	push	ds
	xor	ax,ax			! segment 0x0000
	mov	ds,ax
	dec	ax			! segment 0xffff (HMA)
	mov	gs,ax
	mov	bx,[0x7c]		! we want to restore the value later
a20_wait:
	inc	ax
	mov	[0x7c],ax
	seg	gs
	cmp	ax,[0x7c+0x10]
	je	a20_wait		! loop until no longer aliased
	mov	[0x7c],bx		! restore original value
	pop	ds
		
! make sure any possible coprocessor is properly reset..

	xor	ax,ax
	out	#0xf0,al
	call	delay
	out	#0xf1,al
	call	delay

! well, that went ok, I hope. Now we have to reprogram the interrupts :-(
! we put them right after the intel-reserved hardware interrupts, at
! int 0x20-0x2F. There they won't mess up anything. Sadly IBM really
! messed this up with the original PC, and they haven't been able to
! rectify it afterwards. Thus the bios puts interrupts at 0x08-0x0f,
! which is used for the internal hardware interrupts as well. We just
! have to reprogram the 8259's, and it isn't fun.

	mov	al,#0x11		! initialization sequence
	out	#0x20,al		! send it to 8259A-1
	call	delay
	out	#0xA0,al		! and to 8259A-2
	call	delay
	mov	al,#0x20		! start of hardware int's (0x20)
	out	#0x21,al
	call	delay
	mov	al,#0x28		! start of hardware int's 2 (0x28)
	out	#0xA1,al
	call	delay
	mov	al,#0x04		! 8259-1 is master
	out	#0x21,al
	call	delay
	mov	al,#0x02		! 8259-2 is slave
	out	#0xA1,al
	call	delay
	mov	al,#0x01		! 8086 mode for both
	out	#0x21,al
	call	delay
	out	#0xA1,al
	call	delay
	mov	al,#0xFF		! mask off all interrupts for now
	out	#0xA1,al
	call	delay
	mov	al,#0xFB		! mask all irq's but irq2 which
	out	#0x21,al		! is cascaded

! Well, that certainly wasn't fun :-(. Hopefully it works, and we don't
! need no steenking BIOS anyway (except for the initial loading :-).
! The BIOS routine wants lots of unnecessary data, and it's less
! "interesting" anyway. This is how REAL programmers do it.
!
! Well, now's the time to actually move into protected mode. To make
! things as simple as possible, we do no register set-up or anything,
! we let the GNU-compiled 32-bit programs do that. We just jump to
! absolute address 0x1000 (or the loader supplied one),
! in 32-bit protected mode.
!
! Note that the short jump isn't strictly needed, although there are
! reasons why it might be a good idea. It won't hurt in any case.
!
	mov	ax,#1		! protected mode (PE) bit
	lmsw	ax		! This is it!
	jmp	flush_instr
flush_instr:
	xor	bx,bx		! Flag to indicate a boot

! NOTE: For high loaded big kernels we need a
!	jmpi    0x100000,0x10
!
!	but we yet haven't reloaded the CS register, so the default size 
!	of the target offset still is 16 bit.
!       However, using an operant prefix (0x66), the CPU will properly
!	take our 48 bit far pointer. (INTeL 80386 Programmer's Reference
!	Manual, Mixing 16-bit and 32-bit code, page 16-6)
	db	0x66,0xea	! prefix + jmpi-opcode
code32:	dd	0x1000		! will be set to 0x100000 for big kernels
	dw	0x10


kernel_version:	.ascii	"mini kernel for IAR course"
		db	0

! This is the default real mode switch routine.
! to be called just before protected mode transition

default_switch:
	cli			! no interrupts allowed !
	mov	al,#0x80	! disable NMI for the bootup sequence
	out	#0x70,al
	retf

! This routine only gets called, if we get loaded by the simple
! bootsect loader _and_ have a bzImage to load.
! Because there is no place left in the 512 bytes of the boot sector,
! we must emigrate to code space here.
!
bootsect_helper:
	seg cs
	cmp	word ptr bootsect_es,#0
	jnz	bootsect_second
	seg cs
	mov	byte ptr type_of_loader,#0x20
	mov	ax,es
	shr	ax,#4
	seg	cs
	mov	byte ptr bootsect_src_base+2,ah
	mov	ax,es
	seg cs
	mov	bootsect_es,ax
	sub	ax,#SYSSEG
	retf			! nothing else to do for now
bootsect_second:
	push	cx
	push	si
	push	bx
	test	bx,bx	! 64K full ?
	jne	bootsect_ex
	mov	cx,#0x8000	! full 64K move, INT15 moves words
	push	cs
	pop	es
	mov	si,#bootsect_gdt
	mov	ax,#0x8700
	int	0x15
	jc	bootsect_panic	! this, if INT15 fails
	seg cs
	mov	es,bootsect_es	! we reset es to always point to 0x10000
	seg cs
	inc	byte ptr bootsect_dst_base+2
bootsect_ex:
	seg cs
	mov	ah, byte ptr bootsect_dst_base+2
	shl	ah,4	! we now have the number of moved frames in ax
	xor	al,al
	pop	bx
	pop	si
	pop	cx
	retf

bootsect_gdt:
	.word	0,0,0,0
	.word	0,0,0,0
bootsect_src:
	.word	0xffff
bootsect_src_base:
	.byte	0,0,1			! base = 0x010000
	.byte	0x93			! typbyte
	.word	0			! limit16,base24 =0
bootsect_dst:
	.word	0xffff
bootsect_dst_base:
	.byte	0,0,0x10		! base = 0x100000
	.byte	0x93			! typbyte
	.word	0			! limit16,base24 =0
	.word	0,0,0,0			! BIOS CS
	.word	0,0,0,0			! BIOS DS
bootsect_es:
	.word	0

bootsect_panic:
	push	cs
	pop	ds
	cld
	lea	si,bootsect_panic_mess
	call	prtstr
bootsect_panic_loop:
	jmp	bootsect_panic_loop
bootsect_panic_mess:
	.ascii	"INT15 refuses to access high memory.  Giving up."
	db	0

! This routine checks that the keyboard command queue is empty
! (after emptying the output buffers)
!
! Some machines have delusions that the keyboard buffer is always full
! with no keyboard attached...

empty_8042:
       push    cx
       mov     cx,#0xFFFF

empty_8042_loop:
       dec     cx
       jz      empty_8042_end_loop

	call	delay
	in	al,#0x64	! 8042 status port
	test	al,#1		! output buffer?
	jz	no_output
	call	delay
	in	al,#0x60	! read it
	jmp	empty_8042_loop
no_output:
	test	al,#2		! is input buffer full?
	jnz	empty_8042_loop	! yes - loop
empty_8042_end_loop:
        pop     cx
	ret

!
! Read the CMOS clock. Return the seconds in al
!
gettime:
	push	cx
	mov	ah,#0x02
	int	0x1a
	mov	al,dh			! dh contains the seconds
	and	al,#0x0f
	mov	ah,dh
	mov	cl,#0x04
	shr	ah,cl
	aad
	pop	cx
	ret

!
! Delay is needed after doing I/O
!
delay:
	.word	0x00eb			! jmp $+2
	ret

!
! Descriptor tables
!

gdt:
	.word	0,0,0,0		! dummy

	.word	0,0,0,0		! unused

	.word	0xFFFF		! 4Gb - (0x100000*0x1000 = 4Gb)
	.word	0x0000		! base address=0
	.word	0x9A00		! code read/exec
	.word	0x00CF		! granularity=4096, 386 (+5th nibble of limit)

	.word	0xFFFF		! 4Gb - (0x100000*0x1000 = 4Gb)
	.word	0x0000		! base address=0
	.word	0x9200		! data read/write
	.word	0x00CF		! granularity=4096, 386 (+5th nibble of limit)

idt_48:
	.word	0			! idt limit=0
	.word	0,0			! idt base=0L

gdt_48:
	.word	0x800		! gdt limit=2048, 256 GDT entries
	.word	512+gdt,0x9	! gdt base = 0X9xxxx

!
! Include video setup & detection code
!


# 1 "boot/video.S" 1
!
!	Display adapter & video mode setup, version 2.12 (25-May-98)
!
!	Copyright (C) 1995 -- 1998 Martin Mares <mj@atrey.karlin.mff.cuni.cz>
!	Based on the original setup.S code (C) Linus Torvalds and Mats Anderson
!

! Enable autodetection of SVGA adapters and modes. If you really need this
! feature, drop me a mail as I think of removing it some day...


! Enable autodetection of VESA modes


! Enable compacting of mode table


! Retain screen contents when switching modes


! Enable local mode list


! Force 400 scan lines for standard modes (hack to fix bad behaviour
! of certain broken BIOSes -- don't use unless needed)


! A special hack allowing to force specific BIOS mode ID along with specific
! dimensions. Especially useful for certain X-Window graphics mode hacks
! (e.g., 800x600 modes on IBM ThinkPad).





! This code uses an extended set of video mode numbers. These include:
! Aliases for standard modes
!	NORMAL_VGA (-1)
!	EXTENDED_VGA (-2)
!	0xfffd (-3)
! Video modes numbered by menu position -- NOT RECOMMENDED because of lack
! of compatibility when extending the table. These are between 0x00 and 0xff.

! Standard BIOS video modes (BIOS number + 0x0100)

! VESA BIOS video modes (VESA number + 0x0200)

! Video7 special modes (BIOS number + 0x0900)

! Special video modes
# 62 "boot/video.S"
! Video modes given by resolution


! The "recalculate timings" flag


! Positions of various video parameters passed to the kernel
! (see also include/1/tty.h)
# 78 "boot/video.S"

# 89 "boot/video.S"

! Define DO_STORE according to 






!
! This is the main entry point called by setup.S
!
! Input:
!	DS pointing to the boot sector

video:	push	ds		! We use different segments
	push	ds		! FS contains original DS
	pop	fs
	push	cs		! DS is equal to CS
	pop	ds
	push	cs		! ES is equal to CS
	pop	es
	xor	ax,ax
	mov	gs,ax		! GS is zero
	cld
	call	basic_detect	! Basic adapter type testing (EGA/VGA/MDA/CGA)
# 129 "boot/video.S"
	call	mode_params	! Store mode parameters
	pop	ds		! Restore original DS
	ret

!
! Detect if we have CGA, MDA, EGA or VGA and pass it to the kernel.
!

basic_detect:
	seg	fs		! Default is no VGA
	movb	[0x0f],#0

	mov	ah,#0x12	! Check EGA/VGA
	mov	bl,#0x10
	int	0x10
	seg	fs
	mov	[0x0a],bx	! Used for identification of EGA in the kernel
	cmp	bl,#0x10	! No, this is a CGA/MDA/HGA card.
	je	basret
	incb	[adapter]

	mov	ax,#0x1a00	! Check for EGA/VGA discrimination
	int	0x10
	cmp	al,#0x1a	! 1a means VGA, anything else EGA
	jne	basret
	seg	fs
	incb	[0x0f]	! We've detected a VGA
	incb	[adapter]

basret:	ret

!
! Store the video mode parameters for later usage by the kernel.
! This is done by asking the BIOS except for the rows/columns
! parameters in the default 80x25 mode -- these are set directly,
! because some very obscure BIOSes supply insane values.
!

mode_params:





	mov	ah,#0x03	! Read cursor position
	xor	bh,bh
	int	0x10
	seg	fs
	mov	[0x00],dx

	mov	ah,#0x0f	! Read page/mode/width
	int	0x10
	seg	fs
	mov	[0x04],bx
	seg	fs
	mov	[0x06],ax	! Video mode and screen width
	cmp	al,#7			! MDA/HGA => segment differs
	jnz	mopar0
	mov	[video_segment],#0xb000
mopar0:	seg	gs			! Font size
	mov	ax,[0x485]
	seg	fs
	mov	[0x10],ax	! (valid only on EGA/VGA)

	mov	ax,[force_size]	! Forced size?
	or	ax,ax
	jz	mopar1
	seg	fs
	mov	[0x07],ah
	seg	fs
	mov	[0x0e],al
	ret

mopar1:	mov	al,#25
	cmpb	[adapter],#0	! If we are on CGA/MDA/HGA, the screen must
	jz	mopar2		! have 25 lines.
	seg	gs		! On EGA/VGA, use the EGA+ BIOS variable
	mov	al,[0x484]	! containing maximal line number.
	inc	al
mopar2:	seg	fs
	movb	[0x0e],al
	ret

# 1955 "boot/video.S"

!
! Other variables:
!

adapter:	.byte	0	! Video adapter: 0=CGA/MDA/HGA,1=EGA,2=VGA
video_segment:	.word	0xb800	! Video memory segment
force_size:	.word	0	! Use this size instead of the one in BIOS vars
# 825 "boot/setup.S" 2

!
! Setup signature -- must be last
!

setup_sig1:	.word	0xAA55
setup_sig2:	.word	0x5A5A

!
! After this point, there is some free space which is used by the video mode
! handling code to store the temporary mode table (not used by the kernel).
!

modelist:

.text
endtext:
.data
enddata:
.bss
endbss: