class Array
  def sum
    inject(0.0) { |result, el| result + el }
  end

  def mean
    sum / size
  end
end

file = "turtle2.log"

=begin
content = open(file, "rb") {|io| io.read }

oline = nil
File.open("d_#{file}", 'w') do |f|
  (content.size / 2).times do |cnt|
    data = content[cnt*2]
    clk  = (content[cnt*2+1] & 0x02) >> 1
    st   = (content[cnt*2+1] & 0x04) >> 2
    line = [data, clk, st].join(",")

    if line != oline
      f.write("#{line}\n")
      oline = line
    end
  end
end
=end

odata = []
File.open("d2_#{file}", "w") do |f|
  File.open("d_#{file}", "r") do |infile|
    while (line = infile.gets)
      data, clk, st = line.strip.split(",").map(&:to_i)
      if clk > 0
        odata << data
      elsif odata.size > 0
        f.write("#{odata.mean.to_i},#{odata.join('-')}\n")
        odata = []
      end
    end
  end
end
