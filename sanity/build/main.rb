require 'socket'

sleep 0.1

s = TCPSocket.new('problem', 12345)

s.gets
b = 8.times.map{s.read(1)}
b.each{|x| s.write x}
STDERR.puts b.inspect
s.flush

sleep 0.5

s.puts '/home/user/send_flag'


sleep 0.5
