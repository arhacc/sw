





sudo apt-get install tofrodos gawk xvfb git libncurses5-dev tftpd zlib1g-dev zlib1g-dev:i386 libssl-dev flex bison chrpath socat autoconf libtool texinfo gcc-multilib libsdl1.2-dev libglib2.0-dev screen pax


sudo apt-get install libyaml-0-2 && libyaml-appconfig-perl && libyaml-cpp0.3v5 && libyaml-cpp-dev && libyaml-doc && libyaml-perl && libyaml-snake-java && libyaml-syck-perl && libyaml-0-2-dbg && libyaml-cpp0.3-dev && libyaml-cpp0.5v5 && libyaml-dev && libyaml-libyaml-perl && libyaml-shell-perl && libyaml-snake-java-doc && libyaml-tiny-perl 





----------- for zynq 7000 devices. zynq ultrascale may be a little different----------

daca proiectul este deja generat si configurat, se pot da doar build si package 



sudo tc qdisc add dev wlp2s0 root netem delay 250ms && sudo tc qdisc add dev enp3s0 root netem delay 250ms && unset HTTPS_PROXY && unset https_proxy && unset HTTP_PROXY && unset http_proxy
	poate fi sarit. ^ pt cand crapa ca nu gaseste unele pachete la build
	
petalinux-create -t project -n zynq_xpu --template zynq

cd ./zynq_xpu

project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi
	adauga *cod part 1* (vezi mai jos) - pt internet ssh + dma + niste reserved memory ca xpu sa read/write doar cu ea fara sa buseasca altceva din working ram

project-spec/meta-user/recipes-bsp/u-boot/files/platform-top.h 
	adauga *cod part 2* (vezi mai jos) - pt internet ssh
	
petalinux-config --get-hw-description ../../../../hw/mid_level/xpu_arm/xpu_arm.xsa
	image packaging configuration -> root filesystem type -> ext4 la format
	image packaging configuration -> root filesystem formats -> fara alea cu cpio 
petalinux-config --get-hw-description ../../../../hw/mid_level/xpu_arm/xpu_arm.xsa -c rootfs
	pachete diverse, dupa necesitate (posibil si cele de retea)
petalinux-config --get-hw-description ../../../../hw/mid_level/xpu_arm/xpu_arm.xsa -c kernel
	XILINX_PHY=y	(dau search cu "/")
	REALTEK_PHY=y   (^) (pt 20XX sau 28xx ethernet drivers)
	
for i in {1..10}; do petalinux-build && sleep 10s; done
	ocazional nu gaseste din prima unele pachete si crapa la build => more builds => ok
	
sdk generation : for cross compilation w/ vitis 
	petalinux-build --sdk
	
cd ./images/linux

petalinux-package --boot --fsbl zynq_fsbl.elf --u-boot u-boot.elf --fpga  ../../../../../../hw/mid_level/xpu_arm/xpu_arm.bit --force

petalinux-package --sysroot

copiez boot.bin ; boot.scr ; image.ub pe partitia de boot 
	sudo cp ./boot.scr /media/!!!USER!!!/boot/ && sudo cp ./BOOT.BIN /media/!!!USER!!!/boot/ && sudo cp ./image.ub /media/!!!USER!!!/boot/

copiez rootfs pe partitia de rootfs (dezarhivez)
	sudo tar -xf rootfs.tar.gz -C /media/!!!USER!!!/root















pt clean:
	petalinux-build -x mrproper
	

pt generat xsa: 
	vivado din proiectul deschis 
	file -> export -> export hardware -> include bitstream -> name + locatie
	pt generat bitstream: export bitstream


pt seriala
	sudo minicom -D /dev/ttyUSB1 -b 115200
		sau 
	sudo minicom -D /dev/ttyUSB0 -b 115200
		sau
	orice alt program de seriala, se foloseste: baud 115200; 8data, 1 stop, no parity 
	user petalinux; 
	parola setata la primul login: petalinux


