import m5
from m5.objects import *
from caches import *

# simple arm cpu configuration with 2-level classic cache, following:
# https://www.gem5.org/documentation/learning_gem5/part1/cache_config/

################################################################################
# Parse Params                                                                 #
################################################################################
default_bin = "../stencil-3d/bin/stencil40"
################################################################################
# CPU - ArmTimingSimpleCPU                                                     #
################################################################################
system = System()
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]
system.cpu = ArmTimingSimpleCPU()
################################################################################
# Cache Hierarchy - two level classic cache                                    #
################################################################################
system.cpu.icache = L1ICache(size='8kB', assoc=2)
system.cpu.dcache = L1DCache(size='8kB', assoc=2)
system.cpu.icache.connectCPU(system.cpu)
system.cpu.dcache.connectCPU(system.cpu)
system.l2bus = L2XBar()
system.cpu.icache.connectBus(system.l2bus)
system.cpu.dcache.connectBus(system.l2bus)
system.l2cache = L2Cache(size='64kB', assoc=4)
system.l2cache.connectCPUSideBus(system.l2bus)
system.membus = SystemXBar()
system.l2cache.connectMemSideBus(system.membus)
################################################################################
# Memory                                                                       #
################################################################################
system.cpu.createInterruptController()
system.system_port = system.membus.cpu_side_ports
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports
################################################################################
# Syscall Emulation                                                            #
################################################################################
system.workload = SEWorkload.init_compatible(default_bin)
process = Process()
process.cmd = [default_bin]
system.cpu.workload = process
system.cpu.createThreads()
# intialize the system
root = Root(full_system = False, system = system)
m5.instantiate()
# simulation
print("Beginning simulation!")
exit_event = m5.simulate()
print('Exiting @ tick {} because {}'
      .format(m5.curTick(), exit_event.getCause()))