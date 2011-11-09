





@length = 0
@d = {}

def add(data, cnt)
  if @length == 0
    @length = data
  else
    @d[cnt] = data
    @length -= 1
  end
  if @length == 0
    keys = @d.keys
    puts "#{keys.min}-#{keys.max}: #{@d.values.join(' ')}"
    @d = {}
  end
end

size = 100000
content = content[0...size]
content = content.split("")
content.shift

#(contents.size / 2)
old_clk = 0
p1 = 0
p2 = 0
100000.times do |cnt|
  port2 = content[cnt*2+1]
  clk  = (content[cnt*2+1] & 0x02) >> 1
  st   = (content[cnt*2+1] & 0x04) >> 2
  if( old_clk != clk)
    if(clk == 1)
      data = (content[cnt*2] + content[(cnt + 1)*2]  + content[(cnt + 2)*2] + content[(cnt + 3)*2]) / 4
      add(data, cnt)
#      puts "#{cnt}: #{content[cnt*2]} #{data}"
    end
    old_clk = clk
  end

  # if p1 != port1 || p2 != port2
  #   puts "#{port1} #{port2}"
  #   p1 = port1
  #   p2 = port2
  # end
end


File.open("d_turtle2.log", "r") do |infile|
  while (line = infile.gets)
    data, clk, st = line.split(",")
    if( old_clk != clk)
  end
end