pt ssh					
	connect pynq board to router 
	pynq: ifconfig -> get ip addr	
	normal ssh : ssh petalinux@IP_ADDR 
		user petalinux
		parola setata la primul login: petalinux 


pt vitis 
	create workspace
	platform project
		create platform project 
		name
		xsa : browse : select vivado generated xsa file 
		os: linux
		FARA boot components 
		finish 
		build 
	aplication project
		create application project
		select previously created platform 
		name
		application settings:
			sysroot path
				images/linux/sdk/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi
				(daca/dupa ce sa facut build la sdk)
			rootfs
				images/linux/rootfs.ext4
			kernel image 
				images/linux/image.ub
				
		template: linux hello world 
		copy *cod part 3* in ./src/helloworld.c
		build 
		copy ./name/Debug/name.elf -> pynq
		
	pynq: run w/ sudo 
			
			
pt schimbat bitstream:
	by default, petalinux copiaza catre fpga .bit cu care a fost generat (la generarea .xsa, am bifat include bitstream) 
	daca se doreste rularea altei versiuni de masina, fara recompilare linux:
		vivado - proiect dorit
		generate bitstream
		open hardware manager -> open target -> auto connect 
		program device 
	obs: la urmatorul boot se sterge, si se copiaza in fpga .bit original


pt clonat placi:
	petalinux genereaza by default un random mac
	daca se cloneaza o placa, imaginea ei, atunci se cloneaza cu tot cu mac si in cazul in care
		2 placi sunt puse in acelasi server, apare conflict. solutie: script care la boot sa atribuie alt mac:
	a) in rootfs, fac fisier /etc/mac.txt in care pun adresa mac dorita. ex: 00:01:02:03:04:05
	b) in rootfs, fac fisier /etc/init.d/mac_setter in care pun codul de la * cod part 4 * de mai jos
		daca apar caractere gunoi suplimentare, le curat
	c) in rootfs, fac symbolic link /etc/rc5.d/S99mac_setter care sa pointeze catre /etc/init.d/mac_setter
		sudo ln -sf etc/init.d/mac_setter /etc/rc5.d/S99mac_setter
	d) setari de router
	e) reboot placa













*cod part 1*
/include/ "system-conf.dtsi"
/ {
    amba {



       audio0:audio-codec-ctrl@43c00000 {
            compatible = "xlnk,audio-codec-ctrl-1.0";
            reg = <0x43c00000 0x10000>;
        };



       ethernet@e000b000 {
            phy-handle = <&ethernet_phy>;
            ethernet_phy: ethernet-phy@1{
                reg = <1>;
            };
        };
    };
        usb_phy0: phy0 {
                compatible = "ulpi-phy";
                #phy-cells = <0>;
                reg = <0xe0002000 0x1000>;
                view-port = <0x170>;
                drv-vbus;
        };
};



&i2c0 {
    status = "okay";
};



&i2c1 {
    status = "okay";
};



&audio0 {
        compatible = "generic-uio";
};



&usb0 {
        usb-phy = <&usb_phy0>;
};



&sdhci0 {
    status = "okay";
};


&amba_pl {
		#address-cells = <1>;
		#size-cells = <1>;
		compatible = "simple-bus";
		ranges ;
		axi_dma_1: dma@40400000 {
			#dma-cells = <1>;
			clock-names = "s_axi_lite_aclk", "m_axi_mm2s_aclk", "m_axi_s2mm_aclk";
			clocks = <&clkc 15>, <&clkc 15>, <&clkc 15>;
			compatible = "xlnx,axi-dma-7.1", "xlnx,axi-dma-1.00.a";
			reg = <0x40400000 0x10000>;
			xlnx,addrwidth = <0x20>;
			xlnx,sg-length-width = <0xe>;
			dma-channel@40400000 {
				compatible = "xlnx,axi-dma-mm2s-channel";
				dma-channels = <0x1>;
				xlnx,datawidth = <0x40>;
				xlnx,device-id = <0x0>;
			};
			dma-channel@40400030 {
				compatible = "xlnx,axi-dma-s2mm-channel";
				dma-channels = <0x1>;
				xlnx,datawidth = <0x40>;
				xlnx,device-id = <0x0>;
			};
		};
	};


