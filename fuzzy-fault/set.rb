require 'set'

files = [
	File.read('result0.txt'),
	File.read('result1.txt'),
	File.read('result2.txt'),
	File.read('result3.txt'),
	File.read('result4.txt'),
]

lists = files.map do |d|
	d.split('----------')[1..-1].map do |l|
		l.split("\n").to_set
	end
end

4.times do |i|
	p i
	[0, 1, 2, 3, 4].combination(2).to_a.map do |c|
		a, b = c
		p lists[a][i] & lists[b][i]
	end
end
