# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/vasile/Desktop/proiecte/git/xpu/sw/mid_level/vitis/dma_test/xpu_arm/xpu_arm_platform/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/vasile/Desktop/proiecte/git/xpu/sw/mid_level/vitis/dma_test/xpu_arm/xpu_arm_platform/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {xpu_arm_platform}\
-hw {/home/vasile/Desktop/proiecte/git/xpu/hw/mid_level/xpu_arm/xpu_arm.xsa}\
-proc {ps7_cortexa9} -os {linux} -no-boot-bsp -out {/home/vasile/Desktop/proiecte/git/xpu/sw/mid_level/vitis/dma_test/xpu_arm}

platform write
platform active {xpu_arm_platform}
platform generate
platform config -updatehw {/home/vasile/Desktop/proiecte/git/xpu/hw/mid_level/xpu_arm/xpu_arm.xsa}
platform generate -domains 
platform config -updatehw {/home/vasile/Desktop/proiecte/git/xpu/hw/mid_level/xpu_arm/xpu_arm.xsa}
platform config -updatehw {/home/vasile/Desktop/proiecte/git/xpu/hw/mid_level/xpu_arm/xpu_arm.xsa}
platform generate -domains 
platform clean
platform generate
platform generate
platform active {xpu_arm_platform}
platform active {xpu_arm_platform}