/ {
        axidmatest_1: axidmatest@1 {
                        compatible ="xlnx,axi-dma-test-1.00.a";
                        dmas = <&axi_dma_1 0
                                &axi_dma_1 1>;
                        dma-names = "axidma0", "axidma1";
        } ;


};


/ {
   reserved-memory {
      #address-cells = <1>;
      #size-cells = <1>;
      ranges;

      reserved: buffer@0x18000000 {
         no-map;
         reg = <0x18000000 0x04000000>;
      };
   };

   reserved-driver@0 {
      compatible = "xlnx,reserved-memory";
      memory-region = <&reserved>;
   };
};


*end cod part 1*









*cod part 2*

#include <configs/zynq-common.h>
#define CONFIG_SYS_BOOTM_LEN 0xF000000



/*Required for uartless designs */
#ifndef CONFIG_BAUDRATE
#define CONFIG_BAUDRATE 115200
#ifdef CONFIG_DEBUG_UART
#undef CONFIG_DEBUG_UART
#endif
#endif

*end cod part 2*












*cod part 3*
/*
	example of basic program for the xpu system.
		write program file into xpu
		get data in into the xpu system using dma mechanism
		do computations (done automatically)
		get data out from the xpu system using dma mechanism
 */





#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>





															// XPU defines
#define XPU_NR_CELLS 64
#define XPU_BASE_ADDR 0x43c00000
#define XPU_STATUS_REG_ADDR_OFFSET 0x0
#define XPU_FIFO_PROGRAM_ADDR_OFFSET 0x0

#define XPU_WRITE_INT_ACK_ADDR 4
#define XPU_READ_SEL_READABLE_REGS_ADDR 4
#define XPU_WRITE_SEL_READABLE_REGS_ADDR 8
#define XPU_READABLE_REGS_PROGRAM_COUNTER_ADDR 0
#define XPU_READABLE_REGS_CYCLE_COUNTER_ADDR 1
#define XPU_READABLE_REGS_ACCELERATOR_ID 2

															// test related defines
#define NR_TRANSACTIONS XPU_NR_CELLS
#define TIME_DELAY 10000000

                                // DMA defines
#define DMA_BASE_ADDR 0x40400000

#define DMA_MM2S_DMACR_OFFSET 0x00
#define DMA_MM2S_DMASR_OFFSET 0x04
#define DMA_MM2S_SA_LSB_OFFSET 0x18
#define DMA_MM2S_SA_MSB_OFFSET 0x1C
#define DMA_MM2S_LENGTH_OFFSET 0x28
#define DMA_S2MM_DMACR_OFFSET 0x30
#define DMA_S2MM_DMASR_OFFSET 0x34
#define DMA_S2MM_DA_LSB_OFFSET 0x48
#define DMA_S2MM_DA_MSB_OFFSET 0x4C
#define DMA_S2MM_LENGTH_OFFSET 0x58

															// MM2S
		//	MM2S_DMACR - control register
	// rw
#define DMA_MM2S_DMACR_X_RS_LOC 0
#define DMA_MM2S_DMACR_X_RS_SIZE 1
#define DMA_MM2S_DMACR_X_RS_X_STOP 0
#define DMA_MM2S_DMACR_X_RS_X_RUN 1
	// rw
#define DMA_MM2S_DMACR_X_RESET_LOC 2
#define DMA_MM2S_DMACR_X_RESET_SIZE 1
#define DMA_MM2S_DMACR_X_RESET_X_PAUSED 0
#define DMA_MM2S_DMACR_X_RESET_X_START 1
	// rw
#define DMA_MM2S_DMACR_X_IOC_IrqEn_LOC 12
#define DMA_MM2S_DMACR_X_IOC_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_ENABLED 1
	// rw
