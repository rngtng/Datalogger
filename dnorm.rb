class Array
  def sum
    inject(0.0) { |result, el| result + el }
  end

  def mean
    sum / size
  end
end

file = "TB.LOG"

def dnorm(file)
  content = open(file, "rb") {|io| io.read }
  anz = 0
  oline = []
  File.open("d1_#{file}", 'w') do |f|
    (content.size / 2).times do |cnt|
      data  = content[cnt*2+1]
      reset = (content[cnt*2] & 0x01)
      clk   = (content[cnt*2] & 0x02) >> 1
      st    = (content[cnt*2] & 0x10) >> 4
      busy  = (content[cnt*2] & 0x20) >> 5
      line = [reset, clk, st, data]
      if line != oline
       # print ",#{anz}\n"
        puts line.join(" - ") if st == 1 && clk == 1
        puts if st == 0
        #
        #   f.write(",#{anz}\n")
        #   f.write("\n") if line.first != oline.first
        #   f.write(line.join(","))
        # end
        oline = line
        anz = 0
      else
        anz += 1
      end
    end
  end
end

def dnorm2(file)
  content = open(file, "rb") {|io| io.read }
  anz = 0
  oline = nil
  odata = nil
  File.open("d2_#{file}", 'w') do |f|
    (content.size / 2).times do |cnt|
      data = content[cnt*2+1]
      clk  = (content[cnt*2] & 0x01)
      st   = (content[cnt*2] & 0x02) >> 1
      f.write("#{data}\n") if odata != data
      data = nil if odata == data && clk == 0 && st == 0
      odata = data
    end
  end
end


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

def cluster(file)
  content = open(file, "rb") {|io| io.read }
  anz = 0
  a = 0
  last = nil
  File.open("cluster_#{file}", 'w') do |f|
    (content.size / 2).times do |cnt|
      data = content[cnt*2+1]
      clk  = (content[cnt*2] & 0x01)
      st   = (content[cnt*2] & 0x02) >> 1

      if last != data && (clk == 1 || st == 1)
        last = data
        f.write(",#{anz}\n#{data}")
        add(data,cnt)
        anz = 0
        a += 1;
      else
        anz += 1
      end
      if st == 0
        last = nil
      end
    end
  end
end


dnorm(file)

def sort(file)
odata = []
File.open("d2_#{file}", "w") do |f|
  File.open("d_#{file}", "r") do |infile|
    while (line = infile.gets)
      data, clk, st = line.strip.split(",").map(&:to_i)
      odata << data if clk > 0

      if odata.size > odata.first
        f.write("#{odata.mean.to_i},#{odata.join('-')}\n")
        odata = []
      end
    end
  end
end
end

#sort(file)