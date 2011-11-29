class Array
  def sum
    inject(0.0) { |result, el| result + el }
  end

  def mean
    sum / size
  end
end

file = "TURTLEC.LOG"

def dnorm(file)
  content = open(file, "rb") {|io| io.read }
  anz = 0
  oline = nil
  odata = nil
  File.open("d3_#{file}", 'w') do |f|
    (content.size / 2).times do |cnt|
      data = content[cnt*2+1]
      clk  = (content[cnt*2] & 0x01)
      st   = (content[cnt*2] & 0x02) >> 1
      line = [data, clk, st].join(",")
      if line != oline
        f.write("#{line},#{anz}\n")# if st > 0# && (data != odata)
        odata = (clk == 0) ? nil : data
        oline = line
        anz = 0
      else
        anz += 1
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