#define DMA_MM2S_DMACR_X_Dly_IrqEn_LOC 13
#define DMA_MM2S_DMACR_X_Dly_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_ENABLED 1
	// rw
#define DMA_MM2S_DMACR_X_Err_IrqEn_LOC 14
#define DMA_MM2S_DMACR_X_Err_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_ENABLED 1


		//	MM2S_DMASR - status register
	// ro
#define DMA_MM2S_DMASR_X_Halted_LOC 0
#define DMA_MM2S_DMASR_X_Halted_SIZE 1
#define DMA_MM2S_DMASR_X_Halted_X_RUNNING 0
#define DMA_MM2S_DMASR_X_Halted_X_HALTED 1
	// ro
#define DMA_MM2S_DMASR_X_Idle_LOC 1
#define DMA_MM2S_DMASR_X_Idle_SIZE 1
#define DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_MM2S_DMASR_X_Idle_X_IDLE 1
	// ro
#define DMA_MM2S_DMASR_X_SGIncld_LOC 3
#define DMA_MM2S_DMASR_X_SGIncld_SIZE 1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_EN 1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_not_EN 0
	// ro
#define DMA_MM2S_DMASR_X_DMASlvErr_LOC 5
#define DMA_MM2S_DMASR_X_DMASlvErr_SIZE 1
#define DMA_MM2S_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMASlvErr_X_ERR_DETECTED 1
	// ro
#define DMA_MM2S_DMASR_X_DMADecErr_LOC 6
#define DMA_MM2S_DMASR_X_DMADecErr_SIZE 1
#define DMA_MM2S_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMADecErr_X_ERR_DETECTED 1
	// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_IOC_Irq_LOC 12
#define DMA_MM2S_DMASR_X_IOC_Irq_SIZE 1
#define DMA_MM2S_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED 1
	// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_Err_Irq_LOC 14
#define DMA_MM2S_DMASR_X_Err_Irq_SIZE 1
#define DMA_MM2S_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_Err_Irq_X_ERR_INT_DETECTED 1


		// MM2S_SA_LSB - source address
	// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_SIZE 32
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0


		// MM2S_SA_MSB - source address
	// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_SIZE 32
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR  0


		// MM2S_LENGTH
	// R/W
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_SIZE 26
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0


															// S2MM
		//	S2MM_DMACR - control register
	// rw
#define DMA_S2MM_DMACR_X_RS_LOC 0
#define DMA_S2MM_DMACR_X_RS_SIZE 1
#define DMA_S2MM_DMACR_X_RS_X_STOP 0
#define DMA_S2MM_DMACR_X_RS_X_RUN 1
	// rw
#define DMA_S2MM_DMACR_X_RESET_LOC 2
#define DMA_S2MM_DMACR_X_RESET_SIZE 1
#define DMA_S2MM_DMACR_X_RESET_X_PAUSED 0
#define DMA_S2MM_DMACR_X_RESET_X_START 1
	// rw
#define DMA_S2MM_DMACR_X_IOC_IrqEn_LOC 12
#define DMA_S2MM_DMACR_X_IOC_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_ENABLED 1
	// rw
#define DMA_S2MM_DMACR_X_Dly_IrqEn_LOC 13
#define DMA_S2MM_DMACR_X_Dly_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_ENABLED 1
	// rw
#define DMA_S2MM_DMACR_X_Err_IrqEn_LOC 14
#define DMA_S2MM_DMACR_X_Err_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_ENABLED 1


		//	S2MM_DMASR - status register
	// ro
#define DMA_S2MM_DMASR_X_Halted_LOC 0
#define DMA_S2MM_DMASR_X_Halted_SIZE 1
#define DMA_S2MM_DMASR_X_Halted_X_RUNNING 0
#define DMA_S2MM_DMASR_X_Halted_X_HALTED 1
	// ro
#define DMA_S2MM_DMASR_X_Idle_LOC 1
#define DMA_S2MM_DMASR_X_Idle_SIZE 1
#define DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_S2MM_DMASR_X_Idle_X_IDLE 1
	// ro
#define DMA_S2MM_DMASR_X_SGIncld_LOC 3
#define DMA_S2MM_DMASR_X_SGIncld_SIZE 1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_EN 1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_not_EN 0
	// ro
#define DMA_S2MM_DMASR_X_DMASlvErr_LOC 5
#define DMA_S2MM_DMASR_X_DMASlvErr_SIZE 1
#define DMA_S2MM_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMASlvErr_X_ERR_DETECTED 1
	// ro
#define DMA_S2MM_DMASR_X_DMADecErr_LOC 6
#define DMA_S2MM_DMASR_X_DMADecErr_SIZE 1
#define DMA_S2MM_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMADecErr_X_ERR_DETECTED 1
	// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_IOC_Irq_LOC 12
#define DMA_S2MM_DMASR_X_IOC_Irq_SIZE 1
#define DMA_S2MM_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED 1
	// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_Err_Irq_LOC 14
#define DMA_S2MM_DMASR_X_Err_Irq_SIZE 1
#define DMA_S2MM_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_Err_Irq_X_ERR_INT_DETECTED 1


		// S2MM_SA_LSB - source address
	// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_SIZE 32
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0


		// S2MM_SA_MSB - source address
	// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_SIZE 32
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR  0


		// S2MM_LENGTH
	// R/W
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_SIZE 26
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0







void AXI_LITE_write(void * addr, uint32_t value)
{
	*((volatile unsigned *)(addr)) = value;
	return;
}



uint32_t AXI_LITE_read(void * addr)
{
	uint32_t return_value;
	return_value = *((volatile unsigned *)(addr));
	return return_value;
}



void XPU_write_program_file_1(void * addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
{
	// algorithm:
		// data_out_from_dma = ixload + data_in_from_dma
	AXI_LITE_write(addr, 0x6f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000b);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000012);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001e);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x4700001f);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000020);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000021);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000022);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000026);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x1f580000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x64000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a0001ff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x20000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f500000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60ffffff);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x1f300000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x87000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4777ffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000001);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f080000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x12000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x67000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000003);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x00000000);	// function arguments for set addr regs
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x00000000);

	AXI_LITE_write(addr, 0x77000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000006);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x00000001);	// function arguments for wait mat
	AXI_LITE_write(addr, 0x00000000);

	AXI_LITE_write(addr, 0x6700000c);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x7f000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000005);
	AXI_LITE_write(addr, 0x10000000);
	return;
}



void XPU_write_program_file_2(void * addr) // data in ; no compute ; data out; addr regs: 0-0
{
	AXI_LITE_write(addr, 0x6f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000b);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000012);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001f);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000020);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000021);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000022);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000026);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x1f580000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x64000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a0001ff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x20000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f500000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60ffffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f300000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x87000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4777ffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f080000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x12000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x77000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000006);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x67000004);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x7f000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000005);
	AXI_LITE_write(addr, 0x10000000);
	return;
}



void dma_mm2s_status(uint32_t * dma_ptr)
{
	uint32_t status_reg = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2));
    printf("MM2S status (addr offset: 0x%x status:0x%x): ", DMA_MM2S_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001)
    {
    	printf("halted ");
    }
    else
    {
    	printf("running ");
    }

    if (status_reg & 0x00000002)
    {
    	printf("idle ");
    }
    if (status_reg & 0x00000008)
    {
    	printf("SG enabled ");
    }
    if (status_reg & 0x00000010)
    {
    	printf("DMA interrupt error ");
    }
    if (status_reg & 0x00000020)
    {
    	printf("DMA slave error ");
    }
    if (status_reg & 0x00000040)
    {
    	printf("DMA decode error ");
    }
    if (status_reg & 0x00001000)
    {
    	printf("IOC interrupt request ");
    }
    if (status_reg & 0x00002000)
    {
    	printf("Delay interrupt request ");
    }
    if (status_reg & 0x00004000)
    {
    	printf("Error interrupt request ");
    }

    printf("\n");
}



void dma_s2mm_status(uint32_t * dma_ptr)
{
	uint32_t status_reg = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2));
    printf("S2MM status (addr offset: 0x%x status: 0x%x): ", DMA_S2MM_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001)
	{
		printf("halted ");
	}
	else
	{
		printf("running ");
	}

	if (status_reg & 0x00000002)
	{
		printf("idle ");
	}
	if (status_reg & 0x00000008)
	{
		printf("SG enabled ");
	}
	if (status_reg & 0x00000010)
	{
		printf("DMA interrupt error ");
	}
	if (status_reg & 0x00000020)
	{
		printf("DMA slave error ");
	}
	if (status_reg & 0x00000040)
	{
		printf("DMA decode error ");
	}
	if (status_reg & 0x00001000)
	{
		printf("IOC interrupt request ");
	}
	if (status_reg & 0x00002000)
	{
		printf("Delay interrupt request ");
	}
	if (status_reg & 0x00004000)
	{
		printf("Error interrupt request ");
	}

    printf("\n");
}



void print_all_registers_mm2s(void * dma_ptr, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA mm2s registers: tag: %d \n", tag);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2) );
	printf("DMA mm2s: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );
	printf("DMA mm2s: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_SA_LSB_OFFSET>>2) );
	printf("DMA mm2s: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_SA_MSB_OFFSET>>2) );
	printf("DMA mm2s: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_LENGTH_OFFSET>>2) );
	printf("DMA mm2s: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA mm2s registers: \n");
	return;
}



void print_all_registers_s2mm(void * dma_ptr, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA s2mm registers: tag: %d\n", tag);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2) );
	printf("DMA s2mm: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );
	printf("DMA s2mm: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DA_LSB_OFFSET>>2) );
	printf("DMA s2mm: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DA_MSB_OFFSET>>2) );
	printf("DMA s2mm: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_LENGTH_OFFSET>>2) );
	printf("DMA s2mm: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA s2mm registers: \n");
	return;
}



void dma_mm2s_wait_transfers_complete(uint32_t * dma_ptr)
{
	uint32_t mm2s_status =  AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );

    while( ( mm2s_status & (1<<DMA_MM2S_DMASR_X_Idle_LOC) ) == DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_mm2s_status(dma_ptr);
        mm2s_status =  AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );
    }
}



void dma_s2mm_wait_transfers_complete(uint32_t * dma_ptr)
{
	uint32_t s2mm_status = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );

    while( ( s2mm_status & (1<<DMA_S2MM_DMASR_X_Idle_LOC) ) == DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_s2mm_status(dma_ptr);
        s2mm_status = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );
    }
}



void DMA_XPU_read(uint32_t * dma_ptr, uint32_t ddr_start_addr, uint32_t transfer_length)
{

	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 0);
    dma_mm2s_status(dma_ptr);
	printf("Writing source address\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_SA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(dma_ptr + (DMA_MM2S_SA_MSB_OFFSET>>2), 0x00000000);
	dma_mm2s_status(dma_ptr);
	printf("Starting MM2S channel\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 0x0001);
	dma_mm2s_status(dma_ptr);
	printf("Writing MM2S transfer length\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_LENGTH_OFFSET>>2), transfer_length);
	dma_mm2s_status(dma_ptr);
	printf("Waiting for MM2S to be done\n");
	dma_mm2s_wait_transfers_complete(dma_ptr);
	dma_mm2s_status(dma_ptr);
}



void DMA_XPU_write(uint32_t * dma_ptr, uint32_t ddr_start_addr, uint32_t transfer_length )
{
	printf("Writing destination address\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DA_MSB_OFFSET>>2), 0x00000000);
	dma_s2mm_status(dma_ptr);
	printf("Starting S2MM channel\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2), 0x0001);
	dma_s2mm_status(dma_ptr);
	printf("Writing S2MM transfer length\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_LENGTH_OFFSET>>2), transfer_length);
	dma_s2mm_status(dma_ptr);
	printf("Waiting for S2MM to be done\n");
	dma_s2mm_wait_transfers_complete(dma_ptr);
	dma_s2mm_status(dma_ptr);
}



void dma_reset(uint32_t * dma_ptr)
{
	printf("Resetting DMA\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
	dma_mm2s_status(dma_ptr);
	dma_s2mm_status(dma_ptr);
}



void print_main_mem(void* address, int32_t nr_bytes)
{
    char *p = address;

    for (int i = 0; i < nr_bytes; i++)
    {
        printf("%02x", p[i]);

        if ( (i+1) % 4 == 0)
        {
        	printf(" ");
        }

        if( (i+1) % (4*8) == 0)
		{
			printf("\n");
		}
    }

    printf("\n");
}





int main()
{
    void *xpu_ptr;
    uint64_t delay;
	unsigned int xpu_status_reg = 0x0;


	int32_t memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);


	xpu_ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, memory_file_descriptor, XPU_BASE_ADDR );
	uint32_t * dma_ptr = mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, DMA_BASE_ADDR);
	uint32_t * data_in_ptr  = mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x19000000);
	uint32_t * data_out_ptr = mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x1A000000);



    for(int i = 0 ; i < NR_TRANSACTIONS; i++ )					// data in generation
    {
    	data_in_ptr[i]= 50;
    }

    memset(data_out_ptr, 0, NR_TRANSACTIONS * sizeof(uint32_t) ); // Clear destination block

    printf("AXI xpu write program + run + dma test.\n");

    printf("Source memory block:      ");
    print_main_mem(data_in_ptr, NR_TRANSACTIONS * sizeof(uint32_t));

    printf("Destination memory block: ");
    print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t));



    dma_reset(dma_ptr);

    xpu_status_reg = *((volatile unsigned *)(xpu_ptr + XPU_STATUS_REG_ADDR_OFFSET));	// write program file
	printf("before loading program file : %x\n", xpu_status_reg);
	printf("xpu: start program_file_load \n");
	XPU_write_program_file_1(xpu_ptr + XPU_FIFO_PROGRAM_ADDR_OFFSET);
	printf("xpu: end program_file_load \n");

    																					// load data in; ddr->dma->xpu
	printf("dma->xpu: start load data in \n");
	DMA_XPU_read(dma_ptr, 0x19000000, NR_TRANSACTIONS * sizeof(uint32_t) );
	printf("dma->xpu: end load data in\n");

																						// interrupt ack
	AXI_LITE_write(xpu_ptr + XPU_WRITE_INT_ACK_ADDR,1);
	for (delay = 0; delay < TIME_DELAY; delay++)
	{
		;
	}
	xpu_status_reg = AXI_LITE_read(xpu_ptr + XPU_STATUS_REG_ADDR_OFFSET);
	printf("after interrupt ack: status reg: %x\n", xpu_status_reg);

																						// get data out; xpu -> dma -> ddr
	printf("xpu->dma: start load data out \n");
	DMA_XPU_write(dma_ptr, 0x1A000000, NR_TRANSACTIONS * sizeof(uint32_t) );
	printf("xpu->dma: end load data out\n");

																						// print results
    printf("Destination memory block: ");
    print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t));
    printf("\n");

    																					// unmap memory regions
    munmap(dma_ptr,65535);
    munmap(data_in_ptr,NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(data_out_ptr,NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(xpu_ptr,4096);



    return 0;
}




*end cod part 3*










* cod part 4 *



#!/bin/sh

ETHIF=eth0

# this assumes the file /etc/mac.txt , containing just the mac addr, example : 00:01:02:03:04:05
# also assumes eth0 is used

MACADDR_FILE=/etc/mac.txt

if [ -f $MACADDR_FILE ]; then
	MACADDR=$(<$MACADDR_FILE)
	echo Setting MAC address to $MACADDR

	ifconfig $ETHIF down
	ifconfig $ETHIF hw ether $MACADDR
	ifconfig $ETHIF up
fi

unset ETHIF MACADDR_FILE MACADDR




* end cod part 4 *





